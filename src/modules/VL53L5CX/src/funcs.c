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
    printf("comms init called\n");
    napi_value argv[1] = {NULL};
    napi_value this;
    size_t argc = 0;
    void* data = NULL;
    napi_status status;

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
    if (argc != 1) {
        napi_throw_error(
            env, 
            "expected config index", 
            "Must give device index (0-9) as argument."
        );
        return NULL;
    }
    uint32_t device_ndx = 0;
    napi_get_value_uint32(env, argv[0], &device_ndx);
    VL53L5CX_Configuration* config = 
        ((VL53L5CX_Configuration*) data) + device_ndx;
    
    uint8_t comms_awry = vl53l5cx_comms_init(&config->platform);
    if(comms_awry) {
        char err[MAX_LEN_ERROR] = {0};
        snprintf(
            err, 
            MAX_LEN_ERROR-1, 
            "couldn't establish comms with vl53l5cx at %hhx (ndx %u)", 
            config->platform.address,
            device_ndx
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
    VL53L5CX_Configuration* platform,
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
    printf("comms is it alive called\n");

    napi_value this_;
    size_t argc;
    void* data;
    napi_status status;
    uint8_t is_alive = 0;
    napi_value argv[1] = {NULL};

    status = napi_get_cb_info(env, info, &argc, NULL, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_is_alive"
        );
        return NULL;
    }
    if (argc != 1) {
        napi_throw_error(
            env, 
            "expected config index", 
            "Must give device index (0-9) as argument."
        );
        return NULL;
    }
    uint32_t device_ndx = 0;
    napi_get_value_uint32(env, argv[0], &device_ndx);

    VL53L5CX_Configuration* conf = 
        ((VL53L5CX_Configuration*) data) + device_ndx;
    

    // Call the driver's relevant function.
    uint8_t drv_status = vl53l5cx_is_alive(conf, &is_alive);

	if(!is_alive || drv_status) {
        char err[MAX_LEN_ERROR] = {0};
        snprintf(
            err,
            MAX_LEN_ERROR-1, 
            "VL53L5CX not detected at address 0x%hhx",
            conf->platform.address
        );
        napi_throw_error(
            env,
            "no such sensor",
            err
        );
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
    size_t argc = 1;
    void* data;
    napi_status status;
    napi_value argv[1] = {NULL};

    status = napi_get_cb_info(env, info, &argc, argv, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_start_ranging"
        );
        return NULL;
    }
    napi_valuetype type;
    status = napi_typeof(env, argv[0], &type);
    if (status != napi_ok || type != napi_number || argc != 1) {
        napi_throw_error(
            env,
            "napi argument error",
            "Expected exactly one number argument 0<=a<10."
        );
    }
    uint32_t device_ndx = 0;
    status = napi_get_value_uint32(env, argv[0], &device_ndx);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "expected config index", 
            "Must give device index (0-9) as argument. fn: cb_vl53l5cx_start_ranging"
        );
        return NULL;
    }
    VL53L5CX_Configuration* conf = 
        ((VL53L5CX_Configuration*) data) + device_ndx;
    
    vl53l5cx_start_ranging(conf);
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
 * vl53l5cx_check_data_ready
 */

 napi_value cb_vl53l5cx_check_data_ready(
    napi_env env,
    napi_callback_info info
) {
    size_t argc = 1;
    napi_value this_;
    void* data;
    napi_status status;
    napi_value ret_val;
    napi_value argv[1] = {NULL};

    status = napi_get_cb_info(env, info, &argc, argv, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_check_data_ready"
        );
        return NULL;
    }

    if (argc != 1) {
        napi_throw_error(
            env, 
            "expected config index", 
            "Must give device index (0-9) as argument. fn: napi_get_cb_info"
        );
        return NULL;
    }

    uint32_t device_ndx = 0;
    napi_get_value_uint32(env, argv[0], &device_ndx);
    VL53L5CX_Configuration* conf = 
        ((VL53L5CX_Configuration*) data) + device_ndx;

    uint8_t is_ready = 0;
    vl53l5cx_check_data_ready(conf, &is_ready);
    is_ready
        ? napi_get_boolean(env, true, &ret_val)
        : napi_get_boolean(env, false, &ret_val);

    return ret_val;
 }

 void register_vl53l5cx_check_data_ready(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
 ) {
    napi_value fn;
    napi_status status;
    // In the driver the chip code is in capital letters.
    const char* name = "vl53l5cx_check_data_ready";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_vl53l5cx_check_data_ready,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            MODULE_INIT_ERROR, 
            "Could not create JS func vl53l5cx_check_data_ready"
        );
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
            "Could not bind JS func vl53l5cx_check_data_ready to the module"
        );
    }
}

/**
 *  Stop ranging
 */

