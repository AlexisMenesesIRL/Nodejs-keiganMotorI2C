{
  "targets": [
    {
      "target_name": "ki2c",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
            "conditions":[
      	["OS=='linux'", {
      	  "sources": [  "keigani2c/ki2c.cc","keigani2c/KM1_I2C.cc", "keigani2c/Definitions.h","keigani2c/TypeUtility.cc", "keigani2c/CRC16.cc" ]
      	  }],
      	["OS=='mac'", {
      	  "sources": [ "keigani2c/nli2c.cc" ]
      	}],
        ["OS=='win'", {
      	  "sources": [ "keigani2c/nli2c.cc" ]
      	}]
      ], 
      "include_dirs" : [
          "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "libraries": [],
      "defines": [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
