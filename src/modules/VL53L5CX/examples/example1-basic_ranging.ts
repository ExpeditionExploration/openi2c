import { stdout } from "process";
import { vl53l5cx } from "..";
import { sleep } from "../../../utils";

function example1() {
    const cfg = 0;
    vl53l5cx.vl53l5cx_comms_init(cfg);
    vl53l5cx.vl53l5cx_start_ranging(cfg);
    for (let i = 0; i < 10; i++) {
        while (!vl53l5cx.vl53l5cx_check_data_ready(cfg)) {
            const sab = new SharedArrayBuffer(4);
            const int32 = new Int32Array(sab);
            Atomics.wait(int32, 0, 0, 5); // Sleep for 5ms
        }
        const data = vl53l5cx.vl53l5cx_get_ranging_data(cfg);
        console.log(`Chip temperature: ${data.chipTempC}C degrees`);
        for (let zone of data.scanZones) {
            if (zone) { // TODO: Change C code to make arr len reflect zone count
                console.log(zone);
            }
        }
    }
}

example1()