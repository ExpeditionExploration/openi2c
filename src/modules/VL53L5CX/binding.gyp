{
  "targets": [
    {
      "target_name": "vl53l5cx_native",
      "sources": [ 
          "src/binding.c",
          "src/constants.h",
          "src/error.c",
          "src/funcs.c",
      ],
      "include_dirs": [
          "vl53l5cx-linux/user/uld-driver/inc",
          "vl53l5cx-linux/user/platform"
      ]
    }
  ]
}