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
			ServerTCPConnection(connection_id);

			void startCommunication();
			 
		};
	}
}