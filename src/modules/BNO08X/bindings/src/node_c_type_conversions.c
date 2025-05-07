
#include "error.h"
#include "sh2/sh2.h"
#include <node/node_api.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

napi_value mkNapiSensorEvent(napi_env env, sh2_SensorEvent_t* ev) {
    napi_status status;
    napi_value  obj; // JS object
    status = napi_create_object(env, &obj);

    uint8_t* buf = (uint8_t*)malloc(ev->len);
    memcpy((void*)buf, (const void*)&ev->report, ev->len);

    napi_value timestamp_uS; // uint64_t -> number
    napi_value delay_uS;     // int64_t -> number
    napi_value len;          // uint8_t -> number
    napi_value reportId;     // uint8_t -> number
    napi_value report;       // uint8_t -> ArrayBuffer
    status |= napi_create_bigint_uint64(env, ev->timestamp_uS, &timestamp_uS);
    status |= napi_create_int64(env, ev->delay_uS, &delay_uS);
    status |= napi_create_uint32(env, ev->len, &len);
    status |= napi_create_uint32(env, ev->reportId, &reportId);
    status |= napi_create_external_arraybuffer(env, buf, ev->len, NULL, NULL,
                                               &report);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to create NAPI value");
        return NULL;
    }

    status = napi_set_named_property(env, obj, "timestampMicroseconds",
                                     timestamp_uS);
    status |= napi_set_named_property(env, obj, "delayMicroseconds", delay_uS);
    status |= napi_set_named_property(env, obj, "length", len);
    status |= napi_set_named_property(env, obj, "reportId", reportId);
    status |= napi_set_named_property(env, obj, "report", report);
    if (status != napi_ok) {
        napi_throw_error(env, ERROR_TRANSLATING_STRUCT_TO_NODE,
                         "Failed to set NAPI property");
        return NULL;
    }
    return obj;
}

napi_value mkProductId(napi_env env, sh2_ProductId_t* id) {
    napi_status status;
    napi_value  obj; // JS object
    status = napi_create_object(env, &obj);

    napi_value resetCause;     // uint8_t -> number
    napi_value swVersionMajor; // uint8_t -> number
    napi_value swVersionMinor; // uint8_t -> number
    napi_value swPartNumber;   // uint32_t -> number
    napi_value swBuildNumber;  // uint32_t -> number
    napi_value swVersionPatch; // uint16_t -> number
    napi_value reserved0;      // uint8_t -> number
    napi_value reserved1;      // uint8_t -> number

    status |= napi_create_uint32(env, id->resetCause, &resetCause);
    status |= napi_create_uint32(env, id->swVersionMajor, &swVersionMajor);
    status |= napi_create_uint32(env, id->swVersionMinor, &swVersionMinor);
    status |= napi_create_uint32(env, id->swPartNumber, &swPartNumber);
    status |= napi_create_uint32(env, id->swBuildNumber, &swBuildNumber);
    status |= napi_create_uint32(env, id->swVersionPatch, &swVersionPatch);
    status |= napi_create_uint32(env, id->reserved0, &reserved0);
    status |= napi_create_uint32(env, id->reserved1, &reserved1);

    if (status != napi_ok) {
        napi_throw_error(env, ERROR_TRANSLATING_STRUCT_TO_NODE,
                         "Couldn't construct a ProductId");
    }

    return obj;
}

