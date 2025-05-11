import { sleep, toSigned16Bit } from '../../utils';
import { Module } from '../Module';
import {
    BUS_VOLTAGE_REGISTER,
    CALIBRATION_REGISTER,
    CONFIGURATION_REGISTER,
    CURRENT_REGISTER,
    POWER_REGISTER,
    SHUNT_VOLTAGE_REGISTER
} from './constants'

export enum BusVoltage {
    Max16V = 0,
    Max32V = 1,
}

export enum ShuntVoltage {
    /**
     * TinyRange offers the range of +-40mV over the shunt resistor.
     * 
     * Conversly, TinyRange offers best resolution (~1.22μV). 
     */
    Max40mV = 0, // Gain = 1

    /**
     * SmallRange offers the range of +-80mV over the shunt resistor.
     * 
     * Conversly, SmallRange offers a bit worse resolution (~2.44μV).
     */
    Max80mV = 1, // Gain = 2

    /**
     * LargeRange offers the range of +-160mV over the shunt resistor.
     * 
     * Conversly, LargeRange offers worse resolution (~4.88μV).
     */
    Max160mV = 2, // Gain = 4

    /**
     * HugeRange offers the range of 320mV over the shunt resistor.
     * 
     * Conversly, HugeRange offers worst resolution (~9.766μV).
     */
    Max320mV = 3, // Gain = 8
}


export type Config = {
    address: number;

    // Configuration and calibration parameters

    /**
     * TODO
     */
    reset: number;

    /**
     * Set PGA gain.
     */
    maxShuntVoltage: ShuntVoltage;

    /**
     * Maximum expected bus voltage.
     */
    maxBusVoltage: BusVoltage;

    /**
     * Shunt resistor is 0.5ohm, but this lets you input the real value in case needed.
     */
    shuntResistance: number;

    /**
     * Shunt ADC resolution/averaging setting. See datasheet page 27. Four bits.
     */
    shuntADCResolution: number;

    /**
     * Bus ADC resolution/averaging setting. See datasheet page 26/27. Four bits.
     */
    busADCResolution: number;

    /**
     * TODO
     */
    mode: number;

    /**
     * TODO
     */
    calibration: number;
}

export class INA219 extends Module<Config> {
    config: Config = {
        address: 0x40, // Default slave address with no soldering
        reset: 0b0, // Default
        maxBusVoltage: BusVoltage.Max32V, // Default
        shuntResistance: 0.1,
        maxShuntVoltage: ShuntVoltage.Max320mV, // Default to smaller resolution to avoid overflow.
        busADCResolution: 0b0010, // Default is 12-bit mode
        shuntADCResolution: 0b0010, // Default is 12-bit mode
        mode: 0b111, // Default is Shunt and Bus, Continuous
        calibration: 0, // Default is No calibration
    }

    private lsbCurrent?: number;
    private lsbPower?: number;

    constructor(busNumber?: number, config?: Partial<Config>) {
        super(busNumber);
        this.config = Object.assign(this.config, config);
    }

    async init() {
        super.init();

        console.log(`Initialisation:`);

        // Init configuration register.
        let configuration = await this.readConfiguration();

        configuration |= (this.config.reset & 0b1) << 15; // RST
        configuration |= (this.config.maxBusVoltage & 0b1) << 13; // BRNG
        configuration |= (this.config.maxShuntVoltage & 0b11) << 11; // PGA
        configuration |= (this.config.busADCResolution & 0b1111) << 7; // BADC
        configuration |= (this.config.shuntADCResolution & 0b1111) << 3; // SADC
        configuration |= (this.config.mode & 0b111) << 0; // MODE
        configuration = 0b0011100110011111; // Default

        await this.writeConfiguration(configuration);

        await this.calibrate();
    }

    /**
     * Calibrate
     * 
     * **Note**: Datasheet describes secondary, correctional calibration step. It is not implemented yet.
     * 
     * @param vBusMax Volts
     * @param vShuntMax Volts
     * @param shuntResistance Ohms
     */
    async calibrate() {
        let calibration = await this.readCalibration();

        let vBusMax = this.config.maxBusVoltage ? 32000 : 16000;
        let shuntResistance = this.config.shuntResistance;
        let vShuntMax = 40 << this.config.maxShuntVoltage;

        // Calculate max possible/expected current
        const maxExpectedI = vShuntMax / shuntResistance / 1000;

        this.debug(`Bus Max / mV: ${vBusMax}, Shunt Max / mV: ${vShuntMax}, Current Max / A: ${maxExpectedI}, Shunt Resistance / Ohm: ${shuntResistance}`);

        // Calculate min and max LSB.
        const minimumLSB = maxExpectedI / 32767; // 2**15 - 1
        const maximumLSB = maxExpectedI / 4096;

        // Choose LSB from midpoint of min and max. // TODO: Explain why this is better than what's in the Datasheet
        const LSB = (minimumLSB + maximumLSB) / 2; // Adjust this for more granular or coarse resolution
        this.lsbCurrent = LSB;

        this.debug(`LSB / A / bit: ${LSB} (Min: ${minimumLSB}, Max: ${maximumLSB})`);

        // Calculate calibration register value and write it to the register.
        calibration = Number.parseInt((0.04096 / (LSB * shuntResistance)).toFixed(0));

        // TODO: Remove override when calibration works
        this.lsbCurrent = 0.01; // Override
        calibration = 4096; // Override

        await this.writeCalibration(calibration);

        // Calculate power LSB
        this.lsbPower = 20 * this.lsbCurrent;

        this.config.calibration = calibration;

        /*
        // Compute maximum current before overflow.
        const maxCurrent = LSB * 32767;
        let maxCurrentBeforeOverflow: number;
        if (maxCurrent >= maxPossibleI) {
            maxCurrentBeforeOverflow = maxPossibleI;
        } else {
            maxCurrentBeforeOverflow = maxCurrent;
        }

        // Compute maximum shunt voltage.
        const maxShuntVoltage = maxCurrentBeforeOverflow * shuntResistance;
        let maxShuntVoltageBeforeOverflow: number;
        if (maxShuntVoltage >= vShuntMax) {
            maxShuntVoltageBeforeOverflow = vShuntMax;
        } else {
            maxShuntVoltageBeforeOverflow = maxShuntVoltage;
        }

        // Calculate maximum power
        const maximumPower = maxCurrentBeforeOverflow * vBusMax;
        */
    }

