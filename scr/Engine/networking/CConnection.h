/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 07:06:2014: Waring J.
*************************************************************************/
#pragma once

#include <SDL_net.h>
#include <queue>
#include <mutex>
#include <atomic>
#include "..\util\BufferReader.h"
#include "..\util\CBuffer.h"
#include "..\component\CGUID.h"

namespace Pro{
	namespace Networking{
		using namespace std;
		struct CConnection :
			public Component::CGUID
		{
			mutex mutex;
			queue<CBuffer> inputStack;
			queue<CBuffer> outputStack;
			atomic<bool> connected;
		public:
			CConnection(const string& name) : CGUID(name){};
			CConnection() : CGUID("Connection"){}; 

			// returns the amount of bytes recieved
			CBuffer recv();

			// returns the amount of bytes recieved without clearing the buffer
			unsigned int peek();

			// returns true if the connection is active
			bool isConnected();

			// sends the buffer through the connection
			void send(CBuffer& buffer);

			static int lSend(lua_State*);
			static int lPeek(lua_State*);
			static int lRecv(lua_State*);
			static int lIsConnected(lua_State*);
			 
			/*constexpr*/ static const char* lGetMetatable(){
				return "connection_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){ 
				fields.push_back({ "send", &T::lSend });
				fields.push_back({ "peek", &T::lPeek });
				fields.push_back({ "recv", &T::lRecv });
				fields.push_back({ "isConnected", &T::lIsConnected });
			}
		};
	}
}