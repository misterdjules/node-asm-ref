{
	"includes": [ "deps/common-sqlite.gypi" ],
	"variables": {
		"sqlite%":"internal",
		"sqlite_libname%":"sqlite3"
	},
	"targets": [
		{
			"target_name": "x86-asm-ref",
			"dependencies": [
              "libx86ref"
            ],
			"sources": [ "addon.cc", "x86asmref.cc" ],
			"conditions": [
				[ "OS=='win'", {
					"defines": [
						"X86_REF_STATIC"
					]
				}]
			],
			"include_dirs": [
				"../../libs/intel-x86-ref"
			],
		},
		{
			'target_name': 'libx86ref',
	        'type': 'static_library',
	        'sources': [
				'../../libs/intel-x86-ref/intel_x86_ref.c'
	        ],
	        "conditions": [
				[ "OS=='win'", {
					"defines": [
						"X86_REF_STATIC"
					]
				}]
			],
	        "dependencies": [
				"deps/sqlite3.gyp:sqlite3"
	        ]
	    }
	]
}