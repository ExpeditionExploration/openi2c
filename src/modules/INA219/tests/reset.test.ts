import { Config, INA219 } from '..';
import { sleep } from '../../../utils';
import { ADCSetting } from '../constants';

const bus = parseInt(process.env.I2C_BUS ?? '5');
const address = parseInt(process.env.I2C_ADDRESS ?? '0x40');

test('Tests resetting device', async () => {
    expect(async ()=>{
        // Create ina219 instance with non-default settings
        const conf: Config = {
            address: address,
            busADCResolution: ADCSetting.Reso12bSamples128,
            shuntADCResolution: ADCSetting.Reso12bSamples128,
        };
        const ina219 = new INA219(bus, conf);
        await ina219.init();

        // Read the configuration and check the adc settings have been reset
        await ina219.reset();
        const resetedConf = await ina219.readConfiguration();
        expect((resetedConf >> 3) & 0xF).toEqual(ADCSetting.Reso12b); // SADC
        expect((resetedConf >> 7) & 0xF).toEqual(ADCSetting.Reso12b); // BADC
    }).not.toThrow();
});
