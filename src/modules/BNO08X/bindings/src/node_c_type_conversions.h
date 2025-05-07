
#ifndef NODE_C_TYPE_CONVERSIONS
#define NODE_C_TYPE_CONVERSIONS

#include "sh2/sh2.h"
#include <node/node_api.h>

napi_value mkNapiSensorEvent(napi_env env, sh2_SensorEvent_t *ev);
napi_value mkProductId(napi_env env, sh2_ProductId_t *id);
napi_value mkSensorIdEnum(napi_env env);

#endif
