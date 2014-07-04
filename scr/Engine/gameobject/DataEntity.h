/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "..\Components.h"
#include <unordered_map>

namespace Pro{
	namespace GameObject{
		using namespace Component;
		class DataEntity 
		{
			unordered_map<string, void*> data;
		public:
			DataEntity();

			void addData(pair<string, void*>& data);
			void* getData(string);
		};
	}
}

