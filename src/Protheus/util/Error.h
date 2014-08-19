#pragma once
#include <string>
#include <fstream> 
using namespace std;

namespace Pro{

	static class Error{
		std::fstream log; 
	public:
		Error(){ log.open("log.txt", std::ios::out | std::ios::binary | std::ios::trunc); }
		~Error(){ log.close(); }

		unsigned long reportFatal(const string& msg){
			static unsigned long errNum = 0;
			if (log.is_open() == false)
				return 0;
			log << "START FATAL ";
			log << errNum;
			log << "\n";
			log.write(msg.data(), msg.size()); 
			log << "\nEND FATAL\n\n"; 
			log.flush();
			++errNum;
			return errNum;
		}
		 
		unsigned long reportError(const string& msg){
			static unsigned long errNum = 0;
			if (log.is_open() == false)
				return 0;
			log << "START ERROR ";
			log << errNum;
			log << "\n";
			log.write(msg.data(), msg.size()); 
			log << "\nEND ERROR\n\n"; 
			++errNum;
			log.flush();
			return errNum;
		}

		unsigned long reportMessage(const string& msg){
			static unsigned long msgNum = 0;
			if (log.is_open() == false)
				return 0;
			log << "START MESSAGE ";
			log << msgNum;
			log << "\n";
			log.write(msg.data(), msg.size()); 
			log << "\nEND MESSAGE\n\n"; 
			log.flush();
			++msgNum;
			return msgNum;
		} 
	} error; 
}

