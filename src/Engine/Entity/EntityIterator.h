#pragma once
#pragma once

#include <unordered_map>
#include <map>
#include <typeinfo>
#include <typeindex>

#include <LinkedArrayRaw.h> 
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
		 
		// Still working out how to get this to work efficiently.

		// class EntityIterator {
		// 	// We should pool this object
		// 	std::vector<pair<type_index, LinkedArrayIterator<Component>*>> component_iterators;
		// 	friend class Entity;
		// 
		// 	void AttachComponent(pair<type_index, LinkedArrayIterator<Component>*> component) {
		// 
		// 	}
		// 
		// public:
		// 	EntityIterator() = default;
		// 
		// 	template<typename T>
		// 	inline bool Contains() {
		// 		return component_instances.find(typeid(T)) != component_instances.end();
		// 	}
		// 
		// 	template<typename T>
		// 	inline T* Get() {   
		// 		return reinterpret_cast<T*>(component_iterators.at(typeid(T)));
		// 	}
		// }; 
	}
}