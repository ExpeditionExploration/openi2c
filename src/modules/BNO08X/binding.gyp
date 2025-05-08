{
  "targets": [
    {
      "target_name": "bno08x_native",
      "sources": [
        "bindings/src/binding.c",
        "bindings/src/sh2_hal.c",
        "bindings/src/types.c",
        "bindings/src/error.c",
        "bindings/src/funcs.c",
        "bindings/src/constants.c",
        "bindings/src/error.c",
        "bindings/src/sh2/sh2.c",
        "bindings/src/sh2/euler.c",
        "bindings/src/sh2/sh2_SensorValue.c",
        "bindings/src/sh2/sh2_util.c",
        "bindings/src/sh2/shtp.c"
      ],
      "include_dirs": [
          "bindings/src/"
      ],
      "cflags!": [
            "-Wall",    # Enable all warnings
            "-Werror",  # Make them errors
            "-Wno-missing-braces", # Require braces
            "-O2",      # Optimize for speed
            "-g0",      # No debug info
            "-fPIC"     # Generate position independent code
      ]
    }
  ]
}
