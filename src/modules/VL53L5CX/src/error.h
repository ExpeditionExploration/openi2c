#ifndef __ERROR_H
#define __ERROR_H

#include <node_api.h>
#include <stdint.h>

/**
 * Errors indicating a hardware problem.
 */

extern const char* INVALID_PARAM_ERROR;
extern const char* XTALK_ERROR;
extern const char* CORRUPTED_FRAME_ERROR;
extern const char* TIMEOUT_ERROR;
extern const char* CRC_CHECKSUM_ERROR;
extern const char* MCU_ERROR;
extern const char* ERROR;

extern const char* INIT_COMMS_ERROR;

/**
 * Node-API side of errors.
 */
extern const char* MODULE_INIT_ERROR;
extern const char* MODULE_FUNC_ARGUMENT_ERROR;
extern const char* MODULE_FUNC_INFO_ERROR;

#endif