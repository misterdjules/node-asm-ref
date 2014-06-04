#ifndef X86_ASM_REF_H_
#define X86_ASM_REF_H_

#include <string>

#include <node.h>

typedef struct ref_database_s ref_database_t;
typedef struct uv_work_s uv_work_t;

class X86AsmRef : public node::ObjectWrap
{
public:
	static void Init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module);

	const std::string GetDbPath() const 	{ return m_dbPath; }

	void SetRefDb(ref_database_t* refDb) 	{ m_refDb = refDb; }
	ref_database_t* GetRefDb()  			{ return m_refDb;  }

private:
	explicit X86AsmRef(const std::string& db_path);
	~X86AsmRef();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	static v8::Handle<v8::Value> GetAllInstructions(const v8::Arguments& args);

	static void GetInstructionsListWorker(uv_work_t* req);
	static void GetInstructionsListDone(uv_work_t* req);

	const std::string 	m_dbPath;
	ref_database_t*		m_refDb;
};

#endif /* X86_ASM_REF_H_ */