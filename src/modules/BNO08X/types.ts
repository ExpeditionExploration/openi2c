import { LargeNumberLike } from "crypto"

export type SensorEvent = {
    timestampMicrosecond: number,
    delayMicrosecond: number,
    len: number,
    reportId: number,
    report: ArrayBuffer
}

export type SensorCallback = (cookie: any, event: SensorEvent) => void;

export type ProductId = {
    resetCause: number,
    swVersionMajor: number,
    swVersionMinor: number,
    swPartNumber: number,
    swBuildNumber: number,
    swVersionPatch: number,
    reserved0: number,
    reserved1: number,
}

export enum SensorId {
    SH2_RAW_ACCELEROMETER,
    SH2_ACCELEROMETER,
    SH2_LINEAR_ACCELERATION,
    SH2_GRAVITY,
    SH2_RAW_GYROSCOPE,
    SH2_GYROSCOPE_CALIBRATED,
    SH2_GYROSCOPE_UNCALIBRATED,
    SH2_RAW_MAGNETOMETER,
    SH2_MAGNETIC_FIELD_CALIBRATED,
    SH2_MAGNETIC_FIELD_UNCALIBRATED,
    SH2_ROTATION_VECTOR,
    SH2_GAME_ROTATION_VECTOR,
    SH2_GEOMAGNETIC_ROTATION_VECTOR,
    SH2_PRESSURE,
    SH2_AMBIENT_LIGHT,
    SH2_HUMIDITY,
    SH2_PROXIMITY,
    SH2_TEMPERATURE,
    SH2_RESERVED,
    SH2_TAP_DETECTOR,
    SH2_STEP_DETECTOR,
    SH2_STEP_COUNTER,
    SH2_SIGNIFICANT_MOTION,
    SH2_STABILITY_CLASSIFIER,
    SH2_SHAKE_DETECTOR,
    SH2_FLIP_DETECTOR,
    SH2_PICKUP_DETECTOR,
    SH2_STABILITY_DETECTOR,
    SH2_PERSONAL_ACTIVITY_CLASSIFIER,
    SH2_SLEEP_DETECTOR,
    SH2_TILT_DETECTOR,
    SH2_POCKET_DETECTOR,
    SH2_CIRCLE_DETECTOR,
    SH2_HEART_RATE_MONITOR,
    SH2_ARVR_STABILIZED_RV,
    SH2_ARVR_STABILIZED_GRV,
    SH2_GYRO_INTEGRATED_RV,
    SH2_IZRO_MOTION_REQUEST,
    SH2_RAW_OPTICAL_FLOW,
    SH2_DEAD_RECKONING_POSE,
    SH2_WHEEL_ENCODER,

    // UPDATE to reflect greatest sensor id
    SH2_MAX_SENSOR_ID
}

export type SensorConfig = {
        /* Change sensitivity enabled */
        changeSensitivityEnabled: boolean,  /**< @brief Enable reports on change */

        /* Change sensitivity - true if relative; false if absolute */
        changeSensitivityRelative: boolean,  /**< @brief Change reports relative (vs absolute) */
    
        /* Wake-up enabled */
        wakeupEnabled: boolean,  /**< @brief Wake host on event */
    
        /* Always on enabled */
        alwaysOnEnabled: boolean,  /**< @brief Sensor remains on in sleep state */
    
        sniffEnabled: boolean, /**< @brief Any output from this sensor should
                                be sent to host, but reportInterval_us and
                                sensorSpecific do not influence sensor
                                operation. Not supported by all sensors. */
    
        /* 16-bit signed fixed point integer representing the value a
         * sensor output must exceed in order to trigger another input
         * report. A setting of 0 causes all reports to be sent.
         */
        changeSensitivity: number,  /**< @brief Report-on-change threshold */
    
        /* Interval in microseconds between asynchronous input reports. */
        reportInterval_us: number,  /**< @brief [uS] Report interval */
    
        /* Reserved field, not used. */
        batchInterval_us: number,  /**< @brief [uS] Batch interval */
    
        /* Meaning is sensor specific */
        sensorSpecific: number,  /**< @brief See SH-2 Reference Manual for details. */    
}

export type SensorMetadata = {
    meVersion: number,   /**< @brief Motion Engine Version */
    mhVersion: number,  /**< @brief Motion Hub Version */
    shVersion: number,  /**< @brief SensorHub Version */
    range: number,  /**< @brief Same units as sensor reports */
    resolution: number,  /**< @brief Same units as sensor reports */
    revision: number,  /**< @brief Metadata record format revision */
    power_mA: number,    /**< @brief [mA] Fixed point 16Q10 format */
    minPeriod_uS: number,  /**< @brief [uS] */
    maxPeriod_uS: number,  /**< @brief [uS] */
    fifoReserved: number,  /**< @brief (Unused) */
    fifoMax: number,  /**< @brief (Unused) */
    batchBufferBytes: number,  /**< @brief (Unused) */
    qPoint1: number,     /**< @brief q point for sensor values */
    qPoint2: number,     /**< @brief q point for accuracy or bias fields */
    qPoint3: number,     /**< @brief q point for sensor data change sensitivity */
    vendorIdLen: number, /**< @brief [bytes] */
    vendorId: string;  /**< @brief Vendor name and part number */
    sensorSpecificLen: number,  /**< @brief [bytes] */
    sensorSpecific: ArrayBuffer;  /**< @brief See SH-2 Reference Manual */
}

