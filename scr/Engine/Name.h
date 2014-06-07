/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include <string>
namespace Pro{
	namespace Component{
		class Name
		{
			std::string name;
		public:
			void setName(const std::string& _name);
			const std::string getName();

			// returns the Metatable assosiated with this object
			static inline string lGetMetatable(){
				return "componenet_name_metatable";
			}
		};
	}
}

