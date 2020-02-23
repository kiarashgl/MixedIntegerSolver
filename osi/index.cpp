#include "Osi.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll)
{
    Nan::Set(target, Nan::New("solve").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(solveLp)).ToLocalChecked());
}

NODE_MODULE(Osi, InitAll);