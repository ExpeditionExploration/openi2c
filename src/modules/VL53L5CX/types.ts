
export type ScanZone = {
    zoneIndex: number;
    distanceMillimeters: number;
    status: number;
}
export type ScanData = {
    chipTempC: number;
    scanZones: ScanZone[];
}

export type VL53L5CX = {
  /**
   * Initialize communications with a sensor.
   * 
   * Initializes a device to config slot 0-9, as chosen by the caller. All
   * subsequent api calls for a sensor **must** use the same `cfg`.
   * 
   * @param cfg number 0-9
   * @returns nothing
   * @throws errors if something went awry
   */
  comms_init: (cfg: number) => undefined;

  /**
   * Check if sensor is alive.
   * 
   * @returns nothing
   * @throws errors if something went awry
   */
  is_alive: (cfg: number) => undefined;

  /**
   * Check if data is ready to be fetched.
   * 
   * @return true
   * @throws errors if something went awry
   */
  check_data_ready: (cfg: number) => boolean;

  /**
   * Start ranging
   * 
   * @return nothing
   * @throws errors if something went awry
   */
  start_ranging: (cfg: number) => undefined;

  /**
   * Stop ranging
   * 
   * @return nothing
   * @throws errors if something went awry
   */
  stop_ranging: (cfg: number) => undefined;

  /**
   * Fetch scan data.
   * 
   * @param cfg 
   * @returns {ScanData}
   * 
   * @throws `couldn't create napi value`, `unknown generic error`,
   *  `couldn't set named property`, `couldn't set array napi value`
   */
  get_ranging_data: (cfg: number) => ScanData;

  /**
   * Set scan resolution.
   * 
   * Other sensor parameters depend on this, so this **must** be the first one
   * to be set.
   * 
   * @param cfg config slot
   * @param resolution One of the `VL53L5CX_RESOLUTION_*` constants
   * @returns Nothing
   * 
   * @throws `invalid arguments`, `change setting error`
   */
  set_resolution: (cfg: number, resolution: number) => undefined;

  /**
   *  Used as parameter for vl53l5cx_set_resolution(..)
   */
  VL53L5CX_RESOLUTION_4X4: number;
  /**
   *  Used as parameter for vl53l5cx_set_resolution(..)
   */
  VL53L5CX_RESOLUTION_8X8: number;

  /**
   * Set ranging frequency
   * 
   * **Must** be set after setting the resolution.
   * 
   * @param cfg cfg slot
   * @param freq Frequency in Hz
   * @returns undefined
   * 
   * @throws `invalid arguments`, `change setting error`
   */
  set_ranging_frequency_hz: (cfg: number, freq: number) => undefined;

  /**
   * Set target ordering
   * 
   * **Must** be set after setting the resolution.
   * 
   * @param cfg cfg slot
   * @param ordering One of `VL53L5CX_TARGET_ORDER_*` constants.
   * @returns undefined
   * 
   * @throws `invalid arguments`, `change setting error`
   */
  set_target_order: (cfg: number, ordering: number) => undefined;
  VL53L5CX_TARGET_ORDER_CLOSEST: number;
  VL53L5CX_TARGET_ORDER_STRONGEST: number;

  /**
   * Set ranging mode
   * 
   * @param cfg
   * @mode One of `VL53L5CX_RANGING_MODE_*` constants.
   * @returns undefined
   * 
   * @throws `invalid arguments`, `change setting error`
   */
  set_ranging_mode: (cfg: number, mode: number) => undefined;
  VL53L5CX_RANGING_MODE_CONTINUOUS: number;
  VL53L5CX_RANGING_MODE_AUTONOMOUS: number;
};
