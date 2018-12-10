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

		// A EntityInstance is used for initializing a entity, it contains pointers to the components for this instance
		// DEPRICATED
		// class EntityReference {
		// 	// We should pool this object
		// 	std::map<type_index, void*> component_instances;
		// 	friend class Entity;
		// 	//friend class FriendSystem;
		// 
		// public:
		// 	EntityInitializer() = default;
		// 
		// 	EntityInitializer(const EntityInitializer&) = delete;
		// 	EntityInitializer& operator=(const EntityInitializer&) = delete;
		// 
		// 	EntityInitializer(EntityInitializer&&) = default;
		// 	EntityInitializer& operator=(EntityInitializer&&) = default;
		// 
		// 	template<typename T, typename... args>
		// 	inline void Initialize(args... arg) {
		// 		*reinterpret_cast<T*>(component_instances.at(typeid(T))) = T(arg...);
		// 	}
		// 
		// 	template<typename T>
		// 	inline bool Contains() {
		// 		return component_instances.find(typeid(T)) != component_instances.end();
		// 	}
		// 
		// 	template<typename T>
		// 	inline T* Get() {
		// 		return reinterpret_cast<T*>(component_instances.at(typeid(T)));
		// 	}
		// };

		// A Entity component is the link between an entity and a component, it's responsible for the management of a components pool
		// and initialization
		template<typename T>
		struct EntityComponent {
			friend class Entity;
			friend class EntityInitializer;

			size_t count;
			LinkedArray<T> components;

			EntityComponent() : components() {
				count = 0;
			}

			EntityComponent(const EntityComponent&) = delete;
			EntityComponent(EntityComponent&&) = default;

			EntityComponent& operator= (const EntityComponent&) = delete;
			EntityComponent& operator= (EntityComponent&&) = default;

			// NOTE: if the components are uninitialized from the create function, then
			// should we bother initializing them with the populate? and enforce the component 
			// reaction through systems? We'll see how this pans out, since initialization might require other
			// components, this might be the way to go. Then again, how do you create objects with different variables...

			//template<typename... args>
			//void Create(size_t count, args... arg) {
			//	for (unsigned i = 0; i < count; ++i)
			//		components.Append<T>(arg...);
			//}

			inline LinkedArrayIterator<T> Iterator() {
				return LinkedArrayIterator<T>(components);
			}
		};

		class EntityReference {


		};

		// A entity is a configuration of components, each entity can have many instances
		// adding a component to a entity will add it to all instances
		// Since we have objects that refer to the entities directly, we will need to ensure that the array is resized at
		// known times. 
		class Entity {
			friend class FactorySystem;

			string entityName;

			// void* is a EntityComponent<T>* where T is the type_index
			// function<void(void*)> is a function that is used to construct the component, it requires a cast from void* to T*
			
			// TODO this should be swapped for a vector, with a simple linear search. since it's faster for this dataset size
			struct ComponentEntry {
				ComponentEntry(type_index idx, void* comp, function<void(void*)> construc) : index(idx){
					component = comp; constructor = construc;
				}
				type_index index;
				void* component; // EntityComponent*
				function<void(void*)> constructor;
			};

			std::vector<ComponentEntry> components;
			size_t instanceCount = 0;

			template<typename T>
			inline ComponentEntry* GetPack() {
				for (size_t i = 0; i < components.size(); ++i)
					if (components[i].index == typeid(T))
						return &components[i];
				return nullptr;
			}

			template<typename T>
			inline EntityComponent<T>* GetComponent() {
				for (size_t i = 0; i < components.size(); ++i)
					if (components[i].index == typeid(T))
						return reinterpret_cast<EntityComponent<T>*>(components[i].component);
				return nullptr;
			}

		public:
			inline size_t Instances() const { return instanceCount; }
			/// Copies the structure of the Entity
			/// Entity(const Entity& copy) {
			/// 
			/// }
			/// 
			/// Entity(Entity&& move) {
			/// 
			/// }

			Entity(const string& name) {
				entityName = name;
				//AddComponent<Enabled>([](void* component) { reinterpret_cast<Enabled*>(component)->enabled = true; });
			}

			template<typename T>
			void AddComponent() {
				// Here we use the known type information to create a function
				// that will allow us to create components without their type known.   
				EntityComponent<T>* component = new EntityComponent<T>();
				for (size_t i = 0; i < instanceCount; ++i)
					component->components.Emplace(); 

				components.emplace_back(type_index(typeid(T)), component,
					[=](void* ec)
					{
						EntityComponent<T>* component = reinterpret_cast<EntityComponent<T>*>(ec);
						component->components.Emplace();
					}
				);
			}

			template<typename T, typename ...args>
			void AddComponent(std::function<void(void*)> constructor) {
				// if (_DEBUG && (components.find(type_index(typeid(T))) != components.end())) {
				// 	global_log.Report<LogCode::FAULT>("Duplicate Component: " + entityName, __FUNCTION__, __LINE__);
				// 	return;
				// }
				// Here we use the known type information to create a function
				// that will allow us to create components without their type known.  
				EntityComponent<T>* component = new EntityComponent<T>();
				for (size_t i = 0; i < instanceCount; ++i)
					constructor((void*)component->components.Append());

				components.emplace_back(type_index(typeid(T)), component,
					[=](void* ec)
					{
						EntityComponent<T>* component = reinterpret_cast<EntityComponent<T>*>(ec);
						constructor((void*)component->components.Append());
					}
				); 
			}

			template<typename T>
			LinkedArrayIterator<T> Iterator() {
				return GetComponent<T>()->Iterator();
			}
			 
			template<typename T>
			inline bool Contains() {
				return GetPack<T>() != nullptr;
			}

			void NewInstance() {
				instanceCount++;
				for (auto& pack : components)
					pack.constructor(pack.component);
			}

			/*! Destory a instance of an entity, this will deactivate the entity and flag it for removal */
			void Destroy() {

			}
		};
	}
}