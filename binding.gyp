{
	"includes": [ "deps/sqlite3/common-sqlite.gypi" ],
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
				"deps/asm-ref/libs/intel-x86-ref"
			],
		},
		{
			'target_name': 'libx86ref',
	        'type': 'static_library',
	        'sources': [
				'deps/asm-ref/libs/intel-x86-ref/intel_x86_ref.c'
	        ],
	        "conditions": [
				[ "OS=='win'", {
					"defines": [
						"X86_REF_STATIC"
					]
				}]
			],
	        "dependencies": [
				"deps/sqlite3/sqlite3.gyp:sqlite3",
				"libdebug",
				"libjstring",
				"libsqlite-helper"
	        ],
	        "include_dirs": [
				"deps/asm-ref/libs/debug",
				"deps/asm-ref/libs/jstring",
				"deps/asm-ref/libs/sqlite-helper"
			]
	    },
		{
			'target_name': 'libdebug',
	        'type': 'static_library',
	        'sources': [
				'deps/asm-ref/libs/debug/debug.c'
	        ],
	        "conditions": [
				[ "OS=='win'", {
					"defines": [
						"DEBUG_STATIC"
					]
				}]
			]
	    },
		{
			'target_name': 'libjstring',
	        'type': 'static_library',
	        'sources': [
				'deps/asm-ref/libs/jstring/jstring.c'
	        ],
	        "dependencies": [
				"libdebug"
	        ],
	        "include_dirs": [
	        	"deps/asm-ref/libs/debug"
	        ],
	        "conditions": [
				[ "OS=='win'", {
					"defines": [
						"JSTRING_API_STATIC"
					]
				}]
			]
	    },
		{
			'target_name': 'libsqlite-helper',
	        'type': 'static_library',
	        'sources': [
				'deps/asm-ref/libs/sqlite-helper/sqlite_helper.c'
	        ],
	        "dependencies": [
				"libdebug",
				"libjstring"
	        ],
	        "include_dirs": [
	        	"deps/asm-ref/libs/debug",
	        	"deps/asm-ref/libs/jstring"
	        ],
	        "conditions": [
				[ "OS=='win'", {
					"defines": [
						"SQLITE_HELPER_API_STATIC"
					]
				}]
			]
	    }
	]
}