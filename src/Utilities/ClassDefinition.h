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

namespace Pro {
	namespace Util {
		namespace Serializer {
			using namespace std;

			/*! Allows a class to be packed for transmission between computers and classes of the same type with different padding */

			class ClassDefinition
			{
				void* base_pointer;
				vector<Member> members;
			public:
				/*! Must be called with ClassDefinition(new T)*/
				ClassDefinition(void* class_pointer) {
					base_pointer = class_pointer;
				}

				~ClassDefinition() {
					if (base_pointer != nullptr)
						delete base_pointer;
				}

				/*! Adds a member to the ClassDefinition */
				void AddMember(const string& memberName, const void* member_pointer, const size_t member_size) {
					auto m = Member();

					m.name = memberName;
					m.offset = static_cast<unsigned>((char*)member_pointer - (char*)base_pointer);
					m.size = static_cast<unsigned>(member_size);

					members.push_back(move(m));
				}

				/*! Returns a vector of all members */
				const vector<Member>& members() const {
					return members;
				}


				/*! Returns the sizeof all members */
				const unsigned SizeOf() const {
					unsigned sizeTotal = 0;
					for each(const auto m in members)
						sizeTotal += m.size;
					return sizeTotal;
				}

				/*! Returns the internal base pointer.
					Used for offsets. */
				const void* base() const {
					return base_pointer;
				}

				void* base() {
					return base_pointer;
				}

				/*! Call onces finished adding members,
					Deletes the base pointer*/
				void Finish() {
					delete base_pointer;
					base_pointer = nullptr;
				}
			};
		}
	}
}
