var Transform = require('stream').Transform;
var util = require('util');
var Resampler  = require('./build/Release/node_resample');

// Transformの継承
util.inherits(resampler, Transform);

function resampler(inRate, outRate){
    Transform.call(this);	
    Resampler.new(inRate, outRate);
}



// ここで主に処理をする
resampler.prototype._transform = function(chunk, encoding, callback){

//this.push(chunk);
//console.log(chunk.length);
	Resampler.resampler(chunk, function(a){
	//console.log(a.length);
		this.push(a);
	}.bind(this),function(x){console.log(x);});
callback();
};

//必要ならflushの処理を追加する
//MyTransform.prototype._flush = function(chunk, encoding, callback){
//};

module.exports = samplerate;
