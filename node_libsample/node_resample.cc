#include "node_resample.h"

Persistent<Function> Resampler::constructor;

Resampler::Resampler(double x/*, double y*/) {
	//target_samplerate = y;
	//in_samplerate = x;
	ratio = x;
	//cout << x << endl;
	int* err = 0;
	const int type = SRC_SINC_BEST_QUALITY;   // = 0
  //const int type = SRC_SINC_MEDIUM_QUALITY; // = 1 
  //const int type = SRC_SINC_FASTEST; // = 2
  //const int type = SRC_ZERO_ORDER_HOLD; // = 3;
  //const int type = SRC_LINEAR; // = 4
  state = src_new(type,1,err);
}

Resampler::~Resampler() {
}

void Resampler::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Resampler"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("resample"),
      FunctionTemplate::New(resample)->GetFunction());
  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Resampler"), constructor);
}


Handle<Value> Resampler::New(const Arguments& args) {
HandleScope scope;

if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    double x = args[0]->NumberValue();
    //double y = args[1]->NumberValue();
    Resampler* obj = new Resampler(x/*,y*/);
   // cout << x << endl;
    //cout << y << endl;
    obj->Wrap(args.This());
    return args.This();
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    return scope.Close(constructor->NewInstance(argc, argv));
  }
}

Handle<Value> Resampler::resample(const Arguments& args) {
  HandleScope scope;
  Resampler* obj = ObjectWrap::Unwrap<Resampler>(args.This());
	double ratio = obj->ratio;


  long length = Buffer::Length(args[0])/2;//frame数＝byte/2
  // read
  //cout << length <<endl;
  char* in= Buffer::Data(args[0]);
  
  //Local<Function> cb = Local<Function>::Cast(args[1]);
  
  short int*  input = reinterpret_cast<short int*>(in);
  vector<short int> result;

  // convert to float
  vector<float> before(length);
  vector<float> after(static_cast<long>(length*ratio));
  src_short_to_float_array(input, before.data(), length);

  // setting for resampling
  SRC_DATA data;
  data.data_in = before.data();
  data.input_frames = length;
  data.data_out = after.data();
  data.output_frames = length*ratio;
  data.src_ratio = ratio;
  data.end_of_input = 0;
  src_process(obj->state, &data);

  // convert back to short int
  //before.clear();
  //cout << data.input_frames <<endl;
  //cout << data.output_frames_gen <<endl;
  //cout << data.output_frames <<endl;
  result.resize(data.output_frames_gen);
   //cout << "resize" <<endl; 
  src_float_to_short_array(after.data(), result.data(), result.size());
 //cout << length <<endl; 
 int len =  result.size()*2;
  Buffer *slowBuffer = Buffer::New(len);
memcpy(Buffer::Data(slowBuffer), result.data(), len);

Local<Object> globalObj = Context::GetCurrent()->Global();
Local<Function> bufferConstructor = Local<Function>::Cast(globalObj->Get(String::New("Buffer")));
Handle<Value> constructorArgs[3] = { slowBuffer->handle_, v8::Integer::New(len), v8::Integer::New(0) };
Local<Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);
  
  //cb->Call(Context::GetCurrent()->Global(), 1, &constructorArgs[0]);
  return actualBuffer;
}

