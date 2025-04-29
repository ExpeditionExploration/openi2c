#include <stdio.h>

#include <node_api.h>
#include <vl53l5cx_api.h>

#include "funcs.h"

/* Maximum length of formatted error message. */
#define MAX_LEN_ERROR 80



/************
 * Comms init
 */

/// @brief Initialize comms. This method must be called exactly once for device.
/// @param env Node environment
/// @param info Device addr as optional arg
/// @return nothing
napi_value cb_vl53l5cx_comms_init(napi_env env, napi_callback_info info) {
    napi_value argv[1] = {0};
    napi_value this;
    size_t argc = 0;
    void* data = NULL;
    napi_status status;
    uint32_t read_sensor_address;

    status = napi_get_cb_info(env, info, &argc, argv, &this, &data);
    if (status != napi_ok) {
        const napi_extended_error_info* error_info;
        napi_get_last_error_info(env, &error_info);
        char msg[MAX_LEN_ERROR] = {0};
        snprintf(
            msg, 
            MAX_LEN_ERROR-1, 
            "NAPI error: %s\n", 
            error_info->error_message
        );
        napi_throw_error(
            env, 
            MODULE_FUNC_INFO_ERROR, 
            msg
        );
        return NULL;
    }
    VL53L5CX_Platform* platform = (VL53L5CX_Platform*) data;
    printf("plt addr: %p\n", platform);

    // Accept 1 argument which is the sensor address.
    if (argc > 0) {
        status = napi_get_value_uint32(env, argv[0], &read_sensor_address);
        if (status != napi_ok) {
            napi_throw_error(
                env, 
                MODULE_FUNC_ARGUMENT_ERROR, 
                "Accepting an unsigned number in 8-bit range"
            );
            return NULL;
        } else {
            // Change to the address given as argument.
            platform->address = (uint8_t)read_sensor_address;
        }
    }

    uint8_t comms_awry = vl53l5cx_comms_init(platform);
    if(comms_awry) {
        char err[MAX_LEN_ERROR] = {0};
        snprintf(
            err, 
            MAX_LEN_ERROR-1, 
            "couldn't establish comms with vl53l5cx at %hhx", 
            platform->address
        );
        napi_throw_error(
            env, 
            INIT_COMMS_ERROR, 
            err
        );
    }
    return NULL;
}

void register_vl53l5cx_comms_init(
    VL53L5CX_Platform* platform,
    napi_env env,
    napi_value exports
) {
    printf("plt addr: %p\n", platform);

    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_comms_init";

    napi_callback cb = cb_vl53l5cx_comms_init;
    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb,
        platform, 
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

/// @brief Is the sensor alive? In case of a problem throws an error.
///        Sensor may have multiple errors, but _only one_ error will be thrown.
///        For such cases see `drv_status` and compare to the error values in
///        `vl53lcx_api.h` in the Linux driver.
/// @param env Node environment
/// @param info 
/// @return Nothing
napi_value cb_vl53l5cx_is_alive(napi_env env, napi_callback_info info) {
    napi_value this_;
    size_t argc;
    void* data;
    napi_status status;
    uint8_t is_alive = 0;
    uint8_t drv_status = 0;

    status = napi_get_cb_info(env, info, &argc, NULL, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_is_alive"
        );
        return NULL;
    }
    VL53L5CX_Configuration* conf = (VL53L5CX_Configuration*) data;

    // Call the driver's relevant function.
    drv_status = vl53l5cx_is_alive(conf, &is_alive);
    if (!is_alive || drv_status) {
        // VL53LCX has problems.
        char msg[MAX_LEN_ERROR] = {0};
        
        if (drv_status & VL53L5CX_STATUS_TIMEOUT_ERROR) {
            snprintf(
                msg,
                MAX_LEN_ERROR-1, 
                "timeout waiting for sensor at 0x%hhx, drv_status: 0x%hhx",
                (uint8_t)conf->platform.address,
                drv_status
            );
            napi_throw_error(
                env, 
                TIMEOUT_ERROR, 
                msg
            );
            *msg = 0;
        } else if (drv_status & VL53L5CX_STATUS_CORRUPTED_FRAME) {
            snprintf(
                msg,
                MAX_LEN_ERROR-1,
                "corrupted frame while communicating with sensor at 0x%hhx,  drv_status: 0x%hhx",
                (uint8_t)conf->platform.address,
                drv_status
            );
            napi_throw_error(
                env,
                CORRUPTED_FRAME_ERROR,
                msg
            );
            *msg = 0;
        } else if (drv_status & VL53L5CX_STATUS_XTALK_FAILED) {
            snprintf(
                msg,
                MAX_LEN_ERROR-1,
                "xtalk correction error with sensor at 0x%hhx, drv_status: 0x%hhx",
                (uint8_t)conf->platform.address,
                drv_status
            );
            napi_throw_error(
                env,
                XTALK_ERROR,
                msg
            );
            *msg = 0;
        } else if (drv_status & VL53L5CX_STATUS_INVALID_PARAM) {
            snprintf(
                msg,
                MAX_LEN_ERROR-1,
                "invalid parameter to sensor at 0x%hhx, drv_status: 0x%hhx",
                (uint8_t)conf->platform.address,
                drv_status
            );
            napi_throw_error(
                env,
                INVALID_PARAM_ERROR,
                msg
            );
            *msg = 0;
        }
    }
    return NULL;
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
            MODULE_INIT_ERROR, 
            "Could not create JS func cb_vl53l5cx_is_alive"
        );
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
            "Could not bind JS func cb_vl53l5cx_is_alive to the module"
        );
    }
}