napi_value mkSensorIdEnum(napi_env env) {
    napi_status status;
    napi_value  obj;
    status = napi_create_object(env, &obj);

    napi_value raw_accelerometer;
    napi_value accelerometer;
    napi_value linear_acceleration;
    napi_value gravity;
    napi_value raw_gyroscope;
    napi_value gyroscope_calibrated;
    napi_value gyroscope_uncalibrated;
    napi_value raw_magnetometer;
    napi_value magnetic_field_calibrated;
    napi_value magnetic_field_uncalibrated;
    napi_value rotation_vector;
    napi_value game_rotation_vector;
    napi_value geomagnetic_rotation_vector;
    napi_value pressure;
    napi_value ambient_light;
    napi_value humidity;
    napi_value proximity;
    napi_value temperature;
    napi_value reserved;
    napi_value tap_detector;
    napi_value step_detector;
    napi_value step_counter;
    napi_value significant_motion;
    napi_value stability_classifier;
    napi_value shake_detector;
    napi_value flip_detector;
    napi_value pickup_detector;
    napi_value stability_detector;
    napi_value personal_activity_classifier;
    napi_value sleep_detector;
    napi_value tilt_detector;
    napi_value pocket_detector;
    napi_value circle_detector;
    napi_value heart_rate_monitor;
    napi_value arvr_stabilized_rv;
    napi_value arvr_stabilized_grv;
    napi_value gyro_integrated_rv;
    napi_value izro_motion_request;
    napi_value raw_optical_flow;
    napi_value dead_reckoning_pose;
    napi_value wheel_encoder;
    napi_value max_sensor_id;

    status |=
        napi_create_uint32(env, SH2_RAW_ACCELEROMETER, &raw_accelerometer);
    status |= napi_create_uint32(env, SH2_ACCELEROMETER, &accelerometer);
    status |=
        napi_create_uint32(env, SH2_LINEAR_ACCELERATION, &linear_acceleration);
    status |= napi_create_uint32(env, SH2_GRAVITY, &gravity);
    status |= napi_create_uint32(env, SH2_RAW_GYROSCOPE, &raw_gyroscope);
    status |= napi_create_uint32(env, SH2_GYROSCOPE_CALIBRATED,
                                 &gyroscope_calibrated);
    status |= napi_create_uint32(env, SH2_GYROSCOPE_UNCALIBRATED,
                                 &gyroscope_uncalibrated);
    status |= napi_create_uint32(env, SH2_RAW_MAGNETOMETER, &raw_magnetometer);
    status |= napi_create_uint32(env, SH2_MAGNETIC_FIELD_CALIBRATED,
                                 &magnetic_field_calibrated);
    status |= napi_create_uint32(env, SH2_MAGNETIC_FIELD_UNCALIBRATED,
                                 &magnetic_field_uncalibrated);
    status |= napi_create_uint32(env, SH2_ROTATION_VECTOR, &rotation_vector);
    status |= napi_create_uint32(env, SH2_GAME_ROTATION_VECTOR,
                                 &game_rotation_vector);
    status |= napi_create_uint32(env, SH2_GEOMAGNETIC_ROTATION_VECTOR,
                                 &geomagnetic_rotation_vector);
    status |= napi_create_uint32(env, SH2_PRESSURE, &pressure);
    status |= napi_create_uint32(env, SH2_AMBIENT_LIGHT, &ambient_light);
    status |= napi_create_uint32(env, SH2_HUMIDITY, &humidity);
    status |= napi_create_uint32(env, SH2_PROXIMITY, &proximity);
    status |= napi_create_uint32(env, SH2_TEMPERATURE, &temperature);
    status |= napi_create_uint32(env, SH2_RESERVED, &reserved);
    status |= napi_create_uint32(env, SH2_TAP_DETECTOR, &tap_detector);
    status |= napi_create_uint32(env, SH2_STEP_DETECTOR, &step_detector);
    status |= napi_create_uint32(env, SH2_STEP_COUNTER, &step_counter);
    status |=
        napi_create_uint32(env, SH2_SIGNIFICANT_MOTION, &significant_motion);
    status |= napi_create_uint32(env, SH2_STABILITY_CLASSIFIER,
                                 &stability_classifier);
    status |= napi_create_uint32(env, SH2_SHAKE_DETECTOR, &shake_detector);
    status |= napi_create_uint32(env, SH2_FLIP_DETECTOR, &flip_detector);
    status |= napi_create_uint32(env, SH2_PICKUP_DETECTOR, &pickup_detector);
    status |=
        napi_create_uint32(env, SH2_STABILITY_DETECTOR, &stability_detector);
    status |= napi_create_uint32(env, SH2_PERSONAL_ACTIVITY_CLASSIFIER,
                                 &personal_activity_classifier);
    status |= napi_create_uint32(env, SH2_SLEEP_DETECTOR, &sleep_detector);
    status |= napi_create_uint32(env, SH2_TILT_DETECTOR, &tilt_detector);
    status |= napi_create_uint32(env, SH2_POCKET_DETECTOR, &pocket_detector);
    status |= napi_create_uint32(env, SH2_CIRCLE_DETECTOR, &circle_detector);
    status |=
        napi_create_uint32(env, SH2_HEART_RATE_MONITOR, &heart_rate_monitor);
    status |=
        napi_create_uint32(env, SH2_ARVR_STABILIZED_RV, &arvr_stabilized_rv);
    status |=
        napi_create_uint32(env, SH2_ARVR_STABILIZED_GRV, &arvr_stabilized_grv);
    status |=
        napi_create_uint32(env, SH2_GYRO_INTEGRATED_RV, &gyro_integrated_rv);
    status |=
        napi_create_uint32(env, SH2_IZRO_MOTION_REQUEST, &izro_motion_request);
    status |= napi_create_uint32(env, SH2_RAW_OPTICAL_FLOW, &raw_optical_flow);
    status |=
        napi_create_uint32(env, SH2_DEAD_RECKONING_POSE, &dead_reckoning_pose);
    status |= napi_create_uint32(env, SH2_WHEEL_ENCODER, &wheel_encoder);
    status |= napi_create_uint32(env, SH2_MAX_SENSOR_ID, &max_sensor_id);

    status |= napi_set_named_property(env, obj, "RAW_ACCELEROMETER",
                                      raw_accelerometer);
    status |= napi_set_named_property(env, obj, "ACCELEROMETER", accelerometer);
    status |= napi_set_named_property(env, obj, "LINEAR_ACCELERATION",
                                      linear_acceleration);
    status |= napi_set_named_property(env, obj, "GRAVITY", gravity);
    status |= napi_set_named_property(env, obj, "RAW_GYROSCOPE", raw_gyroscope);
    status |= napi_set_named_property(env, obj, "MAGNETIC_FIELD_CALIBRATED",
                                      magnetic_field_calibrated);
    status |= napi_set_named_property(env, obj, "MAGNETIC_FIELD_UNCALIBRATED",
                                      magnetic_field_uncalibrated);
    status |=
        napi_set_named_property(env, obj, "ROTATION_VECTOR", rotation_vector);
    status |= napi_set_named_property(env, obj, "GAME_ROTATION_VECTOR",
                                      game_rotation_vector);
    status |= napi_set_named_property(env, obj, "GEOMAGNETIC_ROTATION_VECTOR",
                                      geomagnetic_rotation_vector);
    status |= napi_set_named_property(env, obj, "PRESSURE", pressure);
    status |= napi_set_named_property(env, obj, "AMBIENT_LIGHT", ambient_light);
    status |= napi_set_named_property(env, obj, "HUMIDITY", humidity);
    status |= napi_set_named_property(env, obj, "PROXIMITY", proximity);
    status |= napi_set_named_property(env, obj, "TEMPERATURE", temperature);
    status |= napi_set_named_property(env, obj, "RESERVED", reserved);
    status |= napi_set_named_property(env, obj, "TAP_DETECTOR", tap_detector);
    status |= napi_set_named_property(env, obj, "STEP_DETECTOR", step_detector);
    status |= napi_set_named_property(env, obj, "STEP_COUNTER", step_counter);
    status |= napi_set_named_property(env, obj, "SIGNIFICANT_MOTION",
                                      significant_motion);
    status |= napi_set_named_property(env, obj, "STABILITY_CLASSIFIER",
                                      stability_classifier);
    status |=
        napi_set_named_property(env, obj, "SHAKE_DETECTOR", shake_detector);
    status |= napi_set_named_property(env, obj, "FLIP_DETECTOR", flip_detector);
    status |=
        napi_set_named_property(env, obj, "PICKUP_DETECTOR", pickup_detector);
    status |= napi_set_named_property(env, obj, "STABILITY_DETECTOR",
                                      stability_detector);
    status |= napi_set_named_property(env, obj, "PERSONAL_ACTIVITY_CLASSIFIER",
                                      personal_activity_classifier);
    status |=
        napi_set_named_property(env, obj, "SLEEP_DETECTOR", sleep_detector);
    status |= napi_set_named_property(env, obj, "TILT_DETECTOR", tilt_detector);
    status |=
        napi_set_named_property(env, obj, "POCKET_DETECTOR", pocket_detector);
    status |=
        napi_set_named_property(env, obj, "CIRCLE_DETECTOR", circle_detector);
    status |= napi_set_named_property(env, obj, "HEART_RATE_MONITOR",
                                      heart_rate_monitor);
    status |= napi_set_named_property(env, obj, "ARVR_STABILIZED_RV",
                                      arvr_stabilized_rv);
    status |= napi_set_named_property(env, obj, "ARVR_STABILIZED_GRV",
                                      arvr_stabilized_grv);
    status |= napi_set_named_property(env, obj, "GYRO_INTEGRATED_RV",
                                      gyro_integrated_rv);
    status |= napi_set_named_property(env, obj, "IZRO_MOTION_REQUEST",
                                      izro_motion_request);
    status |=
        napi_set_named_property(env, obj, "RAW_OPTICAL_FLOW", raw_optical_flow);
    status |= napi_set_named_property(env, obj, "DEAD_RECKONING_POSE",
                                      dead_reckoning_pose);
    status |= napi_set_named_property(env, obj, "WHEEL_ENCODER", wheel_encoder);
    status |= napi_set_named_property(env, obj, "MAX_SENSOR_ID", max_sensor_id);

    if (status != napi_ok) {
        napi_throw_error(env, ERROR_TRANSLATING_STRUCT_TO_NODE,
                         "Could not create SensorId enum.");
        return NULL;
    }

    return obj;
}

