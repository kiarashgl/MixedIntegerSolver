# Web Service for Solving Mixed-Integer Programs
This program is a web service that provides a solution for Linear Programming problems, using the [Open Solver Interface](https://github.com/coin-or/Osi) API.
All you have to do is to run the service with *npm*, and then upload an [LP-File](https://www.coin-or.org/Doxygen/Osi/classCoinLpIO.html#details), and then the result is shown on the page.
This app is based on Node.js and Express.js, and the OSI framework is integrated as a plugin using [Native Abstractions for Node.js (NAN)](https://github.com/nodejs/nan).
This project is currently supported on Linux.
## How to Install
At first, install the latest version of Node.js; for example, if you want to install version 13.x, enter this command in terminal: 
```bash
curl -sL https://deb.nodesource.com/setup_13.x | sudo -E bash -
sudo apt-get install -y nodejs
```
For installing the dependencies for OSI, we need an installer tool named [CoinBrew](https://coin-or.github.io/coinbrew/). Download its script from [here](https://raw.githubusercontent.com/coin-or/coinbrew/master/coinbrew).
Then, run these commands: (`/path/to/coinbrew` should be changed to the path to the downloaded script. e.g. If it is in your current directory, the path would be `./coinbrew`)
You may need to install liblapack or pkgconf.
```bash
chmod 777 /path/to/coinbrew
sudo apt install liblapack-dev
sudo apt install pkgconf
/path/to/coinbrew fetch --main-proj=Osi
sudo /path/to/coinbrew build --main-proj=Osi --prefix=/usr/local
sudo /path/to/coinbrew install --main-proj=Osi
```
Also, we use the [Clp](https://www.github.com/coin-or/Clp) solver with our interface. To install it, again we use Coinbrew. (You may need to install gfortran.)
```bash
sudo apt install gfortran
/path/to/coinbrew fetch Clp
sudo /path/to/coinbrew build Clp --prefix=/usr/local
sudo /path/to/coinbrew install Clp
```
Finally, you could `cd` into the main directory of the project and run these commands to install the node dependencies and run the webserver:
```bash
npm install
npm start
```
The web server would run on http://localhost:3000. You should upload a [.lp file](https://www.coin-or.org/Doxygen/Osi/classCoinLpIO.html#details) on the form, and then you will get the linear optimization result.
- If the port 3000 was in use, run this command to free this port:
```bash
sudo kill -9 `sudo lsof -t -i:3000`
```
## Dependencies
This project's dependencies are listed below:
- [Node.js](https://github.com/nodejs/node)
- [npm](https://github.com/npm/cli)
- [nan](https://github.com/nodejs/nan)
- [Open Solver Interface](https://github.com/coin-or/Osi)
- [Clp](https://www.github.com/coin-or/Clp)
- [Express](https://github.com/expressjs/express)
- [pug](https://github.com/pugjs/pug)
- [multer](https://github.com/expressjs/multer)
- bindings
- cookie-parser
- debug
- http-errors
- morgan

The last 4 dependencies are required by the Express framework.
## Issues
If you have any problems with running the project or any other issues, feel free to mention it in the GitHub issues.
