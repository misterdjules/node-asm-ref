var x86AsmRef = require('../index.js');
var util      = require('util');
var path	  = require('path');

var officialX86Ref = x86AsmRef();

officialX86Ref.getAllInstructions(function (err, instructions) {
	if (err) {
		console.log('Couldn\'t get all instructions from database.');
	} else {
		console.dir(instructions);
	}
});

officialX86Ref.searchInstructionsByMnemonic("A", function (err, instructions) {
	if (err) {
		console.log("Couldn't search for instructions with prefix 'A'.");
	} else {
		console.dir(instructions);
	}
});

officialX86Ref = null;
global.gc();