napi_value mkSensorConfig(napi_env env, sh2_SensorConfig_t* cfg) {
    napi_status status;
    napi_value  obj;
    status = napi_create_object(env, &obj);

    napi_value changeSensitivityEnabled;
    napi_value changeSensitivityRelative;
    napi_value wakeupEnabled;
    napi_value alwaysOnEnabled;
    napi_value sniffEnabled;
    napi_value changeSensitivity;
    napi_value reportInterval_us;
    napi_value batchInterval_us;
    napi_value sensorSpecific;

    // Create fields
    status |= napi_get_boolean(env, cfg->changeSensitivityEnabled,
                               &changeSensitivityEnabled);
    status |= napi_get_boolean(env, cfg->changeSensitivityRelative,
                               &changeSensitivityRelative);
    status |= napi_get_boolean(env, cfg->wakeupEnabled, &wakeupEnabled);
    status |= napi_get_boolean(env, cfg->sniffEnabled, &sniffEnabled);
    status |= napi_get_boolean(env, cfg->alwaysOnEnabled, &alwaysOnEnabled);
    status |=
        napi_create_uint32(env, cfg->changeSensitivity, &changeSensitivity);
    status |=
        napi_create_uint32(env, cfg->reportInterval_us, &reportInterval_us);
    status |= napi_create_uint32(env, cfg->batchInterval_us, &batchInterval_us);
    status |= napi_create_uint32(env, cfg->sensorSpecific, &sensorSpecific);

    // Set named props
    status |= napi_set_named_property(env, obj, "changeSensitivityEnabled",
                                      changeSensitivityEnabled);
    status |= napi_set_named_property(env, obj, "changeSensitivityRelative",
                                      changeSensitivityRelative);
    status |= napi_set_named_property(env, obj, "wakeupEnabled", wakeupEnabled);
    status |=
        napi_set_named_property(env, obj, "alwaysOnEnabled", alwaysOnEnabled);
    status |= napi_set_named_property(env, obj, "sniffEnabled", sniffEnabled);
    status |= napi_set_named_property(env, obj, "changeSensitivity",
                                      changeSensitivity);
    status |= napi_set_named_property(env, obj, "reportInterval_us",
                                      reportInterval_us);
    status |=
        napi_set_named_property(env, obj, "batchInterval_us", batchInterval_us);
    status |=
        napi_set_named_property(env, obj, "sensorSpecific", sensorSpecific);

    if (status != napi_ok) {
        napi_throw_error(env, ERROR_TRANSLATING_STRUCT_TO_NODE,
                         "Couldn't construct a SensorConfig.");
        return NULL;
    }

    return obj;
}

