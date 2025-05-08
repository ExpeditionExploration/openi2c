#include "error.h"
#include "node_c_type_conversions.h"
#include "sh2/sh2_hal.h"
#include "sh2_hal_supplement.h"
#include <node/node_api.h>
#include <stdint.h>
#include <stdlib.h>

// Max arguments a function can take
#define MAX_ARGUMENTS 10

/**
 * Auxialiry func to parse napi_callback arguments from Node
 *
 * @return success
 */
bool parse_args(napi_env env, napi_callback_info info, size_t* argc,
                napi_value* argv, napi_value* this, void** data,
                size_t argc_min, size_t argc_max) {
    napi_status status = napi_get_cb_info(env, info, argc, argv, this, data);
    switch (status) {
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
        napi_throw_error(env, ARGUMENT_ERROR,
                         "Detatchable arraybuffer expected");
        return false;

    case napi_would_deadlock: /* unused */
    case napi_no_external_buffers_allowed:
        napi_throw_error(env, ARGUMENT_ERROR, "No external buffers allowed");
        return false;

    case napi_cannot_run_js:
        napi_throw_error(env, ARGUMENT_ERROR, "=D");
        return false;

    default:
        napi_throw_error(env, UNKNOWN_ERROR,
                         "A new, unexpected error happened.");
        return false;
    }

    if (*argc < argc_min || *argc > argc_max) {
        napi_throw_error(env, ARGUMENT_ERROR, "Too many or too few arguments");
        return false;
    }
    return true;
}

// This function creates struct with settings to communicated with a SB2 device.
//
// args:
//  - bus_number: uint8_t
//  - addr: uint8_t
napi_value cb_setI2CSettings(napi_env env, napi_callback_info info) {
    napi_value argv[MAX_ARGUMENTS] = {NULL};
    napi_value this;
    size_t argc = MAX_ARGUMENTS;
    void*  data = NULL;

    bool success = parse_args(env, info, &argc, argv, &this, &data, 2, 2);
    if (!success) {
        return NULL;
    }

    napi_status status;
    uint32_t    bus, addr;

    i2c_settings_t* settings = malloc(sizeof(i2c_settings_t));

    status = napi_get_value_uint32(env, argv[0], &bus);
    status |= napi_get_value_uint32(env, argv[1], &addr);

    if (status != napi_ok || bus > 0xFF || addr > 0xFF) {
        napi_throw_error(env, ARGUMENT_ERROR,
                         "Probably argument out of range. Settings not set. "
                         "fn: cb_setI2CSettings");
        return NULL;
    }

    i2c_settings_t i2c_settings = {.bus = bus, .addr = addr};
    set_i2c_settings(&i2c_settings);

    return NULL;
}

// This function gets current I2C settings and returns them.
//
// If no settings have been saved, the contents should be zeroes.
//
// args: None
napi_value cb_getI2CSettings(napi_env env, napi_callback_info _) {
    napi_status status;

    i2c_settings_t settings = get_i2c_settings();

    napi_value bus, addr;
    status = napi_create_uint32(env, settings.bus, &bus);
    status |= napi_create_uint32(env, settings.addr, &addr);
    if (status != napi_ok) {
        napi_throw_error(env, ERROR_TRANSLATING_STRUCT_TO_NODE,
                         "Couldn't construct I2C settings");
    }
    return NULL;
}
