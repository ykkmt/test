#include <node.h>
#include "node_resample.h"

using namespace v8;
using namespace node;



void InitAll(Handle<Object> exports) {
  Resampler::Init(exports);
}

NODE_MODULE(resampler, InitAll)

