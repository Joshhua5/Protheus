#include "Error.h"

using namespace Pro; 

Pro::Error Pro::error; 

Error::Error() { 
	file_lock.lock();
	if (!log.is_open())
		log.open("log.txt", std::ios::out | std::ios::binary | std::ios::trunc);
	file_lock.unlock();
}
Error::~Error() {
	// find out how to close the log when appropriate.
	log.close();
}

unsigned long Error::_reportFatal(const string& msg, const char* file, const unsigned long line) {
	static unsigned long ftlNum = 0;
	file_lock.lock();
	if (log.is_open() == false)
		return 0;
	log << "FATAL: " << ftlNum << " LINE: " << line << " FUNC " << file << " -- ";
	log.write(msg.data(), msg.size()) << "\n";
	file_lock.unlock(); 
	return ++ftlNum;
}

unsigned long Error::_reportError(const string& msg, const char* file, const unsigned long line) {
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

unsigned long Error::_reportMessage(const string& msg, const char* file, const unsigned long line) {
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
void Error::_reportFatalNR(const string& msg, const char* file , const unsigned long line) {
	_reportError(msg, file, line); 
}

// NR stands for No Return
void Error::_reportErrorNR(const string& msg, const char* file, const unsigned long line) {
	_reportError(msg, file, line);
}

// NR stands for No Return
void Error::_reportMessageNR(const string& msg, const char* file, const unsigned long line) {
	_reportMessage(msg, file, line);
}
