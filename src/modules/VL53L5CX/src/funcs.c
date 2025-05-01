#include <stdio.h>

#include <node_api.h>
#include <vl53l5cx_api.h>

#include "funcs.h"

/* Maximum length of formatted error message. */
#define MAX_LEN_ERROR 80

/* Maximum number of arguments the binding function can take from Node */
#define MAX_ARGUMENTS 10

/**
 * Auxialiry func to parse napi_callback arguments from Node
 * 
 * @return success
 */
bool parse_args(
    napi_env env,
    napi_callback_info info,
    size_t* argc,
    napi_value *argv,
    napi_value* this,
    void** data,
    size_t argc_min,
    size_t argc_max
) {
    napi_status status = napi_get_cb_info(env, info, argc, argv, this, data);
    switch (status)
    {
    case napi_ok:
        break;

    case napi_invalid_arg:
        napi_throw_error(env, ARGUMENT_ERROR, "Invalid argument");
        return false;

    case napi_object_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Object expected");
        return false;

    case napi_string_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "String expected");
    return false;

    case napi_name_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Name expected");
        return false;

    case napi_function_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Function expected");
        return false;

    case napi_number_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Number expected");
        return false;

    case napi_boolean_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Boolean expected");
        return false;

    case napi_array_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Array expected");
        return false;

    case napi_generic_failure:
        napi_throw_error(env, ARGUMENT_ERROR, "Generic failure");
        return false;

    case napi_pending_exception:
        napi_throw_error(env, ARGUMENT_ERROR, "Pending exeption");
        return false;

    case napi_cancelled:
        napi_throw_error(env, ARGUMENT_ERROR, "Cancelled");
        return false;

    case napi_escape_called_twice:
        napi_throw_error(env, ARGUMENT_ERROR, "Escape called twice");
        return false;

    case napi_handle_scope_mismatch:
        napi_throw_error(env, ARGUMENT_ERROR, "Scope mismatch");
        return false;

    case napi_callback_scope_mismatch:
        napi_throw_error(env, ARGUMENT_ERROR, "Callback scope mismatch");
        return false;

    case napi_queue_full:
        napi_throw_error(env, ARGUMENT_ERROR, "Napi queue full");
        return false;

    case napi_closing:
        napi_throw_error(env, ARGUMENT_ERROR, "Closing");
        return false;

    case napi_bigint_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Bigint expected");
        return false;

    case napi_date_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Date expected");
        return false;

    case napi_arraybuffer_expected:
        napi_throw_error(env, ARGUMENT_ERROR, "Arraybuffer expected");
        return false;

    case napi_detachable_arraybuffer_expected:
        napi_throw_error(
            env, ARGUMENT_ERROR, "Detatchable arraybuffer expected"
        );
        return false;

    case napi_would_deadlock:  /* unused */
    case napi_no_external_buffers_allowed:
        napi_throw_error(env, ARGUMENT_ERROR, "No external buffers allowed");
        return false;

    case napi_cannot_run_js:
        napi_throw_error(env, ARGUMENT_ERROR, "=D");
        return false;

    default:
        napi_throw_error(
            env, UNKNOWN_ERROR, "A new, unexpected error happened."
        );
        return false;
    }

    if (*argc < argc_min || *argc > argc_max) {
        napi_throw_error(
            env, ARGUMENT_ERROR, "Too many or too few arguments"
        );
        return false;
    }
    return true;
}

/************
 * Comms init
 */