    async readConfiguration(): Promise<number> {
        const configuration = (await this.readBytes(CONFIGURATION_REGISTER, 2)).readUint16BE(0);
        this.debug(`configuration register value: ${configuration}(0b${(configuration >>> 0).toString(2).padStart(16, '0')})`);
        const RST = (configuration >> 15) & 0b1;
        const D14UNUSED = (configuration >> 14) & 0b1;
        const BRNG = (configuration >> 13) & 0b1;
        const PGA = (configuration >> 11) & 0b11;
        const BADC = (configuration >> 7) & 0b1111;
        const SADC = (configuration >> 3) & 0b1111;
        const MODE = (configuration >> 0) & 0b111;
        this.debug(`RST: ${RST}, D14UNUSED: ${D14UNUSED}, BRNG: ${BRNG}, PGA: 0b${(PGA >>> 0).toString(2).padStart(2, '0')}, BADC: 0b${(BADC >>> 0).toString(2).padStart(4, '0')}, SADC: 0b${(SADC >>> 0).toString(2).padStart(4, '0')}, MODE: 0b${(MODE >>> 0).toString(2).padStart(3, '0')} `);
        return configuration;
    }

    async readShuntVoltage(): Promise<number> {
        const shunt_voltage = (await this.readBytes(SHUNT_VOLTAGE_REGISTER, 2)).readInt16BE(0);
        this.debug(`shunt voltage register value: ${shunt_voltage} (0b${(shunt_voltage >>> 0).toString(2).padStart(32, '0').slice(16, 32)})`);
        const SIGN = (shunt_voltage >> 15) & 0b1;
        const LSB = 10;
        this.debug(`SIGN: ${SIGN}, LSB / uV: ${LSB} `);
        return shunt_voltage * LSB; // Device takes care of PGA-based shifting
    }

    async readBusVoltage(): Promise<number> {
        const bus_voltage = (await this.readBytes(BUS_VOLTAGE_REGISTER, 2)).readUint16BE(0);
        this.debug(`bus voltage register value: ${bus_voltage} (0b${(bus_voltage >>> 0).toString(2).padStart(16, '0')})`);
        const OVF = (bus_voltage >> 0) & 0b1; // Math Overflow Flag (for Power or Current calculations)
        const CNVR = (bus_voltage >> 1) & 0b1; // Conversion Ready
        const D2UNUSED = (bus_voltage >> 2) & 0b1;
        const LSB = 4;
        this.debug(`OVF: ${OVF}, CNVR: ${CNVR}, D2UNUSED: ${D2UNUSED}, LSB / mV: ${LSB} `);
        return (bus_voltage >> 3) * LSB;
    }

    async readPower(): Promise<number> {
        const power = (await this.readBytes(POWER_REGISTER, 2)).readUint16BE(0);
        this.debug(`power register value: ${power} (0b${(power >>> 0).toString(2).padStart(16, '0')})`);
        const LSB = this.lsbPower!;
        this.debug(`LSB / mW: ${LSB} `);
        return power * LSB;
    }

    async readCurrent(): Promise<number> {
        const current = (await this.readBytes(CURRENT_REGISTER, 2)).readInt16BE(0);
        this.debug(`current register value: ${current} (0b${(current >>> 0).toString(2).padStart(32, '0').slice(16, 32)})`);
        const CSIGN = (current >> 15) & 0b1;
        const LSB = this.lsbCurrent!;
        this.debug(`CSIGN: ${CSIGN}, LSB / mA: ${LSB} `);
        return current * LSB;
    }

    async readCalibration(): Promise<number> {
        const calibration = (await this.readBytes(CALIBRATION_REGISTER, 2)).readUint16BE(0);
        this.debug(`calibration register value: ${calibration} (0b${(calibration >>> 0).toString(2).padStart(16, '0')})`);
        const D0UNUSED = (calibration >> 0) & 0b1;
        this.debug(`D0UNUSED: ${D0UNUSED} `);
        return calibration;
    }

    async writeConfiguration(configuration: number): Promise<number> {
        const buf = Buffer.alloc(2);
        buf.writeUInt16BE(configuration);
        await this.writeBytes(CONFIGURATION_REGISTER, buf);
        return this.readConfiguration();
    }

    async writeCalibration(calibration: number): Promise<number> {
        const buf = Buffer.alloc(2);
        buf.writeUInt16BE(calibration);
        await this.writeBytes(CALIBRATION_REGISTER, buf);
        return this.readCalibration();
    }
}