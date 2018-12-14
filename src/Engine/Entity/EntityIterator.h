#pragma once
#pragma once

#include <unordered_map>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <tuple>

#include <Buffer.h>
#include <BufferReader.h>
#include <LinkedArray.h>
#include <LinkedArrayIterator.h>
#include <BufferWriter.h>
#include <iterator.h>

#include "Component.h"
#include "Components/Enabled.h"

/* TODO, We need a data structure that allows expansion without invalidating the original array
	A linked array would be perfect for this.
*/

namespace Pro {
	using namespace std;
	using namespace Util;
	namespace ECS {
		// To initialize the entity, we can return memory points and the type_info which can be used by the user to initialize, we can also provide a nice function do that 
		// A EntityIterator is used for initializing a entity, it contains pointers to the components for this instance
		  
		template<class... Components>
		class EntityIterator {
			template<class T> using add_iterator = LinkedArrayIterator<T>;

			std::tuple<add_iterator<Components...> storage;  
		public:
			EntityIterator(Components... args) : storage(args...) {}
		  
			template<typename T>
			constexpr LinkedArrayIterator<T>& Get() {  
				return std::get<add_iterator<T>>(); 
			}
		}; 
	}
}