napi_value cb_vl53l5cx_stop_ranging(napi_env env, napi_callback_info info) {
    napi_value this_;
    size_t argc = 1;
    void* data;
    napi_status status;
    uint8_t drv_status = 0;
    napi_value argv[1] = {NULL};

    status = napi_get_cb_info(env, info, &argc, NULL, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_stop_ranging"
        );
        return NULL;
    }
    if (argc != 1) {
        napi_throw_error(
            env, 
            "expected config index", 
            "Must give device index (0-9) as argument."
        );
        return NULL;
    }
    uint32_t device_ndx = 0;
    napi_get_value_uint32(env, argv[0], &device_ndx);
    VL53L5CX_Configuration* conf = 
        ((VL53L5CX_Configuration*) data) + device_ndx;
    

    vl53l5cx_stop_ranging(conf);
    return NULL;
}
void register_vl53l5cx_stop_ranging(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
) {
    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_stop_ranging";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_vl53l5cx_stop_ranging,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            MODULE_INIT_ERROR, 
            "Could not create JS func vl53l5cx_stop_ranging"
        );
        return;
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
            "Could not bind JS func vl53l5cx_stop_ranging to the module"
        );
    }
}


/**
 * Get ranging data
 */
napi_value cb_vl53l5cx_get_ranging_data(
    napi_env env,
    napi_callback_info info
) {
    napi_value this_;
    size_t argc = 1;
    void* data;
    napi_status status;
    napi_value argv[1] = {NULL};

    status = napi_get_cb_info(env, info, &argc, argv, &this_, &data);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            "napi_get_cb_info error", 
            "Happened in cb_vl53l5cx_get_ranging_data"
        );
        return NULL;
    }
    if (argc != 1) {
        napi_throw_error(
            env, 
            "expected config index", 
            "Must give device index (0-9) as argument."
        );
        return NULL;
    }

    uint32_t device_ndx = 0;
    napi_get_value_uint32(env, argv[0], &device_ndx);
    VL53L5CX_Configuration* config = 
        ((VL53L5CX_Configuration*) data) + device_ndx;
    

    /* Returned object */
    VL53L5CX_ResultsData results = {0};
    vl53l5cx_get_ranging_data(config, &results);
    napi_value ret_results = NULL;
    napi_create_object(env, &ret_results);

    /* siliconTempC prop */
    napi_value silicon_temp_c = NULL;
    status = napi_create_int32(
        env, 
        (int32_t)results.silicon_temp_degc, 
        &silicon_temp_c
    );
    if (status != napi_ok) {
        napi_throw_error(
            env,
            "couldn't create napi int32",
            "fn: cb_vl53l5cx_get_ranging_data"
        );
        return NULL;
    }
    napi_set_named_property(env, ret_results, "chipTempC", silicon_temp_c);

    /* Scan zones */
    napi_value scan_zones = NULL;
    /* max. 64 zones */
    status = napi_create_array_with_length(env, 64, &scan_zones);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            "couldn't create napi array",
            "fn: cb_vl53l5cx_get_ranging_data"
        );
        return NULL;
    }

    /* Populate scan zones array */
    for (uint32_t i = 0; i < 16; i++) {  // TODO: Accommodate 8x8 scan mode.
        /* Create the zone */
        napi_value zone = NULL;
        status = napi_create_object(env, &zone);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                "couldn't create napi object",
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }

        /* Zone index */
        napi_value zone_index = NULL;
        status = napi_create_uint32(env, i, &zone_index);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                "couldn't create napi object",
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
        napi_set_named_property(env, zone, "zoneIndex", zone_index);
                
        
        /* Measured distance */
        #ifndef VL53L5CX_DISABLE_DISTANCE_MM
        napi_value distance_mm = NULL;
        status = napi_create_int32(
            env, 
            results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*i],
            &distance_mm);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                "couldn't create napi int32",
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
        status = napi_set_named_property(
            env, 
            zone, 
            "distanceMillimeters", 
            distance_mm
        );
        if (status != napi_ok) {
            napi_throw_error(
                env,
                "couldn't napi set named property",
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
        #endif

        /* Target status */
        #ifndef VL53L5CX_DISABLE_TARGET_STATUS
        napi_value target_status = NULL;
        status = napi_create_uint32(
            env,
            results.target_status[VL53L5CX_NB_TARGET_PER_ZONE*i],
            &target_status
        );
        if (status != napi_ok) {
            napi_throw_error(
                env,
                "couldn't create napi uint32",
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
        status = napi_set_named_property(env, zone, "status", target_status);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                "couldn't napi set named property",
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
        #endif

        status = napi_set_element(env, scan_zones, i, zone);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                "couldn't set napi array index value",
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
    }

    /* Assign zones array to the results */
    napi_set_named_property(env, ret_results, "scanZones", scan_zones);
    return ret_results;
}
void register_vl53l5cx_get_ranging_data(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
) {
    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_get_ranging_data";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_vl53l5cx_get_ranging_data,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            MODULE_INIT_ERROR, 
            "Could not create JS func vl53l5cx_get_ranging_data"
        );
        return;
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
            "Could not bind JS func vl53l5cx_get_ranging_data to the module"
        );
    }

}