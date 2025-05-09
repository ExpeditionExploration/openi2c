import { bno08x } from '..';
import { SensorEvent } from '../types';

const bus = parseInt(process.env.I2C_BUS ?? '1');
const address = parseInt(process.env.I2C_ADDRESS ?? '0x4b');

test('Setting sensor callback doesn\'t throw anything', () => {
  expect(() => bno08x.setI2CConfig(bus, address)).not.toThrow();
  expect(() => bno08x.setSensorCallback(
    (cookie: any, event: SensorEvent) => {},
    "lala")).not.toThrow();
});
