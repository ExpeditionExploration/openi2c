#include "funcs.h"
#include <node/node_api.h>
#include <string.h>

napi_value init(napi_env env, napi_value exports) {

    napi_value seti2c;
    napi_create_function(env, "setI2CConfig", strlen("seti2cconfig"),
                         cb_setI2CSettings, NULL, &seti2c);
    napi_set_named_property(env, exports, "setI2CConfig", seti2c);

    napi_value geti2c;
    napi_create_function(env, "getI2CConfig", strlen("geti2cconfig"),
                         cb_getI2CSettings, NULL, &geti2c);
    napi_set_named_property(env, exports, "getI2CConfig", geti2c);

    return exports;
}
NAPI_MODULE(bno08x_native, init)
