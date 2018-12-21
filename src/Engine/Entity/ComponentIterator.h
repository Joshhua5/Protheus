#pragma once
#pragma once

#include <unordered_map>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <tuple>
 
#include <LinkedArray.h>
#include <LinkedArrayIterator.h>  
 
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
		  
		template<typename... Components>
		class alignas(16) ComponentIterator {
			template<typename T> constexpr LinkedArrayIterator<T> GetNull() {
				return nullptr;
			}
		
			std::tuple<LinkedArrayIterator<Components>...> storage;

			friend class Entity; 
		public: 
			//ComponentIterator() : storage(GetNull<Components>()...) {}
			ComponentIterator(Components... args) : storage(args...) {}
			ComponentIterator(LinkedArrayIterator<Components>... args) : storage(args...) {}
		  
			template<typename Component>
			constexpr LinkedArrayIterator<Component>& Get() {
				return std::get<LinkedArrayIterator<Component>>(storage);
			}
		}; 
	}
}