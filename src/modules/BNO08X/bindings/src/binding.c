#include "funcs.h"
#include <node/node_api.h>
#include <stdint.h>
#include <string.h>

static uint8_t register_fn(napi_env env, napi_value exports, const char* name,
                           napi_callback cb, void* context) {
    napi_value  fn;
    napi_status status;
    status = napi_create_function(env, name, strlen(name), cb, context, &fn);
    status |= napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(env, "init c extension error",
                         "couldn't register an fn");
        return 1;
    }

    return 0;
}

napi_value init(napi_env env, napi_value exports) {

    uint8_t status = 0;

    register_fn(env, exports, "setI2CConfig", cb_setI2CSettings, NULL);
    register_fn(env, exports, "getI2CConfig", cb_getI2CSettings, NULL);
    register_fn(env, exports, "service", cb_service, NULL);
    register_fn(env, exports, "setSensorCallback", cb_setSensorCallback, NULL);
    return exports;
}
NAPI_MODULE(bno08x_native, init)
