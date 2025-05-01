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
/// @param conf is always the start of the static CONFS-array.
/// @param env 
/// @param info Accepts I2C device address as optional parameter.
/// @return nothing
napi_value cb_vl53l5cx_comms_init(napi_env env, napi_callback_info info);
void register_vl53l5cx_comms_init(
    VL53L5CX_Configuration* conf,
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


 /**
  * Check if sensor has new data
  */
napi_value cb_vl53l5cx_check_data_ready(napi_env, napi_callback_info);
void register_vl53l5cx_check_data_ready(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
);

/**
 * @brief Stop ranging
 * @param napi_env environment
 * @param napi_callback_info
 * @return Nothing
 * @throw on error
 */
napi_value cb_vl53l5cx_stop_ranging(napi_env, napi_callback_info);
void register_vl53l5cx_stop_ranging(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
);


/**
 * Get ranging data
 */
napi_value cb_vl53l5cx_get_ranging_data(napi_env, napi_callback_info);
void register_vl53l5cx_get_ranging_data(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
);

/**
 * Set resolution
 * 
 * Takes config slot and resolution as arguments.
 */
napi_value cb_vl53l5cx_set_resolution(napi_env, napi_callback_info);
void register_vl53l5cx_set_resolution(
    VL53L5CX_Configuration* conf, 
    napi_env env, 
    napi_value exports
);

/**
 * Set scanning frequency
 * 
 * Takes config slot and frequency as arguments.
 */
napi_value cb_vl53l5cx_set_ranging_frequency_hz(napi_env, napi_callback_info);
void register_vl53l5cx_set_ranging_frequency_hz(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
);

/**
 * Strongest or closest target selection
 * 
 * Takes config slot and order as arguments.
 */
napi_value cb_vl53l5cx_set_target_order(napi_env, napi_callback_info);
void register_vl53l5cx_set_target_order(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
);
#endif