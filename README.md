# Web Service for Solving Mixed-Integer Programs
This program is a web service that provides a solution for Linear Programming problems, using the [Open Solver Interface](https://github.com/coin-or/Osi) API.
All you have to do is to run the service with [npm](https://github.com/npm/cli), and then upload an [LP-File](https://www.coin-or.org/Doxygen/Osi/classCoinLpIO.html#details), and then the result is shown on the page.
This app is based on Node.js and Express.js, and the OSI framework is integrated as a plugin using [Native Abstractions for Node.js (NAN)](https://github.com/nodejs/nan).
This project is currently supported on Linux.
## How to Install
At first, install the latest version of Node.js; for example, if you want to install version 13.x, enter this command in terminal: 
```bash
curl -sL https://deb.nodesource.com/setup_13.x | sudo -E bash -
sudo apt-get install -y nodejs
```
We use the [Cbc](https://www.github.com/coin-or/Cbc) solver with our interface. For installing the dependencies for Cbc and OSI, we need an installer tool named [CoinBrew](https://coin-or.github.io/coinbrew/). Download its script from [here](https://raw.githubusercontent.com/coin-or/coinbrew/master/coinbrew).
Then, run these commands: (`/path/to/coinbrew` should be changed to the path to the downloaded script. e.g. If it is in your current directory, the path would be `./coinbrew`)
You may need to install liblapack, pkgconf or gfortran.
```bash
chmod 777 /path/to/coinbrew
sudo apt install liblapack-dev
sudo apt install pkgconf
sudo apt install gfortran
/path/to/coinbrew fetch Cbc
sudo /path/to/coinbrew build Cbc --prefix=/usr/local
sudo /path/to/coinbrew install cbc
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
Also, you could send a `POST` request to the url http://localhost:3000/json containing the .lp file, to get the result as a `JSON` string. For example, if you want to solve an .lp file named `sampleFile.lp` in your current directory, simply run the following commands: (You may need to install curl.)
```bash
sudo apt install curl
curl -F lpfile=@sampleFile.lp localhost:3000/json
```
## Dependencies
This project's dependencies are listed below:
- [Node.js](https://github.com/nodejs/node)
- [npm](https://github.com/npm/cli)
- [nan](https://github.com/nodejs/nan)
- [Open Solver Interface](https://github.com/coin-or/Osi)
- [Cbc](https://www.github.com/coin-or/Cbc)
- [Express](https://github.com/expressjs/express)
- [pug](https://github.com/pugjs/pug)
- [multer](https://github.com/expressjs/multer)
- [Socket.io](https://github.com/socketio/socket.io)
- [bindings](https://github.com/TooTallNate/node-bindings)
- cookie-parser
- debug
- http-errors
- morgan

The last 4 dependencies are required by the Express framework.
## Issues
If you have any problems with running the project or any other issues, feel free to mention it in the GitHub issues.
