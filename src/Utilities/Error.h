#pragma once
#include <string>
#include <fstream> 
#include <mutex>
using namespace std;

/*! Injects function name and source line into error message */
#define reportMessageNR(msg) _reportMessageNR(msg, __FUNCTION__, __LINE__)
#define reportMessage(msg) _reportMessage(msg, __FUNCTION__, __LINE__)
#define reportErrorNR(msg) _reportErrorNR(msg, __FUNCTION__, __LINE__)
#define reportError(msg) _reportError(msg, __FUNCTION__, __LINE__)
#define reportFatalNR(msg) _reportFatalNR(msg, __FUNCTION__, __LINE__)
#define reportFatal(msg) _reportFatal(msg, __FUNCTION__, __LINE__)
/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/

namespace Pro {
	class Error {
		// Static so that multiple Error's will write to the same file.
		fstream log;
		mutex file_lock;

		// Declared to be uncopyable and moveable.
		Error(const Error&) = delete;
		Error(Error&&) = delete;
		Error& operator=(Error&&) = delete;
		Error& operator=(const Error&) = delete;
	public:
		Error() {
			file_lock.lock();
			if (!log.is_open())
				log.open("log.txt", std::ios::out | std::ios::binary | std::ios::trunc);
			file_lock.unlock();
		}
		~Error() {
			// find out how to close the log when appropriate.
			log.close();
		}


		/*! Writes a fatal error to the log file and returns the index of the error */
		inline unsigned long _reportFatal(const string& msg, const char* file = "", const unsigned long line = 0) {
			static unsigned long ftlNum = 0;
			file_lock.lock();
			if (log.is_open() == false)
				return 0;
			log << "FATAL: " << ftlNum << " LINE: " << line << " FUNC " << file << " -- ";
			log.write(msg.data(), msg.size()) << "\n";
			log.flush();
			file_lock.unlock();
			return ++ftlNum;
		}

		/*! Writes a error to the log file and returns the index of the error */
		inline unsigned long _reportError(const string& msg, const char* file = "", const unsigned long line = 0) {
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

		/*! Writes a message to the log file and returns the index of the message */
		inline unsigned long _reportMessage(const string& msg, const char* file = "", const unsigned long line = 0) {
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

		/*! Writes a fatal error to the log file*/
		inline void _reportFatalNR(const string& msg, const char* file = "", const unsigned long line = 0) {
			_reportError(msg, file, line);
		}

		/*! Writes a error to the log file*/
		inline void _reportErrorNR(const string& msg, const char* file = "", const unsigned long line = 0) {
			_reportError(msg, file, line);
		}

		/*! Writes a message to the log file*/
		inline void _reportMessageNR(const string& msg, const char* file = "", const unsigned long line = 0) {
			_reportMessage(msg, file, line);
		}

	};

	static Error error;
}

