
import { Module } from '../Module';
import './index.d.ts';
const driver = require('./build/Release/vl53l5cx_native.node');
const vl53l5cx = driver;

export type Config = {
    address: number
}

export type ScanZone = {
    zoneIndex: number;
    distanceMillimeters: number;
    status: number;
}
export type ScanData = {
    chipTempC: number;
    scanZones: ScanZone[];
}

export class VL53L5CX extends Module<Config> {
    config = { // Config is not in use at the moment. It uses the default addr.
        address: 0x52,
    }

    private static sensorCount = 0;
    private thisSensor: number | null = null;
    private busNumber: number | undefined;

    constructor(busNumber?: number, config?: Partial<Config>) {
        // This doesn't really do anything for this module. The module uses
        // vendor provided C driver to communicate. But it satisfies TS.
        super(busNumber);

        // Also not in use currently. It is always uses /dev/i2c-1 currently.
        this.busNumber = busNumber;

        // Doesn't respect any configuration. The example drives using default
        // settings the driver sets up for the sensor.
        this.config = Object.assign(this.config, config);
        this.init();
    }

    async init() {
        // super.init(); // This makes no sense in this case.

        this.commsInit();
    }

    /**
     * Initialize communications for the sensor.
     * 
     * @returns Nothing and throws instead in case something fails.
     */
    commsInit(): undefined {
        if (this.thisSensor != undefined) {
            throw "already initialized";
        } else if (VL53L5CX.sensorCount >= 10) {
            throw "max sensor count reached";
        } 
        this.thisSensor = VL53L5CX.sensorCount++;
        vl53l5cx.vl53l5cx_comms_init(this.thisSensor);
    }

    /**
     * Check if the sensor is responsive.
     * 
     * @returns Nothing and throws instead in case something fails.
     */
    isAlive(): undefined {
        vl53l5cx.vl53l5cx_is_alive(this.thisSensor);
    }

    /**
     * Start ranging.
     * 
     * @returns Nothing and throws instead in case something fails.
     */
    startRanging(): undefined {
        vl53l5cx.vl53l5cx_start_ranging(this.thisSensor);
    }

    /**
     * Stop ranging.
     * 
     * @returns Nothing again, and throws if needed.
     */
    stopRanging(): undefined {
        vl53l5cx.vl53l5cx_stop_ranging(this.thisSensor);
    }

    /**
     * Check for new ranging data.
     * 
     * @returns Boolean whether new scan data is available. False if not.
     */
    checkDataReady(): undefined {
        return vl53l5cx.vl53l5cx_check_data_ready(this.thisSensor);
    }

    /**
     * Get ranging data.
     * 
     * @returns Ranging data object
     */
    getRangingData(): ScanData {
        return vl53l5cx.vl53l5cx_get_ranging_data(this.thisSensor);
    }
}


