import { stdout } from "process";
import { ScanData, VL53L5CX } from "..";
import { sleep } from "../../../utils";

function example1() {
    const sensor = new VL53L5CX(1, {address: 0x52});
    sensor.startRanging();
    for (let i = 0; i < 10; i++) {
        while(sensor.checkDataReady() == false) {
            const sab = new SharedArrayBuffer(4);
            const int32 = new Int32Array(sab);
            Atomics.wait(int32, 0, 0, 5); // Sleep for 5ms
        }
        const data = sensor.getRangingData();
        console.log(`Chip temperature: ${data.chipTempC}C`);
        for (let zone of data.scanZones) {
            if (zone)  // TODO: Change C code to make arr len reflect zone count
                console.log(zone)
        }
    }
}

example1()