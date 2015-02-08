#pragma once
#include <string>
#include <fstream> 
#include <thread>
#include <mutex>
#include "Queue.h" 

/*! Injects function name and source line into error message */
#define report(code, msg) Report<code>(msg, __FUNCTION__, __LINE__) 
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
	enum struct ErrorCode {
		MESSAGE,
		FATAL,
		ERROR
	};
	class Error {
		struct MessagePack {
			ErrorCode code;
			int id;
			int line;
			const char* function;
			std::string message;
		};

		// Static so that multiple Error's will write to the same file.
		std::thread worker_;
		std::atomic<bool> running_;
		Util::Queue<MessagePack> messages_;

		static void worker_thread(Util::Queue<MessagePack>* messages, std::atomic<bool>* running) {
			std::fstream log;
			if (!log.is_open())
				log.open("log.xml", std::ios::out | std::ios::binary | std::ios::trunc);

			log.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n", 40);
			log.write("<Log>\n", 7);

			while (running->load()) {
				while (!messages->Empty()) {
					MessagePack& top = messages->Top();
					std::string strCache;
					log.write("<entry>\n", 9);

					strCache = "<id>" + std::to_string(top.id) + "</id>\n";
					log.write(strCache.data(), strCache.size());

					switch (top.code) {
					case ErrorCode::ERROR:
						log.write("<severity> error </severity>\n", 30);
						break;
					case ErrorCode::FATAL:
						log.write("<severity> fatal </severity>\n", 30);
						break;
					case ErrorCode::MESSAGE:
						log.write("<severity> message </severity>\n", 32);
						break;
					}

					strCache = "<line>" + std::to_string(top.line) + "</line>\n";
					log.write(strCache.data(), strCache.size());
					 
					strCache = "<function>" + std::string(top.function) + "</function>\n";
					log.write(strCache.data(), strCache.size());
					  
					strCache = "<message>" + top.message + "</message>\n";
					log.write(strCache.data(), strCache.size());

					log.write("</entry>\n", 10);

					messages->Pop();

				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			log.write("</Log>\n", 8);
			log.flush();
			log.close();
		}

		// Declared to be uncopyable and moveable.
		Error(const Error&) = delete;
		Error(Error&&) = delete;
		Error& operator=(Error&&) = delete;
		Error& operator=(const Error&) = delete;
	public:
		Error() {
			running_.store(true);
			messages_.Resize(1000);
			worker_ = std::thread(&worker_thread, &messages_, &running_);
		}
		~Error() {
			running_.store(false);
			worker_.join();
		}

		template<ErrorCode E>
		inline unsigned long Report(const string& msg,
			const char* file,
			const unsigned long line) {
			static size_t num = 0;

			MessagePack pack;
			pack.code = T;
			pack.id = ftlNum;
			pack.line = line;
			pack.function = file;
			pack.message = msg;
			messages_.Push(std::move(pack)); 
		}
	};

	static Error error;
}

