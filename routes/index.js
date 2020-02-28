var express = require('express');
var router = express.Router();
var Multer = require('multer');
const storage = Multer.diskStorage({
  destination: function (req, file, callback) {
    callback(null, 'uploads/')
  },
  filename: function (req, file, callback) {
    callback(null, file.originalname)
  }
});
var upload = new Multer({storage: storage});
var osi = require('bindings')('Osi.node');
var path = require('path');

router.get('/', function(req, res, next) {
  res.render('index', { title: 'Mixed Integer LP Solver' });
});

router.post('/submit', upload.single('lpfile'), (req, res) =>
{
  if (!req.file || req.file === "")
    res.end("Error: No file uploaded.\n")
  else
  {
    const file = path.join(global.appRoot, req.file.path);
    var logStrings = [];
    var sendLogs = function()
    {
      global.io.emit('logs', {log: logStrings.join('\n')});
    }
    var progress = function(str)
    {
      logStrings.push(str.replace(/[^\x00-\x7F]/g, ""));
      sendLogs();
    }
    var answer = function(ans, json)
    {
      global.io.emit('answer', {ans: ans, json: json});
    }
    osi.solve(file, progress, answer);
    res.setHeader('Content-Type', 'text/html; charset=utf-8');
    res.render('result', {title: "Upload Successful", message: "Your upload was successful!"});
    setTimeout(sendLogs, 1000);
  }
});

router.post('/json', upload.single('lpfile'), (req, res) =>
{
  if (!req.file || req.file === "")
    res.end("Error: No file uploaded.\n")
  else
  {
    const file = path.join(global.appRoot, req.file.path);
    var myJson = osi.solveJson(file);
    res.json(JSON.parse(myJson));
  }
})
module.exports = router;
