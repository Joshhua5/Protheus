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
#include "CBuffer.h"
#include "CGUID.h"
#include <vector>
#include <mutex>


namespace Pro{
	namespace Networking{
		struct CConnection : 
			public Component::CGUID
		{ 
			std::mutex mutex;
			std::stack<CBuffer> inputStack;
			std::stack<CBuffer> outputStack; 
			bool connected;

		public:
			CConnection(const string& name) : CGUID(name){
				connected = false;
			};
			CConnection() : CGUID("Connection"){
				connected = false;
			}; 

			~CConnection(){ connected = false; };


		}; 
	}
}