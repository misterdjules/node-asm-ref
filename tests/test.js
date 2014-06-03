var x86AsmRef = require('../build/Debug/x86-asm-ref');

x86AsmRef.getAllInstructions(function (err, instructions) {
	console.dir(instructions);
	instructions.forEach(function (instruction) {
		console.log(instruction);
	});
});
