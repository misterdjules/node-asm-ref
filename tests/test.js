var X86AsmRef = require('../build/Release/x86-asm-ref');
var util      = require('util');

var officialX86Ref = new X86AsmRef('x86-ref.sqlite');

officialX86Ref.getAllInstructions(function (err, instructions) {
	if (err) {
		console.log('Couldn\'t get all instructions from database.');
	} else {
		console.dir(instructions);
		instructions.forEach(function (instruction) {
			console.log(instruction);
		});
	}
});

officialX86Ref = null;
global.gc();


