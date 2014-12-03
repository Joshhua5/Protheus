/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 09:07:2014 Waring J.
*************************************************************************/
#pragma once

#include <string>
#include "Member.h"
#include <vector>

namespace Pro{
	namespace Serializer{
		using namespace std;
		 
		class classDefinition
		{
			void* base_pointer;
			vector<Member> members;
		public:
			classDefinition(void* class_pointer); 

			// Adds a member to the definition
			void addMember(const string& memberName,const void* member_pointer,const size_t member_size);

			// returns a vector of all members
			const vector<Member>& getMembers() const;
			
			// returns the sizeof all members
			const unsigned getSizeOf() const;

			// returns the internal base pointer
			// use for offsets
			const void* getBase() const;

			// deletes the unused data
			// no new members can be added afterwards
			// and the passed pointer is deleted
			void finish();
		};
	}
}
