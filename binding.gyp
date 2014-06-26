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
              "libx86-ref"
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
                "deps/libx86-ref"
            ],
        },
        {
            'target_name': 'libx86-ref',
            'type': 'static_library',
            'sources': [
                'deps/libx86-ref/intel_x86_ref.c'
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
                "libsql-query-generator"
            ],
            "include_dirs": [
                "deps/libx86-ref/deps/debug",
                "deps/libx86-ref/deps/jstring",
                "deps/libx86-ref/deps/sql-query-generator"
            ]
        },
        {
            'target_name': 'libdebug',
            'type': 'static_library',
            'sources': [
                'deps/libx86-ref/deps/debug/debug.c'
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
                'deps/libx86-ref/deps/jstring/jstring.c'
            ],
            "dependencies": [
                "libdebug"
            ],
            "include_dirs": [
                "deps/libx86-ref/deps/debug"
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
            'target_name': 'libsql-query-generator',
            'type': 'static_library',
            'sources': [
                'deps/libx86-ref/deps/sql-query-generator/sqlite_helper.c'
            ],
            "dependencies": [
                "libdebug",
                "libjstring"
            ],
            "include_dirs": [
                "deps/libx86-ref/deps/debug",
                "deps/libx86-ref/deps/jstring"
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