/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:12:2014 Waring J.

*************************************************************************/
#pragma once
#include <string>
#include "Member.h"
#include <vector>

namespace Pro{
	namespace Serializer{
		using namespace std;

		/*! Defines a variable inside of a class, used in ClassDefinitions */
		struct Member{
			union{
				/*! The offset from the base pointer*/
				unsigned offset;
				/*! Points to data inside a object being loaded */
				void* data;
			};
			/*! Size of a member's data */
			unsigned size;
			/*! Name of the memeber */
			string name;
		};
	}
}