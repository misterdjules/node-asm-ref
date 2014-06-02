#include <cassert>

#include <node.h>

#include <intel_x86_ref.h>

static ref_database_t* ref_db = NULL;

v8::Handle<v8::Value> getAllInstructions(const v8::Arguments& args)
{
	v8::HandleScope scope;

	assert(ref_db);
	if (!ref_db)
		return scope.Close(v8::Undefined());

	instructions_list_t* instructions_list = x86_ref_get_all_instructions(ref_db);
	if (!instructions_list)
		return scope.Close(v8::Undefined());

	v8::Local<v8::Array> array = v8::Array::New(x86_ref_instructions_list_get_size(instructions_list));

	unsigned int currentIndex  = 0;
	instruction_t* instruction = NULL;
	while ((instruction = x86_ref_next_instruction_from_list(&instructions_list)) != NULL)
	{
		v8::Local<v8::String> instructionMnemonic = v8::String::New(x86_ref_get_instruction_mnemonic(instruction));
		array->Set(currentIndex, instructionMnemonic);
		++currentIndex;
	}

	return scope.Close(array);
}

void Init(v8::Handle<v8::Object> exports)
{
	exports->Set(v8::String::NewSymbol("getAllInstructions"),
		v8::FunctionTemplate::New(getAllInstructions)->GetFunction());

	ref_db = x86_ref_open_database("x86-ref.sqlite");
	assert(ref_db);
	if (!ref_db)
	{
		v8::ThrowException(v8::Exception::Error(v8::String::New("Couldn't find ref database at x86-ref.sqlite")));
	}
}

NODE_MODULE(x86_asm_ref, Init)