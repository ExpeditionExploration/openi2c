#ifndef FUNCS_H
#define FUNCS_H

#include <node/node_api.h>

napi_value cb_setI2CSettings(napi_env env, napi_callback_info info);
napi_value cb_getI2CSettings(napi_env env, napi_callback_info _);

#endif
