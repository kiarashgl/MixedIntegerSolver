{
    "targets": [
        {
            "target_name": "Osi",
            "sources": [ "osi/Osi.cpp", "osi/index.cpp" ],
            "include_dirs" : [
 	 			"<!(node -e \"require('nan')\")"
			],
            "libraries" : ["-lClp -lOsiClp"],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"]  
        }
    ],
}