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
  const file = path.join(global.appRoot, req.file.path);
  var logStrings = [];
  var progress = function(str)
  {
    logStrings.push(str.replace(/[^\x00-\x7F]/g, ""));
    global.io.emit('logs', {log: logStrings.join('\n')});
  }

  var ret = function()
  {
    return logStrings;
  }

  var progress2 = function(str)
  {
    global.io.emit('answer', {ans: str});
  }
  osi.solve(file, progress, progress2);
  res.render('result', {title: "Upload Successful", message: "Your upload was successful!"});
  res.setHeader('Content-Type', 'text/html; charset=utf-8');
})
module.exports = router;
