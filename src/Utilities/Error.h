#pragma once
#include <string>
#include <fstream> 
#include <mutex>
using namespace std;

#define reportMessageNR(msg) _reportMessageNR(msg, __FUNCTION__, __LINE__)
#define reportMessage(msg) _reportMessage(msg, __FUNCTION__, __LINE__)
#define reportErrorNR(msg) _reportMessageNR(msg, __FUNCTION__, __LINE__)
#define reportError(msg) _reportMessage(msg, __FUNCTION__, __LINE__)
#define reportFatalNR(msg) _reportMessageNR(msg, __FUNCTION__, __LINE__)
#define reportFatal(msg) _reportMessage(msg, __FUNCTION__, __LINE__)


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

		unsigned long _reportFatal(const string& msg, const char* file = "", const unsigned long line = 0);

		unsigned long _reportError(const string& msg, const char* file = "", const unsigned long line = 0);

		unsigned long _reportMessage(const string& msg, const char* file = "", const unsigned long line = 0);

		// NR stands for No Return 
		void _reportFatalNR(const string& msg, const char* file = "", const unsigned long line = 0);

		// NR stands for No Return
		void _reportErrorNR(const string& msg, const char* file = "", const unsigned long line = 0);

		// NR stands for No Return
		void _reportMessageNR(const string& msg, const char* file = "", const unsigned long line = 0);

	}; 

	extern Error error;
}

