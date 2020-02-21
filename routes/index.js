var express = require('express');
var router = express.Router();
var Multer = require('multer');
var upload = new Multer({dest: 'uploads/'});
/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Mixed Integer LP Solver' });
});

router.post('/submit', upload.single('lpfile'), (req, res) =>
{
  console.log("Got a file!");
  res.render('result', {title: "Thanks", message: "Got your file!"})
})
module.exports = router;
