#pragma once
#include <string>
#include <fstream> 
#include <mutex>
using namespace std;

#define reportMessageNR(msg) _reportMessageNR(msg, __FUNCTION__, __LINE__)
#define reportMessage(msg) _reportMessage(msg, __FUNCTION__, __LINE__)
#define reportErrorNR(msg) _reportErrorNR(msg, __FUNCTION__, __LINE__)
#define reportError(msg) _reportError(msg, __FUNCTION__, __LINE__)
#define reportFatalNR(msg) _reportFatalNR(msg, __FUNCTION__, __LINE__)
#define reportFatal(msg) _reportFatal(msg, __FUNCTION__, __LINE__)


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

	} ;

	static Error error;

	inline Error::Error() {
		file_lock.lock();
		if (!log.is_open())
			log.open("log.txt", std::ios::out | std::ios::binary | std::ios::trunc);
		file_lock.unlock();
	}
	inline Error::~Error() {
		// find out how to close the log when appropriate.
		log.close();
	}

	inline unsigned long Error::_reportFatal(const string& msg, const char* file, const unsigned long line) {
		static unsigned long ftlNum = 0;
		file_lock.lock();
		if (log.is_open() == false)
			return 0;
		log << "FATAL: " << ftlNum << " LINE: " << line << " FUNC " << file << " -- ";
		log.write(msg.data(), msg.size()) << "\n";
		file_lock.unlock();
		return ++ftlNum;
	}

	inline unsigned long Error::_reportError(const string& msg, const char* file, const unsigned long line) {
		static unsigned long errNum = 0;
		file_lock.lock();
		if (log.is_open() == false)
			return 0;
		log << "ERROR: " << errNum << " LINE: " << line << " FUNC " << file << " -- ";
		log.write(msg.data(), msg.size()) << "\n";
		log.flush();
		file_lock.unlock();
		return ++errNum;
	}

	inline unsigned long Error::_reportMessage(const string& msg, const char* file, const unsigned long line) {
		static unsigned long msgNum = 0;
		file_lock.lock();
		if (log.is_open() == false)
			return 0;
		log << "MESSAGE: " << msgNum << " LINE: " << line << " FUNC " << file << " -- ";
		log.write(msg.data(), msg.size()) << "\n";
		log.flush();
		file_lock.unlock();
		return ++msgNum;
	}

	// NR stands for No Return 
	inline void Error::_reportFatalNR(const string& msg, const char* file, const unsigned long line) {
		_reportError(msg, file, line);
	}

	// NR stands for No Return
	inline void Error::_reportErrorNR(const string& msg, const char* file, const unsigned long line) {
		_reportError(msg, file, line);
	}

	// NR stands for No Return
	inline void Error::_reportMessageNR(const string& msg, const char* file, const unsigned long line) {
		_reportMessage(msg, file, line);
	}

}

