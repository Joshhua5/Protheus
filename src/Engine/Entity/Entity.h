#pragma once
 
#include <typeinfo>
#include <typeindex>
 
#include <LinkedArray.h>
#include <LinkedArrayIterator.h>

#include "ComponentIterator.h" 
#include "Components/Enabled.h"

/* TODO, We need a data structure that allows expansion without invalidating the original array
	A linked array would be perfect for this.
*/

namespace Pro {
	using namespace std;
	using namespace Util;
	namespace ECS { 

		// A Entity component is the link between an entity and a component, it's responsible for the management of a components pool
		// and initialization
		template<typename T>
		struct EntityComponent {
			friend class Entity;
			friend class EntityInitializer;

			//typename vector<T> collection;
			//typename vector<T>::iterator iterator;
			 
			size_t count;
			vector<T> components;

			EntityComponent() {
				components.reserve(10000);
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

			inline typename vector<T>::iterator Iterator() {  
				return components.begin();
			}
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

				template<typename T>
				constexpr EntityComponent<T>* Component() {
					return reinterpret_cast<EntityComponent<T>*>(component);
				}
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
				for (auto& entry : components)
					if (entry.index == typeid(T))
						return entry.Component<T>();
				return nullptr;
			}
			
			template<typename T>
			inline typename std::vector<T>::iterator GetComponentIterator() {
				for (auto& entry : components)
					if (entry.index == typeid(T))
						return entry.Component<T>()->Iterator();
						//return LinkedArrayIterator<T>(entry.Component<T>()->components);
				throw "Not Found";
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
				AddComponent<Enabled>([](Enabled* component) { component->enabled = true; });
			}

			template<typename T>
			void AddComponent() {
				// Here we use the known type information to create a function
				// that will allow us to create components without their type known.   
				EntityComponent<T>* component = new EntityComponent<T>();
				for (size_t i = 0; i < instanceCount; ++i)
					component->components.emplace_back(); 

				// Attach the component type to the Entity
				components.emplace_back(type_index(typeid(T)), component,
					[=](void* ec)
					{
						auto component = reinterpret_cast<EntityComponent<T>*>(ec);
						component->components.emplace_back();
					}
				);
			}

			template<typename T, typename ...args>
			void AddComponent(std::function<void(T*)> constructor) {
				// if (_DEBUG && (components.find(type_index(typeid(T))) != components.end())) {
				// 	global_log.Report<LogCode::FAULT>("Duplicate Component: " + entityName, __FUNCTION__, __LINE__);
				// 	return;
				// }
				// Here we use the known type information to create a function
				// that will allow us to create components without their type known.  
				auto component = new EntityComponent<T>();
				for (size_t i = 0; i < instanceCount; ++i) {
					component->components.emplace_back();
					constructor(&component->components.back());
				}

				// Attach the component type to the Entity
				components.emplace_back(type_index(typeid(T)), component,
					[=](void* ec)
					{
						auto component = reinterpret_cast<EntityComponent<T>*>(ec);
						component->components.emplace_back();
						constructor(&component->components.back()); 
					}
				); 
			}

			 
			template<typename... Components>
			ComponentIterator<Components...> Iterator() {
				return ComponentIterator<Components...>({ GetComponentIterator<Components>()... });
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