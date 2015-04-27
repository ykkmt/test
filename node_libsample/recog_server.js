var child_process = require("child_process");
//websocket待ち受け
var WebSocketServer = require('ws').Server
var wss = new WebSocketServer({
	host : '0.0.0.0',
	port : 8889
});
var opt = new Object();
opt.silent = true;
var fs = require('fs');

//セッション管理（仮）
var connections = [];

//websocket event
wss.on('connection', function(ws) {
	console.log("websocket open");
   	connections.push(ws);

//var inpcm = fs.createWriteStream('in.pcm');
//var outpcm = fs.createWriteStream('out.pcm');
		var child = child_process.fork("test.js",opt);
		child.stdout.on('data', function (data) {
			//outpcm.write(data);
			});
child.stdout.on('end', function (data) {
	console.log("end_server");	
});
child.stdout.on('error', function (data) {
	console.log("error");	
});

	ws.on('message', function(message) {
		//inpcm.write(message);
		child.stdin.write(message);
	});
});

console.log("end");


