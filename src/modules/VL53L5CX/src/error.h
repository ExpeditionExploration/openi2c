#ifndef __ERROR_H
#define __ERROR_H

#include <node_api.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/* Maximum error msg length. */
static const size_t __MAX_ERROR_MSG_LEN = 81;


/**
 * Status codes for abrupt exits.
 * 
 * It's bad to abruptly exit, but Node going UB is even worse. These make it
 * easier to debug and improve error handling.
 */

 extern const uint8_t FAILED_TO_THROW_ERROR;
 extern const uint8_t UNDEFINED_ERROR;
 extern const uint8_t IMPLEMENTATION_MISSING;
 

/**
 * Return NULL on napi_ok.
 * 
 * Otherwise return exception object.
 */
void handle_status_codes(
    napi_env env,
    napi_status status
);

#endif