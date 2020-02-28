#include "Osi.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll)
{
    Nan::Set(target, Nan::New("solve").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(solveWeb)).ToLocalChecked());

    Nan::Set(target, Nan::New("solveJson").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(solveJson)).ToLocalChecked());
}

NODE_MODULE(Osi, InitAll);