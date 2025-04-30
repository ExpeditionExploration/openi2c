
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
};
