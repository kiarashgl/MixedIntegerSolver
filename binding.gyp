{
    "targets": [
        {
            "target_name": "Osi",
            "sources": [ "osi/Osi.cpp", "osi/index.cpp" ],
            "include_dirs" : [
 	 			"<!(node -e \"require('nan')\")"
			],
            "libraries" : ["-lClp -lOsiClp -lCbc -lOsiCbc"],
            "cflags!": ["-fno-exceptions", "-fno-rtti"],
            "cflags_cc!": ["-fno-exceptions", "-fno-rtti"]
        }
    ],
}