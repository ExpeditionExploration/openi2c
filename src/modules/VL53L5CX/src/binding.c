#include <string.h>
#include <node_api.h>
#include <vl53l5cx_api.h>

#include "constants.h"


/**
 * Assign pre-processor constants from vl53l5cx_api.h to actual constants
 * to have them have addresses. See vl53l5cx_api.h for explanations on what
 * is what.
 */

const char* _API_REVISION = VL53L5CX_API_REVISION;
const uint16_t _DEFAULT_I2C_ADDRESS = VL53L5CX_DEFAULT_I2C_ADDRESS;
const uint8_t _RESOLUTION_4X4 = VL53L5CX_RESOLUTION_4X4;
const uint8_t _RESOLUTION_8X8 = VL53L5CX_RESOLUTION_8X8;
const uint8_t _TARGET_ORDER_CLOSEST = VL53L5CX_TARGET_ORDER_CLOSEST;
const uint8_t _TARGET_ORDER_STRONGEST = VL53L5CX_TARGET_ORDER_STRONGEST;
const uint8_t _RANGING_MODE_AUTONOMOUS = VL53L5CX_RANGING_MODE_AUTONOMOUS;
const uint8_t _RANGING_MODE_CONTINUOUS = VL53L5CX_RANGING_MODE_CONTINUOUS;
const uint8_t _POWER_MODE_SLEEP = VL53L5CX_POWER_MODE_SLEEP;
const uint8_t _POWER_MODE_WAKEUP = VL53L5CX_POWER_MODE_WAKEUP;
const uint8_t _OK = VL53L5CX_STATUS_OK;
const uint8_t _TIMEOUT_ERROR = VL53L5CX_STATUS_TIMEOUT_ERROR;
const uint8_t _CORRUPTED_FRAME = VL53L5CX_STATUS_CORRUPTED_FRAME;
const uint8_t _CRC_CSUM_FAILED = VL53L5CX_STATUS_CRC_CSUM_FAILED;
const uint8_t _XTALK_FAILED = VL53L5CX_STATUS_XTALK_FAILED;
const uint8_t _MCU_ERROR = VL53L5CX_MCU_ERROR;
const uint8_t _STATUS_INVALID_PARAM = VL53L5CX_STATUS_INVALID_PARAM;
const uint8_t _STATUS_ERROR = VL53L5CX_STATUS_ERROR;


napi_value init(napi_env env, napi_value exports) {

    /**
     * Constants from vl53l5cx_api.h
     */

    register_constant(
        env, 
        exports, 
        "VL53L5CX_API_REVISION", 
        CT_UTF_8_STRING,
        _API_REVISION
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_DEFAULT_I2C_ADDRESS", 
        CT_UINT8_INTEGER,
        &_DEFAULT_I2C_ADDRESS
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_RESOLUTION_4X4", 
        CT_UINT8_INTEGER,
        &_RESOLUTION_4X4
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_RESOLUTION_8X8", 
        CT_UINT8_INTEGER,
        &_RESOLUTION_8X8
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_TARGET_ORDER_CLOSEST", 
        CT_UINT8_INTEGER,
        &_TARGET_ORDER_CLOSEST
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_TARGET_ORDER_STRONGEST", 
        CT_UINT8_INTEGER,
        &_TARGET_ORDER_STRONGEST
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_RANGING_MODE_CONTINUOUS", 
        CT_UINT8_INTEGER,
        &_RANGING_MODE_CONTINUOUS
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_RANGING_MODE_AUTONOMOUS", 
        CT_UINT8_INTEGER,
        &_RANGING_MODE_AUTONOMOUS
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_POWER_MODE_SLEEP", 
        CT_UINT8_INTEGER,
        &_POWER_MODE_SLEEP
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_POWER_MODE_WAKEUP", 
        CT_UINT8_INTEGER,
        &_POWER_MODE_WAKEUP
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_STATUS_OK", 
        CT_UINT8_INTEGER,
        &_OK
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_STATUS_TIMEOUT_ERROR", 
        CT_UINT8_INTEGER,
        &_TIMEOUT_ERROR
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_STATUS_CORRUPTED_FRAME", 
        CT_UINT8_INTEGER,
        &_CORRUPTED_FRAME
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_STATUS_CRC_CSUM_FAILED", 
        CT_UINT8_INTEGER,
        &_CRC_CSUM_FAILED
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_STATUS_XTALK_FAILED", 
        CT_UINT8_INTEGER,
        &_XTALK_FAILED
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_MCU_ERROR", 
        CT_UINT8_INTEGER,
        &_MCU_ERROR
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_STATUS_INVALID_PARAM", 
        CT_UINT8_INTEGER,
        &_STATUS_INVALID_PARAM
    );

    register_constant(
        env, 
        exports, 
        "VL53L5CX_STATUS_ERROR", 
        CT_UINT8_INTEGER,
        &_STATUS_ERROR
    );

    return exports;
}

NAPI_MODULE(vl53l5cx_native, init)
