{
  "targets": [
    {
      "target_name": "libvl53l5cx_driver",
      "type": "none",
      "actions": [
        {
          "action_name": "Build VL53L5CX Linux reference driver",
          "inputs": ["build-reference-driver.js", "ref_driver.Makefile"],
          "outputs": [
            "build/ref_drv/libvl53l5cx-driver.so",
            "build/ref_drv/platform.o",
            "build/ref_drv/ref_driver.Makefile",
            "build/ref_drv/vl53l5cx_api.o",
            "build/ref_drv/vl53l5cx_plugin_detection_thresholds.o",
            "build/ref_drv/vl53l5cx_plugin_motion_indicator.o",
            "build/ref_drv/vl53l5cx_plugin_xtalk.o"
          ],
          "action": [
            "node", "build-reference-driver.js"
          ]
        }
      ],
      "direct_dependent_settings": {
        "include_dirs": [
          "vl53l5cx-linux/user/uld-driver/inc",
          "vl53l5cx-linux/user/platform"
        ],
        "libraries": [
          "-Lref_drv",
          "-lvl53l5cx-driver"
        ]
      }
    },
    {
      "target_name": "vl53l5cx_native",
      "sources": [
        "src/binding.c",
        "src/error.c",
        "src/funcs.c"
      ],
      "link_settings": {
        "libraries": [
          "-Wl,-rpath=build/ref_drv/"
        ]
      },
      "dependencies": [ "libvl53l5cx_driver" ]
    }
  ]
}
