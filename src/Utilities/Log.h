#pragma once
#include <string>
#include <fstream> 
#include <thread>
#include <mutex>
#include "Queue.h" 
 
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
	enum struct LogCode {
		MESSAGE,
		FATAL,
		ERROR
	};
	class Log {
		struct MessagePack {
			LogCode code;
			int id;
			int line;
			const char* function;
			std::string message;
		};

		// Static so that multiple Error's will write to the same file.
		std::thread worker_;
		std::atomic<bool> running_;
		Pro::Util::Queue<MessagePack> messages_;

		static void worker_thread(Pro::Util::Queue<MessagePack>* messages, std::atomic<bool>* running) {
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
					case LogCode::ERROR:
						log.write("<severity> error </severity>\n", 30);
						break;
					case LogCode::FATAL:
						log.write("<severity> fatal </severity>\n", 30);
						break;
					case LogCode::MESSAGE:
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
		Log(const Log&) = delete;
		Log(Log&&) = delete;
		Log& operator=(Log&&) = delete;
		Log& operator=(const Log&) = delete;
	public:
		Log() {
			running_.store(true);
			messages_.Resize(1000);
			worker_ = std::thread(&worker_thread, &messages_, &running_);
		}
		~Log() {
			running_.store(false);
			worker_.join();
		}

		template<LogCode E>
		inline unsigned long Report(const std::string& msg,
			const char* file,
			const unsigned long line) {
			static size_t num = 0;

			MessagePack pack;
			pack.code = E;
			pack.id = num++;
			pack.line = line;
			pack.function = file;
			pack.message = msg;
			messages_.Push(std::move(pack)); 


			return num;
		}
	};

	static Log log;
}

