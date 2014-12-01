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

unsigned long Error::reportFatal(const string& msg) {
	static unsigned long ftlNum = 0;
	file_lock.lock();
	if (log.is_open() == false)
		return 0;
	log << "FATAL: " << ftlNum << " -- ";	
	log.write(msg.data(), msg.size()) << "\n";
	file_lock.unlock(); 
	return ++ftlNum;
}

unsigned long Error::reportError(const string& msg) {
	static unsigned long errNum = 0;
	file_lock.lock();
	if (log.is_open() == false)
		return 0;
	log << "ERROR: " << errNum << " -- ";	
	log.write(msg.data(), msg.size()) << "\n";
	log.flush(); 
	file_lock.unlock();
	return ++errNum;
}

unsigned long Error::reportMessage(const string& msg) {
	static unsigned long msgNum = 0;
	file_lock.lock();
	if (log.is_open() == false)
		return 0;
	log << "MESSAGE: " << msgNum << " -- ";	
	log.write(msg.data(), msg.size()) << "\n";
	log.flush();
	file_lock.unlock(); 
	return ++msgNum;
}

// NR stands for No Return 
void Error::reportFatalNR(const string& msg) {
	reportError(msg); 
}

// NR stands for No Return
void Error::reportErrorNR(const string& msg) {
	reportError(msg);
}

// NR stands for No Return
void Error::reportMessageNR(const string& msg) {
	reportMessage(msg);
}
