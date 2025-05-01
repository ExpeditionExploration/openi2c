# VL53L5CX
Datasheet: https://www.st.com/resource/en/datasheet/vl53l5cx.pdf

## About

VL53L5CX is a Time-of-Flight 8x8 multizone ranging sensor with wide field of
view.

This module is in its early stages. It has bare minimum functionality to run
basic ranging for single sensor at default address. And the api isn't complete
or stable either.

## Pre-build

You need `make`, `gcc` and `ar` to build the C driver and Node bindings for it.

## Build

For the moment, to build the Node extension and run the example code in the `examples`directory, execute the following from the module's directory.
```bash
npx node-gyp configure \
    && npx node-gyp build \
    && npx ts-node examples/example1-basic_ranging.ts
```

## Usage
```ts
// TBD...
// ...
```

