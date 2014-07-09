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
#include <vector>

namespace Pro{
	namespace Serializer{
		using namespace std;

		struct Member{
			union{
				// used in writing
				unsigned offset;
				// used in reading
				void* data;
			};
			unsigned size;
			string name;
		};

		class ClassDefinition
		{
			void* base_pointer;
			vector<Member> members;
		public:
			ClassDefinition(void* class_pointer);
			~ClassDefinition();

			void addMember(const string& memberName, void* member_pointer, size_t member_size);

			vector<Member>& getMembers();
		};
	}
}
