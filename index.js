var path      = require('path');
var X86AsmRef = require('./build/Release/x86-asm-ref');

function x86AsmRefFactory(refDbPath) {
	if (refDbPath === undefined) {
		refDbPath = path.join(__dirname, 'x86-ref.sqlite');
	}

	return new X86AsmRef(refDbPath);
}

module.exports = x86AsmRefFactory;
