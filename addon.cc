#include <cassert>

#include <node.h>

#include <intel_x86_ref.h>

#include "x86asmref.h"

void Init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module)
{
	X86AsmRef::Init(exports, module);
}

NODE_MODULE(x86_asm_ref, Init)