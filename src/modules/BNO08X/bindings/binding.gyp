{
  "targets": [
    {
      "target_name": "bno08x_native",
      "sources": [
        "src/binding.c",
        "src/sh2_hal.c",
        "src/types.c",
        "src/error.c",
        "src/funcs.c",
        "src/constants.c",
        "src/sh2/sh2.c",
        "src/sh2/euler.c",
        "src/sh2/sh2_SensorValue.c",
        "src/sh2/sh2_util.c",
        "src/sh2/shtp.c"
      ],
      "include_dirs": [
          "src/"
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