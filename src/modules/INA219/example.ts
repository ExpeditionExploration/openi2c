import { INA219, Config, ShuntVoltage, BusVoltage } from '.';
import { sleep } from '../../utils';

async function main() {
    const bus = parseInt(process.argv[
        process.argv.length - 1
    ]);
    
    const ina219 = new INA219(isNaN(bus) ? 5 : bus);
    await ina219.init();

    let iter: number = 0;
    while (true) {
        console.log(`Measurement #${++iter}:`);

        const shunt = await ina219.readShuntVoltage();
        const bus = await ina219.readBusVoltage();
        const power = await ina219.readPower();
        const current = await ina219.readCurrent();
        const calibration = await ina219.readCalibration();

        console.log(`Shunt / mV: ${shunt}, Bus / mV: ${bus}, Current / mA: ${current}, Power / mW: ${power}`);

        await sleep(300);
    }
}
main();
