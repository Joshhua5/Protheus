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
		public: 
			std::tuple<typename vector<Components>::iterator...> storage;

			friend class Entity; 
			
			template<typename T>
			constexpr void next() { Get<T>()++; }

			template<typename T, typename... Types>
			constexpr void next() { Get<T>()++; next<Types>(); }

		public: 
			//ComponentIterator() : storage(GetNull<Components>()...) {}
			//ComponentIterator(Components... args) : storage(args...) {}
			ComponentIterator(typename vector<Components>::iterator... args) : storage(args) {}
		  
			template<typename Component>
			constexpr typename vector<Component>::iterator& Get() {
				return std::get<typename vector<Component>::iterator>(storage);
			} 

			constexpr std::tuple<Components...>& Next() {
				next<Components>();
			}
		}; 
	}
}