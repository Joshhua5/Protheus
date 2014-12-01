#pragma once
#include <string>
#include <fstream> 
#include <mutex>
using namespace std;

namespace Pro{

	class Error{
		// Static so that multiple Error's will write to the same file.
		 fstream log;
		mutex file_lock;


		// Declared to be uncopyable and moveable.
		Error(const Error&);
		Error(Error&&);
		/*Error& operator=(Error&&);
		Error& operator=(const Error&);*/
	public:
		Error();
		~Error();

		unsigned long reportFatal(const string& msg);

		unsigned long reportError(const string& msg);

		unsigned long reportMessage(const string& msg);

		// NR stands for No Return 
		void reportFatalNR(const string& msg);

		// NR stands for No Return
		void reportErrorNR(const string& msg);

		// NR stands for No Return
		void reportMessageNR(const string& msg);

	}; 

	extern Error error;
}

