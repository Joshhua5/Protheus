#pragma once
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

#include "CTCPConnection.h"

using namespace std;

namespace Pro{
	namespace Networking{
		struct ServerTCPConnection :
			public TCPConnection
		{  
		public: 
			ServerTCPConnection(const string& name);

			void startCommunication();

			/*constexpr*/ static const char* lGetMetatable(){
				return "s_tcp_connection_metatable";
			}

			template<typename T>
			static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
				TCPConnection::lGetFunctions<T>(fields);
			}
		};
	}
}