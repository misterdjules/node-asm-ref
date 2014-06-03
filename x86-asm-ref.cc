#include <cassert>

#include <node.h>

#include <intel_x86_ref.h>

typedef struct GetInstructionsListBaton
{
	v8::Persistent<v8::Function>	callback;
	const ref_database_t*			ref_db;
	instructions_list_t*			instructions;
} GetInstructionsListBaton;

void GetInstructionsList(uv_work_t* req)
{
	assert(req);
	assert(req->data);

	if (!req || !req->data)
		return;

	GetInstructionsListBaton* instructionsListBaton = reinterpret_cast<GetInstructionsListBaton*>(req->data);

	instructionsListBaton->ref_db = x86_ref_open_database("x86-ref.sqlite");
	assert(instructionsListBaton->ref_db);
	if (!instructionsListBaton->ref_db)
		return;

	instructionsListBaton->instructions = x86_ref_get_all_instructions(instructionsListBaton->ref_db);
}

void GetInstructionsListDone(uv_work_t* req)
{
	v8::HandleScope scope;

	GetInstructionsListBaton* baton = reinterpret_cast<GetInstructionsListBaton*>(req->data);
	v8::Persistent<v8::Array> array;
	instructions_list_t* instructionsList = baton->instructions;
	if (instructionsList)
	{
		array = v8::Persistent<v8::Array>::New(v8::Array::New(x86_ref_instructions_list_get_size(instructionsList)));

		unsigned int currentIndex  = 0;
		instruction_t* instruction = NULL;
		while ((instruction = x86_ref_next_instruction_from_list(&instructionsList)) != NULL)
		{
			v8::Local<v8::String> instructionMnemonic = v8::String::New(x86_ref_get_instruction_mnemonic(instruction));
			array->Set(currentIndex, instructionMnemonic);
			++currentIndex;
		}
	}

	v8::Handle<v8::Value> argv[] = { v8::Null(), array };
	const unsigned int argc = 2;

	baton->callback->Call(v8::Context::GetCurrent()->Global(), argc, argv);
	baton->callback.Dispose();

	delete baton;
	delete req;
}

v8::Handle<v8::Value> GetAllInstructions(const v8::Arguments& args)
{
	v8::HandleScope scope;

	assert(args[0]->IsFunction());
	if (!args[0]->IsFunction())
	{
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("First argument must be a callback function.")));
	}

	uv_work_t* req = new uv_work_t;
	GetInstructionsListBaton* instructionsListBaton = new GetInstructionsListBaton;
	req->data = instructionsListBaton;

	v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[0]);
	instructionsListBaton->callback = v8::Persistent<v8::Function>::New(callback);

	uv_queue_work(uv_default_loop(),
				  req, GetInstructionsList,
				  reinterpret_cast<uv_after_work_cb>(GetInstructionsListDone));

	if (!instructionsListBaton->ref_db)
	{
		return v8::ThrowException(v8::Exception::Error(v8::String::New("Couldn't open database.")));
	}

	return scope.Close(v8::Undefined());
}

void Init(v8::Handle<v8::Object> exports)
{
	exports->Set(v8::String::NewSymbol("getAllInstructions"),
		v8::FunctionTemplate::New(GetAllInstructions)->GetFunction());
}

NODE_MODULE(x86_asm_ref, Init)