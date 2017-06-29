/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 09:07:2014 Waring J.
*************************************************************************/
#pragma once

#include <string>
#include "ArrayList.h"
#include "Member.h"

namespace Pro {
	namespace Util {
		namespace Serializer {
			using namespace std;

			/*! Allows a class to be packed for transmission between computers and classes of the same type with different padding */

            
			class alignas(16) ClassDefinition
			{
				void* base_pointer;
                unsigned size_of;
				ArrayList<Member> members_;
                
                ClassDefinition() = delete;
			public:
				//! Pointer isn't deleted unless Finish() is called
				ClassDefinition(void* class_pointer) {
					base_pointer = class_pointer;
                    size_of = 0;
				}
                
                ClassDefinition(ClassDefinition&& rhs){
                    members_ = std::move(rhs.members_);
                    base_pointer = rhs.base_pointer;
                    size_of = rhs.size_of;
                }
                
                ClassDefinition(const ClassDefinition&& rhs){
                    members_ = rhs.members_;
                    base_pointer = rhs.base_pointer;
                    size_of = rhs.size_of;
                }
                
                ClassDefinition& operator=(ClassDefinition&& rhs){
                    members_ = std::move(rhs.members_);
                    base_pointer = rhs.base_pointer;
                    size_of = rhs.size_of;
                    return *this;
                }
                
                ClassDefinition& operator=(const ClassDefinition& rhs){
                    members_ = rhs.members_;
                    base_pointer = rhs.base_pointer;
                    size_of = rhs.size_of;
                    return *this;
                }

				/*! Adds a member to the ClassDefinition */
				void RegisterMember(const string& member_name, const void* member_pointer, const unsigned member_size) {
					members_.emplace_back(member_name,
                            static_cast<unsigned>((char*)member_pointer - (char*)base_pointer),
                            member_size);
                    size_of += member_size;
                     
				}
                
                void RegisterMember(const string& member_name, unsigned offset, const unsigned member_size){
                    size_of += member_size;
                    members_.emplace_back(member_name, offset, member_size);
                }

				/*! Returns the sizeof all registered members */
				const unsigned SizeOf() const {
                    return size_of;
				}
                
                ArrayList<Member>& members(){
                    return members_;
                }
                
                const ArrayList<Member>& members() const{
                    return members_;
                }

				/*! Returns the internal base pointer.
					Used for offsets. */
				const void* base() const {
					return base_pointer;
				}

				void* base() {
					return base_pointer;
				}
                
                template<class T>
                T* base(){
                    return reinterpret_cast<T>(base_pointer);
                }
                
                template<class T>
                const T* base()const {
                    return reinterpret_cast<T>(base_pointer);
                }

                template<class T>
                void Finish(){
                    if(base_pointer != nullptr){
                        delete reinterpret_cast<T>(base_pointer);
                        base_pointer = nullptr;
                    }
                }
            };
		}
	}
}
