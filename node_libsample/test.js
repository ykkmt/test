var Res = require('./build/Release/resampler');
var fs =require('fs');
var obj = Res.Resampler(16000/48000);
var input = process.stdin;
//var input = fs.createReadStream('intest.pcm');
var output = process.stdout;
//var output = fs.createWriteStream('outtest.pcm');

input.on('data',function(data){
	output.write(obj.resample(data));
});

