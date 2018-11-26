#pragma once

#include <vector> 
#include <map>
#include <eh.h>
#include <Buffer.h>
#include <BufferReader.h>

#include "ComponentData.h"
#include "Component.h"
#include "TransformComponent.h"


using namespace std;
using namespace Pro::Util;

namespace Pro {
	namespace Entity {
		class ComponentStorage
		{ 
			map<unsigned, Buffer> _storage;

		public:   
			template<typename Component> 
			Buffer GetComponentData() {
				const unsigned UUID = Component::UUID();
				Buffer data = _storage.find(UUID);
				 
				if (data == _storage.end())
					_storage.insert(new Buffer());
				else
					return data; 
				return _storage.find(UUID);
			}

			template<typename T>
			void AddComponent(const ComponentData& data) { 
 
				GetComponentData<T>().push_back(move(data));
			}

			template<typename Component, typename ComponentData>
			BufferReader GetComponentReader() {
				const unsigned UUID = T::UUID();

				auto buffer = GetComponentData<Component>();
				auto reader = new BufferReader(buffer);
				return reader; 
			}
		};
	}
}