napi_value mkSensorMetadata(napi_env env, sh2_SensorMetadata_t* meta) {
    napi_status status;
    napi_value  obj;
    status = napi_create_object(env, &obj);

    napi_value meVersion;        /**< @brief Motion Engine Version */
    napi_value mhVersion;        /**< @brief Motion Hub Version */
    napi_value shVersion;        /**< @brief SensorHub Version */
    napi_value range;            /**< @brief Same units as sensor reports */
    napi_value resolution;       /**< @brief Same units as sensor reports */
    napi_value revision;         /**< @brief Metadata record format revision */
    napi_value power_mA;         /**< @brief [mA] Fixed point 16Q10 format */
    napi_value minPeriod_uS;     /**< @brief [uS] */
    napi_value maxPeriod_uS;     /**< @brief [uS] */
    napi_value fifoReserved;     /**< @brief (Unused) */
    napi_value fifoMax;          /**< @brief (Unused) */
    napi_value batchBufferBytes; /**< @brief (Unused) */
    napi_value qPoint1;          /**< @brief q point for sensor values */
    napi_value qPoint2; /**< @brief q point for accuracy or bias fields */
    napi_value
        qPoint3; /**< @brief q point for sensor data change sensitivity */
    napi_value vendorIdLen;       /**< @brief [bytes] */
    napi_value vendorId;          /**< @brief Vendor name and part number */
    napi_value sensorSpecificLen; /**< @brief [bytes] */
    napi_value sensorSpecific;    /**< @brief See SH-2 Reference Manual */

    // Create napi values
    status |= napi_create_uint32(env, meta->mhVersion, &meVersion);
    status |= napi_create_uint32(env, meta->shVersion, &meVersion);
    status |= napi_create_uint32(env, meta->range, &range);
    status |= napi_create_uint32(env, meta->resolution, &resolution);
    status |= napi_create_uint32(env, meta->revision, &revision);
    status |= napi_create_uint32(env, meta->power_mA, &power_mA);
    status |= napi_create_uint32(env, meta->minPeriod_uS, &minPeriod_uS);
    status |= napi_create_uint32(env, meta->maxPeriod_uS, &maxPeriod_uS);
    status |= napi_create_uint32(env, meta->fifoReserved, &fifoReserved);
    status |= napi_create_uint32(env, meta->fifoMax, &fifoMax);
    status |=
        napi_create_uint32(env, meta->batchBufferBytes, &batchBufferBytes);
    status |= napi_create_uint32(env, meta->qPoint1, &qPoint1);
    status |= napi_create_uint32(env, meta->qPoint2, &qPoint2);
    status |= napi_create_uint32(env, meta->qPoint3, &qPoint3);
    status |= napi_create_uint32(env, meta->vendorIdLen, &vendorIdLen);
    status |=
        napi_create_uint32(env, meta->sensorSpecificLen, &sensorSpecificLen);
    status |= napi_create_string_utf8(env, meta->vendorId, meta->vendorIdLen,
                                      &vendorId);
    u_int8_t* buf = malloc(meta->sensorSpecificLen);
    memcpy(buf, &meta->sensorSpecific, meta->sensorSpecificLen);
    status |= napi_create_external_arraybuffer(env, &meta->sensorSpecific,
                                               meta->sensorSpecificLen, NULL,
                                               NULL, &sensorSpecific);

    // Set the objects props
    status |= napi_set_named_property(env, obj, "meVersion", meVersion);
    status |= napi_set_named_property(env, obj, "mhVersion", mhVersion);
    status |= napi_set_named_property(env, obj, "shVersion", shVersion);
    status |= napi_set_named_property(env, obj, "range", range);
    status |= napi_set_named_property(env, obj, "resolution", resolution);
    status |= napi_set_named_property(env, obj, "revision", revision);
    status |= napi_set_named_property(env, obj, "power_mA", power_mA);
    status |= napi_set_named_property(env, obj, "minPeriod_uS", minPeriod_uS);
    status |= napi_set_named_property(env, obj, "maxPeriod_uS", maxPeriod_uS);
    status |= napi_set_named_property(env, obj, "fifoReserved", fifoReserved);
    status |= napi_set_named_property(env, obj, "fifoMax", fifoMax);
    status |=
        napi_set_named_property(env, obj, "batchBufferBytes", batchBufferBytes);
    status |= napi_set_named_property(env, obj, "qPoint1", qPoint1);
    status |= napi_set_named_property(env, obj, "qPoint2", qPoint2);
    status |= napi_set_named_property(env, obj, "qPoint3", qPoint3);
    status |= napi_set_named_property(env, obj, "vendorIdLen", vendorIdLen);
    status |= napi_set_named_property(env, obj, "vendorId", vendorId);
    status |= napi_set_named_property(env, obj, "sensorSpecificLen",
                                      sensorSpecificLen);
    status |=
        napi_set_named_property(env, obj, "sensorSpecific", sensorSpecific);

    if (status != napi_ok) {
        napi_throw_error(env, ERROR_TRANSLATING_STRUCT_TO_NODE,
                         "Couldn't build SensorMetaData.");
        return NULL;
    }
    return obj;
}

