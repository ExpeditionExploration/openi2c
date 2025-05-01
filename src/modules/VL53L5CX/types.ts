
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
  vl53l5cx_comms_init: (cfg: number) => undefined;

  /**
   * Check if sensor is alive.
   * 
   * @returns nothing
   * @throws errors if something went awry
   */
  vl53l5cx_is_alive: (cfg: number) => undefined;

  /**
   * Check if data is ready to be fetched.
   * 
   * @return true
   * @throws errors if something went awry
   */
  vl53l5cx_check_data_ready: (cfg: number) => boolean;

  /**
   * Start ranging
   * 
   * @return nothing
   * @throws errors if something went awry
   */
  vl53l5cx_start_ranging: (cfg: number) => undefined;

  /**
   * Stop ranging
   * 
   * @return nothing
   * @throws errors if something went awry
   */
  vl53l5cx_stop_ranging: (cfg: number) => undefined;

  /**
   * Fetch scan data.
   * 
   * @param cfg 
   * @returns 
   */
  vl53l5cx_get_ranging_data: (cfg: number) => ScanData;

  /**
   * Set scan resolution.
   * 
   * Other sensor parameters depend on this, so this **must** be the first one
   * to be set.
   * 
   * @param cfg config slot
   * @param resolution One of the VL53L5CX_RESOLUTION_* constants
   * @returns Nothing
   * 
   * @throws `invalid arguments`, `change setting error`
   */
  vl53l5cx_set_resolution: (cfg: number, resolution: number) => undefined;

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
   * @param freq 
   * @returns undefined
   * 
   * @throws `invalid arguments`, `change setting error`
   */
  vl53l5cx_set_ranging_frequency_hz: (cfg: number, freq: number) => undefined;
};
