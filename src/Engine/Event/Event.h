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
#include <string>

namespace Pro {
	namespace Events { 
		struct Event {
			size_t Channel;
			void* UserData;

			Event() {}
			Event(const std::string& channel, void* data) {
				Channel = std::hash<std::string>()(channel);
				UserData = data;
			}

			Event(size_t hashed_channel, void* data) { 
				Channel = hashed_channel;
				UserData = data;
			}
		};
	}
}