export type sh2_ErrorRecord = {
    severity: number,   /**< @brief Error severity, 0: most severe. */
    sequence: number,   /**< @brief Sequence number (by severity) */
    source: number,     /**< @brief 1-MotionEngine, 2-MotionHub, 3-SensorHub, 4-Chip  */
    error: number,      /**< @brief See SH-2 Reference Manual */
    module: number,     /**< @brief See SH-2 Reference Manual */
    code: number,       /**< @brief See SH-2 Reference Manual */
}

export type Counts = {
    offered: number,   /**< @brief [events] */
    accepted: number,  /**< @brief [events] */
    on: number,        /**< @brief [events] */
    attempted: number, /**< @brief [events] */
}

export type TareBasis = {
    SH2_TARE_BASIS_ROTATION_VECTOR: number,             /**< @brief Use Rotation Vector */
    SH2_TARE_BASIS_GAMING_ROTATION_VECTOR: number,      /**< @brief Use Game Rotation Vector */
    SH2_TARE_BASIS_GEOMAGNETIC_ROTATION_VECTOR: number, /**< @brief Use Geomagnetic R.V. */
}

export type TareAxis = {
    SH2_TARE_X: number,  /**< @brief sh2_tareNow() axes bit field */
    SH2_TARE_Y: number,  /**< @brief sh2_tareNow() axes bit field */
    SH2_TARE_Z: number,  /**< @brief sh2_tareNow() axes bit field */
    SH2_TARE_CONTROL_VECTOR_X: number,         /**< @brief Use X axis of source and frame to perform tare */
    SH2_TARE_CONTROL_VECTOR_Y: number,         /**< @brief Use Y axis of source and frame to perform tare */
    SH2_TARE_CONTROL_VECTOR_Z: number,         /**< @brief Use Z axis of source and frame to perform tare */
    SH2_TARE_CONTROL_SEQUENCE_DEFAULT: number, /**< @brief Tare "typical" toration for source/axis combination */
    SH2_TARE_CONTROL_SEQUENCE_PRE: number,     /**< @brief Apply to pre-rotation (tare world to device) */
    SH2_TARE_CONTROL_SEQUENCE_POST: number,    /**< @brief Apply to post-rotation (tare device to world) */
} 

export type Quaternion = {
    x: number,
    y: number,
    z: number,
    w: number,
};

export enum OscillatorType {
    SH2_OSC_INTERNAL,
    SH2_OSC_EXT_CRYSTAL,
    SH2_OSC_EXT_CLOCK,
};

export enum CalibrationStatus {
    SH2_CAL_SUCCESS = 0,
    SH2_CAL_NO_ZRO,
    SH2_CAL_NO_STATIONARY_DETECTION,
    SH2_CAL_ROTATION_OUTSIDE_SPEC,
    SH2_CAL_ZRO_OUTSIDE_SPEC,
    SH2_CAL_ZGO_OUTSIDE_SPEC,
    SH2_CAL_GYRO_GAIN_OUTSIDE_SPEC,
    SH2_CAL_GYRO_PERIOD_OUTSIDE_SPEC,
    SH2_CAL_GYRO_DROPS_OUTSIDE_SPEC,
};

export enum InteractiveZROMotionIntent {
    SH2_IZRO_MI_UNKNOWN,
    SH2_IZRO_MI_STATIONARY_NO_VIBRATION,
    SH2_IZRO_MI_STATIONARY_WITH_VIBRATION,
    SH2_IZRO_MI_IN_MOTION,
    SH2_IZRO_MI_ACCELERATING,
};

export enum InteractiveZROMotionRequest {
    SH2_IZRO_MR_NO_REQUEST = 0,
    SH2_IZRO_MR_STAY_STATIONARY,
    SH2_IZRO_MR_STATIONARY_NON_URGENT,
    SH2_IZRO_MR_STATIONARY_URGENT,
}

export enum AsyncEventId {
    SH2_RESET,
    SH2_SHTP_EVENT,
    SH2_GET_FEATURE_RESP,
}

export enum ShtpEvent {
    SH2_SHTP_TX_DISCARD = 0,
    SH2_SHTP_SHORT_FRAGMENT = 1,
    SH2_SHTP_TOO_LARGE_PAYLOADS = 2,
    SH2_SHTP_BAD_RX_CHAN = 3,
    SH2_SHTP_BAD_TX_CHAN = 4,
    SH2_SHTP_BAD_FRAGMENT = 5,
    SH2_SHTP_BAD_SN = 6,
    SH2_SHTP_INTERRUPTED_PAYLOAD = 7,
};

export type SensorConfigResponse = {
    sensorId: SensorId;
    sensorConfig: SensorConfig;
}

/// Only shtpEvent or SensorConfigResponse is defined at a time.
export type AsyncEvent = {
    eventId: number;
    shtpEvent: ShtpEvent | undefined;
    SensorConfigResponse: SensorConfigResponse | undefined;
}