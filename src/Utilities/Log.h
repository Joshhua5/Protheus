#pragma once
#include "mcmp_queue.h"
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <thread>
#include <mutex>

/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
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
		FAULT,
		WARNING,
		PERFORMANCE,
        NETWORK,
        AUDIO,
		INFO
	};

    /*!
     The Log class is for writing out information in XML format,
     all file io's are performed in a seperate thread.

     The constant LOGNAME is used to set the global_log's output file,
     if undefined then the default name Log.xml is used
     */
	class Log {
		struct MessagePack {
			LogCode code;
            unsigned id;
			int line;
			const char* function;
			std::string message;
		};

        std::mutex mutex;
        std::condition_variable cv;
		std::atomic<bool> running_;
		std::atomic<bool> has_terminated_;
		std::atomic<bool> echo_;
		Util::mcmp_queue<MessagePack> messages_;

		static inline void remove_all(const char ch, std::string& in){
			auto position = in.find_first_of(ch, 0);
			while (position != std::string::npos){
				in.erase(position, 1);
				position = in.find_first_of(ch, position - 1);
			}
		}

		static void worker_thread(Util::mcmp_queue<MessagePack>* messages,
                                  std::string log_file_name,
                                  std::atomic<bool>* running,
                                  std::atomic<bool>* terminated,
                                  std::atomic<bool>* echo,
                                  std::mutex* mutex,
                                  std::condition_variable* cv) {
			*terminated = false;
            std::fstream log(log_file_name, std::ios::out | std::ios::binary | std::ios::trunc);
			if (!log.is_open()){
				*terminated = true;
				return;
			}

			log.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n", 40);
			log.write("<Log>\r\n", 7);

			// Don't close until all messages have been written
			while (*running) {
                // Reduce the atomic loads by only checking after a pause in messages
                std::unique_lock<std::mutex> lock(*mutex);
                cv->wait_for(lock, std::chrono::seconds(5));
                
				bool echo_enabled = echo->load();
                MessagePack top; 
				while (messages->Pop(top)) {
					// Check that the Message is valid
					if (top.function == nullptr)
						continue;
					 
					std::string strCache;
					strCache.reserve(40);
					log.write("<entry>\r\n", 9);

					strCache = "<id>" + std::to_string(top.id) + "</id>\r\n";
					log.write(strCache.data(), strCache.size());

					switch (top.code) {
					case LogCode::FAULT:
						log.write("<severity> error </severity>\r\n", 30);
						break;
					case LogCode::FATAL:
						log.write("<severity> fatal </severity>\r\n", 30);
						break;
					case LogCode::MESSAGE:
						log.write("<severity> message </severity>\r\n", 32);
						break;
					case LogCode::INFO:
						log.write("<severity> info </severity>\r\n", 32);
						break;
					case LogCode::PERFORMANCE:
						log.write("<severity> performance </severity>\r\n", 36);
						break;
                    case LogCode::NETWORK:
                        log.write("<severity> network </severity>\r\n", 32);
                        break;
					case LogCode::WARNING:
						log.write("<severity> warning </severity>\r\n", 32);
						break;
                    case LogCode::AUDIO:
                        log.write("<severity> audio </severity>\r\n", 32);
                        break;
					default:
						// Assume message is broken
						log.write("</entry>\r\n", 10);
						top.line = -1;
					}

					// Break out of broken message
					if (top.line < 0)
						break;

					strCache = "<line>" + std::to_string(top.line) + "</line>\r\n";
					log.write(strCache.data(), strCache.size());

					// Perform a nullptr check on function
					strCache = "<function>" + ((top.function != nullptr) ? std::string(top.function) : "Invalid") + "</function>\r\n";
					log.write(strCache.data(), strCache.size());

					// top.message can contain data which will break the XML format
					// Search for either < or > to remove
					//for (auto position = top.message.find_first_of('<', 0); position != std::string::npos; position = top.message.find_first_of('<', 0));

					remove_all('>', top.message);
					remove_all('<', top.message);

					strCache = "<message>" + top.message + "</message>\r\n";
					log.write(strCache.data(), strCache.size());

					log.write("</entry>\r\n", 10);


					if (echo_enabled) {
						auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
						std::cout << "[" << std::to_string(time.count()) << "] " <<
							top.message << " Line: " << top.line << " Function: " << top.function << std::endl;
					}
				}
                log.flush();
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
		//! Reserved log files:
		//! Log.xml
		//! Profile.xml
		Log(std::string log_name) : messages_(2048, std::thread::hardware_concurrency() * 4){
            echo_ = false;
			running_ = true;
			std::thread(&worker_thread, &messages_, log_name, &running_, &has_terminated_, &echo_, &mutex, &cv).detach();
		}

		~Log() {
			Close();
		}

		template<LogCode E>
		inline unsigned long Report(const std::string& msg,
			const char* file,
			const unsigned long line) {
			static unsigned num = 0;

			MessagePack pack;
			pack.code = E;
			pack.id = num++;
			pack.line = static_cast<unsigned>(line);
			pack.function = file;
			pack.message = msg; 
			if (has_terminated_ == false)
				messages_.Push(std::move(pack));
			else{
				std::cout << "Log file has closed or failed to open, did you try to create two logs with the same name?" << std::endl;
				std::cout << "Error Message failed to write: " << msg << std::endl;
			}
			return num;
		}

		// TODO Needs System Control
		// Allows control of when to close the log
		// Required currently as there's no way to ensure that all threads terminate correctly
		inline void Close() {
			running_ = false;
			while (has_terminated_ == false) {
				// Wait until thread has closed nicely
				cv.notify_all();
				std::this_thread::sleep_for(std::chrono::milliseconds(64));
			}
		}
        
        //! Forces the worker to print to the console
        //! and write to the file
        inline void Flush(){
            cv.notify_one();
        }

        //! If @enabled is true then all called to Report() will print the message to console
        inline void EchoOnConsole(const bool enabled){
            echo_ = enabled;
        }
	};
 
    extern Log global_log;

}
