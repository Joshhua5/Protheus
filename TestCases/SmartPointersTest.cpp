#include <iostream>
#include <smart_ptr.h>
#include <Log.h>
 
using namespace Pro;
using namespace Util;

class Test {
public:
	Test() {
		global_log.Report<LogCode::INFO>("Constructed", __FUNCTION__, __LINE__);
	}	
	
	Test(const Test& test) {
		global_log.Report<LogCode::INFO>("Copied", __FUNCTION__, __LINE__);
	}	
	
	Test(Test&& test) {
		global_log.Report<LogCode::INFO>("Moved", __FUNCTION__, __LINE__);
	}

	~Test() {
		global_log.Report<LogCode::INFO>("Deconstructed", __FUNCTION__, __LINE__);
	}

};

int main() {
	unsigned char exit_status = 0;
	global_log.EchoOnConsole(true);
	{
		smart_ptr<Test> original(new Test()); 
		if (original.references() != 1){
			global_log.Report<LogCode::INFO>("Incorrect reference count" + original.references(), __FUNCTION__, __LINE__);
			exit_status |= 1;
		}
		
		smart_ptr<Test> copy = original; 
		if (copy.references() != 2){
			global_log.Report<LogCode::INFO>("Incorrect reference count" + copy.references(), __FUNCTION__, __LINE__);
			exit_status |= 2;
		}
		
		smart_ptr<Test> original_m = std::move(original);
		if (copy.references() != 2){
			global_log.Report<LogCode::INFO>("Incorrect reference count" + copy.references(), __FUNCTION__, __LINE__);
			exit_status |= 4;
		}
		
		original.Dereference(); 
		if (copy.references() != 2){
			global_log.Report<LogCode::INFO>("Incorrect reference count" + copy.references(), __FUNCTION__, __LINE__);
			exit_status |= 8;
		}
		
		original_m.Dereference(); 
		if (copy.references() != 1){
			global_log.Report<LogCode::INFO>("Incorrect reference count" + copy.references() , __FUNCTION__, __LINE__);
			exit_status |= 16;
		}
		
		if(!original.IsNull()){
			global_log.Report<LogCode::INFO>("original ptr should be null", __FUNCTION__, __LINE__);
			exit_status |= 32;
		}

		copy = nullptr; 
	}
	 
	return exit_status;
}