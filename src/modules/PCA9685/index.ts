import { sleep } from '../../utils';
import { Module } from '../Module';
import {
    MODE1,
    PRESCALE,
    LED0_ON_L,
    LED0_ON_H,
    LED0_OFF_L,
    LED0_OFF_H,
} from './constants'

// 12-bit PWM resolution (0..4095)
const PWM_STEPS = 4096;

type Config = {
    address: number;
    frequency: number;
    oscillatorHz: number; // PCA9685 clock source (default 25 MHz)
}

export class PCA9685 extends Module<Config> {
    config = {
        address: 0x40,
        frequency: 50,
        oscillatorHz: 25_000_000,
    }

    constructor(busNumber?: number, config?: Partial<Config>) {
        super(busNumber);
        this.config = Object.assign(this.config, config);
    }

    async init() {
        super.init();

        await this.setFrequency(this.config.frequency);
    }

    async setDutyCycle(channel: number, dutyCycle: number, inverse: boolean = false): Promise<void> {
        dutyCycle = Math.min(1, Math.max(0, dutyCycle)); // Clamp to [0,1]
        if (inverse) {
            dutyCycle = 1 - dutyCycle;
        }
        this.debug(`Set duty cycle for channel ${channel} to ${dutyCycle}`);
        await this.setPWM(channel, 0, Math.round(dutyCycle * (PWM_STEPS - 1)));
    }

    async setFrequency(freq: number) {
        this.debug(`Set PWM frequency to ${freq} Hz`);

        const prescale = Math.round(this.config.oscillatorHz / (PWM_STEPS * freq)) - 1;

        await this.bus.writeByte(this.address, MODE1, 0x10); // sleep
        await this.bus.writeByte(this.address, PRESCALE, prescale); // set frequency prescaler
        // Does it need to sleep?
        await sleep(1);
        await this.bus.writeByte(this.address, MODE1, 0x80); // wake up
        await sleep(1);
    }

    async setPWM(channel: number, on: number, off: number) {
        this.debug(`Set PWM for channel ${channel} to ${on} to ${off}`);
        await Promise.all([
            this.bus.writeByte(this.address, LED0_ON_L + 4 * channel, on & 0xff),
            this.bus.writeByte(this.address, LED0_ON_H + 4 * channel, on >> 8),
            this.bus.writeByte(this.address, LED0_OFF_L + 4 * channel, off & 0xff),
            this.bus.writeByte(this.address, LED0_OFF_H + 4 * channel, off >> 8)
        ]);
    }
}