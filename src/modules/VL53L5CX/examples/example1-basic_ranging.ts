import { stdout } from "process";
import { vl53l5cx } from "..";
import { sleep } from "../../../utils";

function example1() {
    try {
        vl53l5cx.vl53l5cx_comms_init(0);
        vl53l5cx.vl53l5cx_is_alive(0);
        vl53l5cx.vl53l5cx_start_ranging(0);
        for (let i = 0; i < 10; i++) {
            while (vl53l5cx.vl53l5cx_check_data_ready(0) == false) {
                const sab = new SharedArrayBuffer(4);
                const int32 = new Int32Array(sab);
                Atomics.wait(int32, 0, 0, 5); // Sleep for 5ms
            }
            const data = vl53l5cx.vl53l5cx_get_ranging_data(0);
            console.log(`Chip temperature: ${data.chipTempC}`);
            for (let zone of data.scanZones) {
                if (zone)
                    console.log(zone)
            }
        }
        vl53l5cx.vl53l5cx_stop_ranging(0);
    } catch (e) {
        console.log(e);
    }
}

example1()