/***************
 * Start ranging
 */

 napi_value cb_vl53l5cx_start_ranging(napi_env env, napi_callback_info info) {
    napi_value this_;
    size_t argc;
    void* data;
    napi_status status;
    uint8_t drv_status = 0;

    status = napi_get_cb_info(env, info, &argc, NULL, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_start_ranging"
        );
        return NULL;
    }
    VL53L5CX_Configuration* conf = (VL53L5CX_Configuration*) data;

    char msg[MAX_LEN_ERROR] = {0};
    drv_status = vl53l5cx_start_ranging(conf);
    if (drv_status) {
        snprintf(
            msg,
            MAX_LEN_ERROR-1,
            "Error with drv_status 0x%hhx. fn: cb_vl53l5cx_start_ranging",
            drv_status
        );
        napi_throw_error(
            env, 
            ERROR, 
            msg
        );
    }
    return NULL;
 }

 void register_vl53l5cx_start_ranging(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
 ) {
    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_start_ranging";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_vl53l5cx_start_ranging,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            MODULE_INIT_ERROR, 
            "Could not create JS func cb_vl53l5cx_start_ranging"
        );
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
            "Could not bind JS func cb_vl53l5cx_start_ranging to the module"
        );
    }
}


/**
 * VL53L5CX_wait_for_dataready
 */

 napi_value cb_VL53L5CX_wait_for_dataready(
    napi_env env,
    napi_callback_info info
) {
    napi_value this_;
    size_t argc;
    void* data;
    napi_status status;
    napi_value ret_val;
    uint8_t drv_status = 0;

    status = napi_get_cb_info(env, info, &argc, NULL, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_is_alive"
        );
        return NULL;
    }
    VL53L5CX_Configuration* conf = (VL53L5CX_Configuration*) data;

    drv_status = VL53L5CX_wait_for_dataready(&conf->platform);
    drv_status
        ? napi_get_boolean(env, true, &ret_val)
        : napi_get_boolean(env, false, &ret_val);
    return ret_val;
 }

 void register_VL53L5CX_wait_for_dataready(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
 ) {
    napi_value fn;
    napi_status status;
    // In the driver the chip code is in capital letters.
    const char* name = "vl53l5cx_wait_for_dataready";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_VL53L5CX_wait_for_dataready,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            MODULE_INIT_ERROR, 
            "Could not create JS func vl53l5cx_wait_for_dataready"
        );
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
            "Could not bind JS func vl53l5cx_wait_for_dataready to the module"
        );
    }

 }