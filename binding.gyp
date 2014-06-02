{
	"targets": [
		{
			"target_name": "x86-asm-ref",
			"sources": [ "x86-asm-ref.cc" ],
			"include_dirs": [
				"../../libs/intel-x86-ref"
			],
			"link_settings": {
				"libraries": [
					"-Wl,--rpath,.",
					"-lx86ref"
				],
				"library_dirs": [
					"../../../libs/intel-x86-ref"
				]
			}
		}
	]
}