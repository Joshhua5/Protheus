#pragma once
#include "Queue.h" 
#include <string>
#include <fstream> 
#include <thread>
#include <mutex>
 
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

		std::atomic<bool> running_;
		std::atomic<bool> has_terminated_;
		static Util::Queue<MessagePack> messages_;

		static void worker_thread(Util::Queue<MessagePack>* messages, std::atomic<bool>* running, std::atomic<bool>* terminated) {
			*terminated = false;
			std::fstream log;
			if (!log.is_open())
				log.open("log.xml", std::ios::out | std::ios::binary | std::ios::trunc);

			log.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n", 40);
			log.write("<Log>\r\n", 7);

			// Don't close until all messages have been written
			while (running->load() || !messages->Empty()) {
				while (!messages->Empty()) {
					MessagePack& top = messages->Top();
					std::string strCache;
					log.write("<entry>\r\n", 9);

					strCache = "<id>" + std::to_string(top.id) + "</id>\r\n";
					log.write(strCache.data(), strCache.size());

					switch (top.code) {
					case LogCode::ERROR:
						log.write("<severity> error </severity>\r\n", 30);
						break;
					case LogCode::FATAL:
						log.write("<severity> fatal </severity>\r\n", 30);
						break;
					case LogCode::MESSAGE:
						log.write("<severity> message </severity>\r\n", 32);
						break;
					}

					strCache = "<line>" + std::to_string(top.line) + "</line>\r\n";
					log.write(strCache.data(), strCache.size());
					 
					strCache = "<function>" + std::string(top.function) + "</function>\r\n";
					log.write(strCache.data(), strCache.size());
					  
					strCache = "<message>" + top.message + "</message>\r\n";
					log.write(strCache.data(), strCache.size());

					log.write("</entry>\r\n", 10);

					messages->Pop();

				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}

			log.write("</Log>\r\n", 8);
			log.flush();
			log.close();
			*terminated = true;
		}

		// Declared to be uncopyable and moveable.
		Log(const Log&) = delete;
		Log(Log&&) = delete;
		Log& operator=(Log&&) = delete;
		Log& operator=(const Log&) = delete;
	public:
		Log() {
			static bool initialized = false;
			if (initialized == false) {
				initialized = true;
				running_.store(true);
				messages_.Resize(1000);
				std::thread(&worker_thread, &messages_, &running_, &has_terminated_).detach();
				
			}
		}
		~Log() {
			running_.store(false);
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

		// TODO Needs System Control
		// Allows control of when to close the log
		// Required currently as there's no way to ensure that all threads terminate correctly
		inline void Close() {
			running_.store(false); 
			while (has_terminated_ == false) {
				// Wait until thread has closed nicely
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		}
	};

	static Log global_log;
}

