#include <node.h>
#include <v8.h>
#include <node_buffer.h>
#include <iostream>
#include <vector>
#include <string>
#include <samplerate.h>
#include <fstream>
#include <string.h> 
#include <stdio.h>

using namespace std;
using namespace v8;
using namespace node;

class Resampler;

class Resampler : public ObjectWrap {
public:
  static void Init(Handle<Object> exports);

private:
  explicit Resampler(double x/*,double y*/);
  ~Resampler();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> resample(const v8::Arguments& args);
  static v8::Persistent<v8::Function> constructor;
  //double target_samplerate;
	//double in_samplerate;
	double ratio;
	SRC_STATE* state;
};
