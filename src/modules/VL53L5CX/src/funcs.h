#ifndef __FUNCS_H
#define __FUNCS_H

#include <string.h>
#include <node_api.h>
#include <vl53l5cx_api.h>

#include "error.h"


/******************
 * Initialize comms
 */

/// @brief Initialize comms to the sensor.
/// @param env 
/// @param info Accepts I2C device address as optional parameter.
/// @return nothing
napi_value cb_vl53l5cx_comms_init(napi_env env, napi_callback_info info);
void register_vl53l5cx_comms_init(
    VL53L5CX_Platform* platform,
    napi_env env,
    napi_value exports
);


/*******************************
 * Check if the sensor is alive.
 */

/// @brief Check whether the sensor is alive.
/// @param env Node environment
/// @param info Call paramas and what-not
/// @return boolean on the status
napi_value cb_vl53l5cx_is_alive(napi_env env, napi_callback_info info);

void register_vl53l5cx_is_alive(
    VL53L5CX_Configuration* conf, 
    napi_env env, 
    napi_value exports
);


/***************
 * Start ranging
 */

 napi_value cb_vl53l5cx_start_ranging(napi_env env, napi_callback_info info);
 void register_vl53l5cx_start_ranging(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
 );

#endif