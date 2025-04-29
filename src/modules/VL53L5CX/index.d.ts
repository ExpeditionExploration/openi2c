
export declare const vl53l5cx: {
  /**
   * Initialize communications with a sensor.
   * 
   * @returns nothing
   * @throws errors if something went awry
   */
  vl53l5cx_comms_init(): undefined;

  /**
   * Check if sensor is alive.
   * 
   * @returns nothing
   * @throws errors if something went awry
   */
  vl53l5cx_is_alive(): boolean;

  /**
   * Wait for ranging data to be ready.
   * 
   * @return true
   * @throws errors if something went awry
   */
  vl53l5cx_wait_for_dataready(): boolean;

  /**
   * Start ranging
   * 
   * @return nothing
   * @throws errors if something went awry
   */
  vl53l5cx_start_ranging(): undefined;

  /**
   * Stop ranging
   * 
   * @return nothing
   * @throws errors if something went awry
   */
  vl53l5cx_stop_ranging(): undefined;
};
