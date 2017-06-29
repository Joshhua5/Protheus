/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 02:03:2016 Waring J.

*************************************************************************/
#pragma once
 
#include "EventListener.h"
#include <Parallel.h>
#include <Pipe.h>
#include <vector>
#include <algorithm>
#include <map>

namespace Pro {
	namespace Events { 
		using namespace std;

		class EventManager {
			//! Internally all string values are hashed with std::hash<std::string>
			map<size_t, vector<EventListener>> _listeners;
			atomic<unsigned> _handle_count;

		public:
			EventManager();
			~EventManager();

			EventManager(const EventManager&) = delete;
			EventManager(EventManager&&) = delete;

			void RegisterEvent(const Event& evnt, const string& channel_name);
			void RegisterEvent(const Event& evnt, const size_t hashed_channel_name); 
			void RegisterListener(EventListener& listener, const string& channel_name);
			void RegisterListener(EventListener& listener, const size_t hashed_channel_name);

			//! Does a slow search through every channel to find a listener
			EventListener* RetrieveListener(const unsigned handle);

			//! Does a search through a single channel for a listener
			EventListener* RetrieveListener(const unsigned handle, const std::string& channel_name);
			//! Does a search through a single channel for a listener
			EventListener* RetrieveListener(const unsigned handle, const size_t hashed_channel_name); 
		};

		EventManager& GlobalEventManager() {
			static EventManager manager;
			return manager;
		}
	}
}