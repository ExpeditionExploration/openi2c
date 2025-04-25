
#include <node_api.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"


const uint8_t FAILED_TO_THROW_ERROR = 150;
const uint8_t UNDEFINED_ERROR = 151;
const uint8_t IMPLEMENTATION_MISSING = 152;


static void auxialiry_throw_func(napi_env env) {
    napi_status throw_status = napi_throw_error(
        env, 
        NULL,
        NULL
    );
    if (throw_status != napi_ok) {
        exit(FAILED_TO_THROW_ERROR);
    }
}


/**
 * Throw an error in case status is not napi_ok.
 */
void handle_status_codes(
    napi_env env,
    napi_status status
) {
    switch (status)
    {
    case napi_ok:
        break;
    
    case napi_invalid_arg:
        auxialiry_throw_func(env);
        break;
    
    case napi_object_expected:
        auxialiry_throw_func(env);
        break;

    case napi_string_expected:
        auxialiry_throw_func(env);
        break;

    case napi_name_expected:
        auxialiry_throw_func(env);
        break;

    case napi_function_expected:
        auxialiry_throw_func(env);
        break;

    case napi_number_expected:
        auxialiry_throw_func(env);
        break;

    case napi_boolean_expected:
        auxialiry_throw_func(env);
        break;

    case napi_array_expected:
        auxialiry_throw_func(env);
        break;

    case napi_generic_failure:
        auxialiry_throw_func(env);
        break;

    case napi_pending_exception:
        auxialiry_throw_func(env);
        break;

    case napi_cancelled:
        auxialiry_throw_func(env);
        break;

    case napi_escape_called_twice:
        auxialiry_throw_func(env);
        break;

    case napi_handle_scope_mismatch:
        auxialiry_throw_func(env);
        break;

    case napi_callback_scope_mismatch:
        auxialiry_throw_func(env);
        break;

    case napi_queue_full:
        auxialiry_throw_func(env);
        break;


    case napi_closing:
        auxialiry_throw_func(env);
        break;


    case napi_bigint_expected:
        auxialiry_throw_func(env);
        break;


    case napi_date_expected:
        auxialiry_throw_func(env);
        break;


    case napi_arraybuffer_expected:
        auxialiry_throw_func(env);
        break;


    case napi_detachable_arraybuffer_expected:
        auxialiry_throw_func(env);
        break;

    case napi_would_deadlock:
        auxialiry_throw_func(env);
        break;

    case napi_no_external_buffers_allowed:
        auxialiry_throw_func(env);
        break;

    case napi_cannot_run_js:
        auxialiry_throw_func(env);
        break;

    default:
        // TODO: Handle it in some other way instead of emergency exit.
        auxialiry_throw_func(env);
        exit(UNDEFINED_ERROR);
        break;
    }
}
