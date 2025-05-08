
export type SensorEvent = {
    timestampMicroseconds: number,
    delayMicroseconds: number,
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
    NO_REQUEST,
    STAY_STATIONARY,
    STATIONARY_NON_URGENT,
    STATIONARY_URGENT,
}

export enum AsyncEventId {
    RESET,
    SHTP_EVENT,
    GET_FEATURE_RESP,
}

export enum ShtpEvent {
    TX_DISCARD = 0,
    SHORT_FRAGMENT = 1,
    TOO_LARGE_PAYLOADS = 2,
    BAD_RX_CHAN = 3,
    BAD_TX_CHAN = 4,
    BAD_FRAGMENT = 5,
    BAD_SN = 6,
    INTERRUPTED_PAYLOAD = 7,
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

export type EventCallback = (cookie: any, event: AsyncEvent) => void;

export enum Sensors {
    CAL_ACCEL,
    CAL_GYRO,
    CAL_MAG,
    CAL_PLANAR,
    CAL_ON_TABLE,
    CAL_ZERO_GYRO_CONTROL_MASK,
    CAL_ZERO_GYRO_CONTROL_ON_TABLE_DETECT,
    CAL_ZERO_GYRO_CONTROL_NEVER,
    CAL_ZERO_GYRO_CONTROL_ON_TABLE_CLASS,
    CAL_ZERO_GYRO_CONTROL_ON_TABLE_CLASS_OR_LONG_TERM_STABLE
}
export type SensorSet = Sensors;

export enum WheelDatatype {
    Position = 0,
    Velocity = 1,
}

export type BNO08X = {

    /**
     * @brief Open a session with a sensor hub.
     *
     * This function should be called before others in this API.
     * An instance of an SH2 HAL should be passed in.
     * This call will result in the open() function of the HAL being called.
     *
     * As part of the initialization process, a callback function is registered that will
     * be invoked when the device generates certain events.  (See sh2_AsyncEventId)
     *
     * @param pHal Pointer to an SH2 HAL instance, provided by the target system.
     * @param  eventCallback Will be called when events, such as reset complete, occur.
     * @param  eventCookie Will be passed to eventCallback.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    open: (callback: EventCallback, event: AsyncEvent) => void,

    /**
     * @brief Close a session with a sensor hub.
     *
     * This should be called at the end of a sensor hub session.  
     * The underlying SHTP and HAL instances will be closed.
     *
     */
    close: () => void,

    /**
     * @brief Service the SH2 device, reading any data that is available and dispatching callbacks.
     *
     * This function should be called periodically by the host system to service an open sensor hub.
     *
     */
    service: () => void,

    /**
     * @brief Register a function to receive sensor events.
     *
     * @param  callback A function that will be called each time a sensor event is received.
     * @param  cookie  A value that will be passed to the sensor callback function.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setSensorCallback: (callback: SensorCallback, cookie: any) => void,  // Throws on error

    /**
     * @brief Reset the sensor hub device by sending RESET (1) command on "device" channel.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    devReset: () => void,  // Throws on error

    /**
     * @brief Turn sensor hub on by sending ON (2) command on "device" channel.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    devOn: () => void, // Throws on error

    /**
     * @brief Put sensor hub in sleep state by sending SLEEP (3) command on "device" channel.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    devSleep: () => void, // Throws on error

    /**
     * @brief Get Product ID information from Sensorhub.
     *
     * @param  prodIds Pointer to structure that will receive results.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getProdIds: () => ProductId[], // Throws on error

    /**
     * @brief Get sensor configuration.
     *
     * @param  sensorId Which sensor to query.
     * @param  config SensorConfig structure to store results.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getSensorConfig: (sensorId: SensorId) => SensorConfig, // Throws on error

    /**
     * @brief Set sensor configuration. (e.g enable a sensor at a particular rate.)
     *
     * @param  sensorId Which sensor to configure.
     * @param  pConfig Pointer to structure holding sensor configuration.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setSensorConfig: (sensorId: SensorId, conf: SensorConfig) => void, // Throws on error

    /**
     * @brief Get metadata related to a sensor.
     *
     * @param  sensorId Which sensor to query.
     * @param  pData Pointer to structure to receive the results.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getMetadata: (sensorId: SensorId) => SensorMetadata, // Throws on error

    /**
     * @brief Get an FRS record.
     *
     * @param  recordId Which FRS Record to retrieve.
     * @param  pData pointer to buffer to receive the results
     * @param[in] words Size of pData buffer, in 32-bit words.
     * @param[out] words Number of 32-bit words retrieved.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getFrs: (recordId: number) => ArrayBuffer, // Throws on error

    /**
     * @brief Set an FRS record
     *
     * @param  recordId Which FRS Record to set.
     * @param  pData pointer to buffer containing the new data.
     * @param  words number of 32-bit words to write.  (0 to delete record.)
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setFrs: (recordId: number, fsrData: ArrayBuffer) => void, // THrows on error

    /**
     * @brief Get error counts.
     *
     * @param  severity Only errors of this severity or greater are returned.
     * @param  pErrors Buffer to receive error codes.
     * @param  numErrors size of pErrors array
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getErrors: (severity: number) => sh2_ErrorRecord[], // Throws on error

    /**
     * @brief Read counters related to a sensor.
     *
     * @param  sensorId Which sensor to operate on.
     * @param  pCounts Pointer to Counts structure that will receive data.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getCounts: (sensorId: SensorId) => Counts, // THrows on error

    /**
     * @brief Clear counters related to a sensor.
     *
     * @param  sensorId which sensor to operate on.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    clearCounts: (sensorId: SensorId) => void, // throws on error

    /**
     * @brief Perform a tare operation on one or more axes.
     *
     * @param  axes Bit mask specifying which axes should be tared.
     * @param  basis Which rotation vector to use as the basis for Tare adjustment.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setTareNow: (axes: number, // SH2_TARE_X | SH2_TARE_Y | SH2_TARE_Z
        basis: TareBasis) => void, // Throws on error

    /**
     * @brief Clears the previously applied tare operation.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    clearTare: () => void, // Throws on error

    /**
     * @brief Persist the results of last tare operation to flash.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    persistTare: () => void, // Throws on error

    /**
     * @brief Set the current run-time sensor reorientation. (Set to zero to clear tare.)
     *
     * @param  orientation Quaternion rotation vector to apply as new tare.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setReorientation: (orientation: Quaternion) => void, // Throws blabla.

    /**
     * @brief Command the sensorhub to reset.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    reinitialize: () => void,

    /**
     * @brief Save Dynamic Calibration Data to flash.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    saveDcNow: () => void,

    /**
     * @brief Get Oscillator type.
     *
     * @param  pOscType pointer to data structure to receive results.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getOscType: () => OscillatorType,

    /**
     * @brief Enable/Disable dynamic calibration for certain sensors
     *
     * @param  sensors Bit mask to configure which sensors are affected.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setCalConfig: (sensors: SensorSet) => void,

    /**
     * @brief Get dynamic calibration configuration settings.
     *
     * @param  pSensors pointer to Bit mask, set on return.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    getCalConfig: () => SensorSet,

    /**
     * @brief Configure automatic saving of dynamic calibration data.
     *
     * @param  enabled Enable or Disable DCD auto-save.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setDcdAutoSave: (enabled: boolean) => void,

    /**
     * @brief Immediately issue all buffered sensor reports from a given sensor.
     *
     * @param  sensorId Which sensor reports to flush.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    flush: (sensor: SensorId) => void,

    /**
     * @brief Command clear DCD in RAM, then reset sensor hub.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    clearDcdAndReset: () => void,

    /**
     * @brief Start simple self-calibration procedure.
     *
     * @parameter interval_us sensor report interval, uS.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    startCal: (intevalMicroseconds: number) => void,

    /**
     * @brief Finish simple self-calibration procedure.
     *
     * @parameter status contains calibration status code on return.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    finishCal: () => CalibrationStatus,

    /**
     * @brief send Interactive ZRO Request.
     *
     * @parameter intent Inform the sensor hub what sort of motion should be in progress.
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    setIZro: (intent: InteractiveZROMotionIntent) => void,

    /**
     * @brief Report wheel position/velocity to sensor hub.
     * @parameter wheelIndex platform-dependent: 0= left, 1= right for
     *   typical differential drive robot
     * @parameter timestamp microsecond timestamp (hub scale) of measurement
     * @parameter wheelData raw wheel position or velocity
     * @parameter dataType 0 if data is position, 1 if data is velocity
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    reportWheelEncoder: (
        wheelIndex: number, timestamp: number,
        wheeldata: number, wheelDt: WheelDatatype
    ) => void,

    /**
     * @brief Save Dead Reckoning Calibration Data to flash.
     *
     * @return SH2_OK (0), on success.  Negative value from sh2_err.h on error.
     */
    saveDeadReckoningCalNow: () => void,
}