napi_value mkErrorRecord(napi_env env, sh2_ErrorRecord_t* err) {
    napi_status status;
    napi_value  obj;
    status = napi_create_object(env, &obj);

    napi_value severity; /**< @brief Error severity, 0: most severe. */
    napi_value sequence; /**< @brief Sequence number (by severity) */
    napi_value source; /**< 1-MotionEngine, 2-MotionHub, 3-SensorHub, 4-Chip  */
    napi_value error;  /**< @brief See SH-2 Reference Manual */
    napi_value module; /**< @brief See SH-2 Reference Manual */
    napi_value code;   /**< @brief See SH-2 Reference Manual */

    status |= napi_create_uint32(env, err->severity, &severity);
    status |= napi_create_uint32(env, err->sequence, &sequence);
    status |= napi_create_uint32(env, err->source, &source);
    status |= napi_create_uint32(env, err->error, &error);
    status |= napi_create_uint32(env, err->module, &module);
    status |= napi_create_uint32(env, err->code, &code);

    status |= napi_set_named_property(env, obj, "severity", severity);
    status |= napi_set_named_property(env, obj, "sequence", sequence);
    status |= napi_set_named_property(env, obj, "source", source);
    status |= napi_set_named_property(env, obj, "serroreverity", error);
    status |= napi_set_named_property(env, obj, "smoduleeverity", module);
    status |= napi_set_named_property(env, obj, "scodeeverity", code);

    if (status != napi_ok) {
        napi_throw_error(env, ERROR_TRANSLATING_STRUCT_TO_NODE,
                         "Couldn't build an ErrorRecord.");
        return NULL;
    }

    return obj;
}