/// @brief Initialize comms. This method must be called exactly once for device.
/// @param env Node environment
/// @param info Device addr as optional arg
/// @return nothing
napi_value cb_vl53l5cx_comms_init(napi_env env, napi_callback_info info) {
    printf("comms init called\n");
    napi_value argv[MAX_ARGUMENTS] = {NULL};
    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void* data = NULL;

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 1, 1  // Take exactly 1 argument
    );
    if (!success) {
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
            MODULE_INIT_ERROR, 
            "Could not create JS func cb_vl53l5cx_comms_init"
        );
        return; // don't have anything to bind
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
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

    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void* data;
    uint8_t is_alive = 0;
    napi_value argv[MAX_ARGUMENTS] = {NULL};

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 1, 1
    );
    if (!success) {
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
            NO_SENSOR,
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
        return;
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
    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void* data;
    napi_status status;
    napi_value argv[MAX_ARGUMENTS] = {NULL};

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 1, 1
    );
    if (!success) {
        return NULL;
    }

    uint32_t device_ndx = 0;
    status = napi_get_value_uint32(env, argv[0], &device_ndx);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            ARGUMENT_ERROR, 
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
        return;
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
    size_t argc = MAX_ARGUMENTS;
    napi_value this;
    void* data;
    napi_value ret_val;
    napi_value argv[MAX_ARGUMENTS] = {NULL};

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 1, 1
    );
    if (!success) {
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
        return;
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
    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void* data;
    napi_value argv[MAX_ARGUMENTS] = {NULL};

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 1, 1
    );
    if (!success) {
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
    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void* data;
    napi_status status;
    napi_value argv[MAX_ARGUMENTS] = {NULL};

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 1, 1
    );
    if (!success) {
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
            VALUE_NAPI_ERROR,
            "Couldn't create an int32. fn: cb_vl53l5cx_get_ranging_data"
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
            VALUE_NAPI_ERROR,
            "Couldn't create an array. fn: cb_vl53l5cx_get_ranging_data"
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
                VALUE_NAPI_ERROR,
                "Couldn't create napi object. fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }

        /* Zone index */
        napi_value zone_index = NULL;
        status = napi_create_uint32(env, i, &zone_index);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                VALUE_NAPI_ERROR,
                "Couldn't create napi object. fn: cb_vl53l5cx_get_ranging_data"
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
                VALUE_NAPI_ERROR,
                "Couldn't create napi int32. fn: cb_vl53l5cx_get_ranging_data"
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
                NAMED_PROPERTY_NOT_SET,
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
                VALUE_NAPI_ERROR,
                "Couldn't create napi uint32. fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
        status = napi_set_named_property(env, zone, "status", target_status);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                NAMED_PROPERTY_NOT_SET,
                "fn: cb_vl53l5cx_get_ranging_data"
            );
            return NULL;
        }
        #endif

        status = napi_set_element(env, scan_zones, i, zone);
        if (status != napi_ok) {
            napi_throw_error(
                env,
                ARRAY_VALUE_NOT_SET,
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

napi_value cb_vl53l5cx_set_resolution(napi_env env, napi_callback_info info) {
    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void* data;
    napi_status status;
    napi_value argv[MAX_ARGUMENTS] = {NULL};

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 2, 2
    );
    if (!success) {
        return NULL;
    }

    uint32_t cfg_slot;
    uint32_t resolution;

    status = napi_get_value_uint32(env, argv[0], &cfg_slot);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            ARGUMENT_ERROR, 
            "Invalid first argument for ..set_resolution(cfg, resolution)."
        );
        return NULL;
    }
    status = napi_get_value_uint32(env, argv[1], &resolution);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            ARGUMENT_ERROR, 
            "Invalid second argument for ..set_resolution(cfg, resolution)."
        );
        return NULL;
    }

    VL53L5CX_Configuration* conf = 
        ((VL53L5CX_Configuration*) data) + cfg_slot;

    if (resolution != VL53L5CX_RESOLUTION_8X8
        && resolution != VL53L5CX_RESOLUTION_4X4) {
        napi_throw_error(
            env,
            ARGUMENT_ERROR,
            "Resolution must be one of VL53L5CX_RESOLUTION_*."
        );
        return NULL;
    }

    uint8_t res_status = vl53l5cx_set_resolution(conf, (uint8_t) resolution);
    if (res_status) {
        napi_throw_error(
            env,
            ERROR_CHANGING_SETTING,
            "Couldn't change sensor resolution."
        );
    }
    return NULL;
}
void register_vl53l5cx_set_resolution(
    VL53L5CX_Configuration* conf, 
    napi_env env, 
    napi_value exports
) {
    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_set_resolution";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_vl53l5cx_set_resolution,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            MODULE_INIT_ERROR, 
            "Could not create JS func vl53l5cx_set_resolution"
        );
        return;
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
            "Could not bind JS func vl53l5cx_set_resolution to the module"
        );
    }
}

napi_value cb_vl53l5cx_set_ranging_frequency_hz(
    napi_env env, 
    napi_callback_info info
) {
    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void* data;
    napi_status status;
    napi_value argv[MAX_ARGUMENTS] = {NULL};

    bool success = parse_args(
        env, info, &argc, argv, &this, &data, 2, 2
    );
    if (!success) {
        return NULL;
    }

    uint32_t cfg_slot;
    uint32_t frequency;

    status = napi_get_value_uint32(env, argv[0], &cfg_slot);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            ARGUMENT_ERROR, 
            "Invalid first argument for ..set_frequency_hz(cfg, resolution)."
        );
        return NULL;
    }
    status = napi_get_value_uint32(env, argv[1], &frequency);
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            ARGUMENT_ERROR, 
            "Invalid second argument for ..set_frequency_hz(cfg, resolution)."
        );
        return NULL;
    }

    VL53L5CX_Configuration* conf = 
        ((VL53L5CX_Configuration*) data) + cfg_slot;

    // Need resolution to check the input frequency is sane.
    uint8_t resolution = 0;
    vl53l5cx_get_resolution(conf, &resolution);
    if (((VL53L5CX_RESOLUTION_8X8 == (uint8_t) resolution)
        && (frequency < 1 || frequency > 15))
        ||
        ((VL53L5CX_RESOLUTION_4X4 == (uint8_t) resolution)
        && (frequency < 1 || frequency > 60))) {
        napi_throw_error(
            env,
            ARGUMENT_ERROR,
            "Impossible frequency for the given resolution."
        );
        return NULL;
    }

    uint8_t res_status = vl53l5cx_set_ranging_frequency_hz(
        conf, (uint8_t) frequency);
    if (res_status) {
        napi_throw_error(
            env,
            ERROR_CHANGING_SETTING,
            "Couldn't change sensing frequency."
        );
    }
    return NULL;
}
void register_vl53l5cx_set_ranging_frequency_hz(
    VL53L5CX_Configuration* conf,
    napi_env env,
    napi_value exports
) {
    napi_value fn;
    napi_status status;
    const char* name = "vl53l5cx_set_ranging_frequency_hz";

    status = napi_create_function(
        env,
        name, 
        strlen(name),
        cb_vl53l5cx_set_ranging_frequency_hz,
        conf, 
        &fn
    );
    if (status != napi_ok) {
        napi_throw_error(
            env, 
            MODULE_INIT_ERROR, 
            "Could not create JS func vl53l5cx_set_ranging_frequency_hz"
        );
        return;
    }

    status = napi_set_named_property(env, exports, name, fn);
    if (status != napi_ok) {
        napi_throw_error(
            env,
            MODULE_INIT_ERROR,
        "Could not bind JS func vl53l5cx_set_ranging_frequency_hz to the module"
        );
    }
}
