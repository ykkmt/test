var Transform = require('stream').Transform;
var util = require('util');
var Resampler  = require('./build/Release/resampler');

// Transformの継承
util.inherits(resampler, Transform);
 var obj;
function resampler(inRate, outRate){
    Transform.call(this);	
    obj = Resampler.Resampler(outRate/inRate);
}



// ここで主に処理をする
resampler.prototype._transform = function(chunk, encoding, callback){
	obj.resample(chunk, function(a){
		this.push(a);
	}.bind(this));
callback();
};

//必要ならflushの処理を追加する
//MyTransform.prototype._flush = function(chunk, encoding, callback){
//};

module.exports = resampler;
