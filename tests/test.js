var x86AsmRef = require('../build/Debug/x86-asm-ref');

var x86Instructions = x86AsmRef.getAllInstructions();
console.dir(x86Instructions);
x86Instructions.forEach(function (instruction) {
	console.log(instruction);
	/*
	console.log("Mnemonic: ", instruction.mnemonic);
	console.log("Short desc: ", instruction.shortDesc);
	console.log("Long desc: ", instruction.longDesc);
	*/
})

var x86Add = x86AsmRef.getInstructionByMnemonoc("ADD");
console.log("Mnemonic: ", x86Add.mnemonic);
console.log("Short desc: ", x86Add.shortDesc);
console.log("Long desc: ", x86Add.longDesc);