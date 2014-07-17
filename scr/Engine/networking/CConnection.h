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
#include <stack>
#include <mutex>
#include "..\util\BufferReader.h"
#include "..\util\CBuffer.h"
#include "..\component\CGUID.h"

namespace Pro{
	namespace Networking{
		struct CConnection :
			public Component::CGUID
		{
			std::mutex mutex;
			std::stack<CBuffer> inputStack;
			std::stack<CBuffer> outputStack;

		public:
			CConnection(const string& name) : CGUID(name){};
			CConnection() : CGUID("Connection"){}; 

			// returns the amount of bytes recieved
			CBuffer recv();

			// returns the amount of bytes recieved without clearing the buffer
			unsigned int peek();

			// sends the buffer through the connection
			void send(CBuffer& buffer);

			static int lSend(lua_State*);
			static int lPeek(lua_State*);
			static int lRecv(lua_State*);
			 
			static inline string lGetMetatable(){
				return "connection_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "send", &T::lSend });
				fields.push_back({ "peek", &T::lPeek });
				fields.push_back({ "recv", &T::lRecv });
			}
		};
	}
}