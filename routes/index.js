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
  
  const output = osi.solve(file);
  res.render('result', {title: "Upload Successful", message: "Your upload was successful!", result: output});
})
module.exports = router;
