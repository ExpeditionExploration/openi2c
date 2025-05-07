#include <node/node_api.h>
#include "error.h"

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
