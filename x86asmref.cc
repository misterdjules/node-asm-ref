#define BUILDING_NODE_EXTENSION

#include <iostream>
#include <cstring>

#include <node.h>

#include <intel_x86_ref.h>

#include "x86asmref.h"


X86AsmRef::X86AsmRef(const std::string& dbPath)
	: m_dbPath(dbPath), m_refDb(0)
{
}

X86AsmRef::~X86AsmRef()
{
	if (m_refDb)
	{
		x86_ref_close_database(&m_refDb);
	}
}

v8::Handle<v8::Value> X86AsmRef::New(const v8::Arguments& args)
{
	v8::HandleScope scope;

	v8::Local<v8::String> dbPath = args[0]->ToString();
	v8::String::Utf8Value utf8DbPath(dbPath);
	X86AsmRef* refObj = new X86AsmRef(std::string(*utf8DbPath, utf8DbPath.length()));
	refObj->Wrap(args.This());
	return args.This();
}

typedef struct GetInstructionsListBaton
{
	X86AsmRef* 						x86AsmRef;
	v8::Persistent<v8::Function>	callback;
	instructions_list_t*			instructions;
	virtual ~GetInstructionsListBaton();
} GetInstructionsListBaton;

GetInstructionsListBaton::~GetInstructionsListBaton()
{
	if (this->instructions)
		x86_ref_destroy_instructions_list(this->instructions);
	this->instructions = NULL;
}

typedef struct SearchByMnemonicBaton : GetInstructionsListBaton
{
	std::string prefix;
} SearchByMnemonicBaton;

v8::Handle<v8::Value> X86AsmRef::GetAllInstructions(const v8::Arguments& args)
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
	instructionsListBaton->x86AsmRef = ObjectWrap::Unwrap<X86AsmRef>(args.This());

	uv_queue_work(uv_default_loop(),
				  req, GetInstructionsListWorker,
				  reinterpret_cast<uv_after_work_cb>(GetInstructionsListDone));

	return scope.Close(v8::Undefined());
}

v8::Handle<v8::Value> X86AsmRef::SearchInstructionsByMnemonic(const v8::Arguments& args)
{
	v8::HandleScope scope;

	assert(args.Length() == 2);
	if (args.Length() != 2)
	{
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Function requires two parameters.")));
	}

	assert(args[0]->IsString());
	if (!args[0]->IsString())
	{
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("First argument must be a string.")));
	}

	assert(args[1]->IsFunction());
	if (!args[1]->IsFunction())
	{
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Second argument must be a callback function.")));
	}

	uv_work_t* req = new uv_work_t;
	SearchByMnemonicBaton* baton = new SearchByMnemonicBaton;
	req->data = baton;

	v8::String::AsciiValue prefix(args[0]->ToString());
	baton->prefix = std::string(*prefix);

	v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[1]);
	baton->callback = v8::Persistent<v8::Function>::New(callback);

	baton->x86AsmRef = ObjectWrap::Unwrap<X86AsmRef>(args.This());

	uv_queue_work(uv_default_loop(),
				  req,
				  SearchInstructionsByMnemonicWorker,
				  reinterpret_cast<uv_after_work_cb>(GetInstructionsListDone));

	return scope.Close(v8::Undefined());
}

void X86AsmRef::GetInstructionsListWorker(uv_work_t* req)
{
	assert(req);
	assert(req->data);

	if (!req || !req->data)
		return;

	GetInstructionsListBaton* instructionsListBaton = reinterpret_cast<GetInstructionsListBaton*>(req->data);

	X86AsmRef* ref = instructionsListBaton->x86AsmRef;
	assert(ref);
	if (!ref)
		return;

	// retrieve DB ref from cache
	ref_database_t* refDb = ref->GetRefDb();
	if (!refDb)
	{
		refDb = x86_ref_open_database(ref->GetDbPath().c_str());
		assert(refDb);
		if (!refDb)
			return;
		// cache DB reference for subsequent uses
		ref->SetRefDb(refDb);
	}

	instructionsListBaton->instructions = x86_ref_get_all_instructions(refDb);
}

void X86AsmRef::SearchInstructionsByMnemonicWorker(uv_work_t* req)
{
	assert(req);
	assert(req->data);

	if (!req || !req->data)
		return;

	SearchByMnemonicBaton* baton = reinterpret_cast<SearchByMnemonicBaton*>(req->data);

	X86AsmRef* ref = baton->x86AsmRef;
	assert(ref);
	if (!ref)
		return;

	ref_database_t* refDb = ref->GetRefDb();
	if (!refDb)
	{
		refDb = x86_ref_open_database(ref->GetDbPath().c_str());
		assert(refDb);
		if (!refDb)
			return;
		ref->SetRefDb(refDb);
	}

	baton->instructions = x86_ref_search_instructions_by_mnemonic(refDb, baton->prefix.c_str());
}

void X86AsmRef::GetInstructionsListDone(uv_work_t* req)
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
			v8::Local<v8::String> instructionSynopsis = v8::String::New(x86_ref_get_instruction_synopsis(instruction));

			v8::Local<v8::Object> instructionObject = v8::Object::New();
			instructionObject->Set(v8::String::New("mnemonic"), instructionMnemonic);
			instructionObject->Set(v8::String::New("synopsis"), instructionSynopsis);

			array->Set(currentIndex, instructionObject);
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

void X86AsmRef::Init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module)
{
	v8::Local<v8::FunctionTemplate> constructorTpl = v8::FunctionTemplate::New(New);
	constructorTpl->SetClassName(v8::String::NewSymbol("X86AsmRef"));
	constructorTpl->InstanceTemplate()->SetInternalFieldCount(2);

	v8::Local<v8::Function> getAllInstructionFunc = v8::FunctionTemplate::New(GetAllInstructions)->GetFunction();
	constructorTpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getAllInstructions"),
											 getAllInstructionFunc);

	v8::Local<v8::Function> searchInstructionsByMnemonicFunc = v8::FunctionTemplate::New(SearchInstructionsByMnemonic)->GetFunction();
	constructorTpl->PrototypeTemplate()->Set(v8::String::NewSymbol("searchInstructionsByMnemonic"),
											 searchInstructionsByMnemonicFunc);

	module->Set(v8::String::NewSymbol("exports"), constructorTpl->GetFunction());
}
