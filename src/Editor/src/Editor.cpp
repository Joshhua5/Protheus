#include <napi.h>

using namespace Napi;

Object InitAll(Env env, Object exports){
    return exports;
}


NODE_API_MODULE(protheus, InitAll)

