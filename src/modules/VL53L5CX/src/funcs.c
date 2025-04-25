#include <node_api.h>
#include <vl53l5cx_api.h>

#include "funcs.h"


/************
 * Comms init
 */

/// @brief Initialize comms
/// @param env Node environment
/// @param info Device addr as optional arg
/// @return true on success
napi_value cb_vl53l5cx_comms_init(napi_env env, napi_callback_info info) {
    napi_value argv[2] = {0};
    napi_value this;
    size_t argc;
    VL53L5CX_Configuration conf[1];
    napi_status status;
    uint32_t read_sensor_address;
    uint16_t sensor_address;
    napi_value return_boolean;

    printf("Entering func\n");
    status = napi_get_cb_info(env, info, &argc, argv, &this, (void**)&conf);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_comms_init"
        );
    }
    printf("Got cb_info\n");
    // Accept 1 argument which is the sensor address.
    if (argc > 0) {
        status = napi_get_value_uint32(env, argv[0], &read_sensor_address);
        if (status != napi_ok) {
            napi_throw_error(
                env, 
                "argument error", 
                "Accepting an unsigned number"
            );
        } else {
            // Change to the address given as argument.
            sensor_address = (uint16_t)read_sensor_address;
            conf->platform.address = sensor_address;
        }
    }
    printf("Got uint32_t value\n");

    uint8_t comms_awry = vl53l5cx_comms_init(&conf->platform);
    if(comms_awry) {
        napi_throw_error(env, "init error", "could not initialize comms");
        napi_get_boolean(env, false, &return_boolean);
        return return_boolean;
    }
    napi_get_boolean(env, true, &return_boolean);
    return return_boolean;
}

void register_vl53l5cx_comms_init(
    VL53L5CX_Platform* platform,
    napi_env env,
    napi_value exports
) {
    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_comms_init";
    VL53L5CX_Configuration conf;

    napi_callback cb = cb_vl53l5cx_comms_init;
    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb,
        &conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "initialization error", 
            "Could not create JS func cb_vl53l5cx_comms_init"
        );
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            "initialization error",
            "Could not bind JS func cb_vl53l5cx_comms_init to the module"
        );
    }
}


/***********
 * Is alive?
 */

/// @brief Initialize comms
/// @param env Node environment
/// @param info 
/// @return 
napi_value cb_vl53l5cx_is_alive(napi_env env, napi_callback_info info) {
    napi_value argv[1] = {0};
    napi_value this;
    size_t argc;
    VL53L5CX_Configuration* conf;
    napi_status status;
    uint8_t is_alive;
    uint8_t is_alive_fcall_status;
    napi_value return_value;

    status = napi_get_cb_info(env, info, &argc, argv, &this, (void**)&conf);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_is_alive"
        );
    }

    // Call the driver's relevant function.
    is_alive_fcall_status = vl53l5cx_is_alive(conf, &is_alive);
    if (!is_alive || is_alive_fcall_status) {
        // VL53LCX not found at the address.
        napi_get_boolean(env, false, &return_value);
    } else {
        // Yay, everything is okkay.
        napi_get_boolean(env, true, &return_value);
    }
    return return_value;
}

void register_vl53l5cx_is_alive(
    VL53L5CX_Configuration* conf,
    napi_env env, 
    napi_value exports
) {
    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_is_alive";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_vl53l5cx_is_alive,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "initialization error", 
            "Could not create JS func cb_vl53l5cx_is_alive"
        );
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            "initialization error",
            "Could not bind JS func cb_vl53l5cx_is_alive to the module"
        );
    }
}