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
		struct ClientTCPConnection :
			public TCPConnection
		{
		public:
			ClientTCPConnection();
			ClientTCPConnection(const string& name);
			~ClientTCPConnection(){};
		};
	}
}