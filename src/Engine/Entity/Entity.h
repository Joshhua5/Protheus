#pragma once

#include <unordered_map>
#include <map>
#include <typeinfo>
#include <typeindex>

#include <LinkedArrayRaw.h>
#include <Buffer.h>
#include <BufferReader.h>
#include <LinkedArray.h>
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
		class EntityInstance {
			// We should pool this object
			std::map<type_index, void*> component_instances; 
			friend class Entity;

		public:
			template<typename T, typename... args> 
			void Initialize(args...) {
				component_instances.at(typeid(T)) = T(args...);
			}
		};

		// A Entity component is the link between an entity and a component, it's responsible for the management of a components pool
		// and initialization 
		struct EntityComponent {
			type_info* Type;
			size_t sizeOf;
			size_t count;
			LinkedArrayRaw components;
			EntityComponent(size_t _sizeOf, type_info* ID, size_t _capacity = 10) :
				components(_sizeOf) {  
				sizeOf = _sizeOf; 
				count = 0; 
			} 

			friend class Entity;

			// NOTE: if the components are uninitialized from the create function, then
			// should we bother initializing them with the populate? and enforce the component 
			// reaction through systems? We'll see how this pans out, since initialization might require other
			// components, this might be the way to go. Then again, how do you create objects with different variables...
			template<typename T>
			void Create(size_t count) {
				BufferWriter writer(buffer);
				for (unsigned i = 0; i < count; ++i) {
					writer.Write(T()); 
				} 
			}
		}; 

		// A entity is a configuration of components, each entity can have many instances
		// adding a component to a entity will add it to all instances
		// Since we have objects that refer to the entities directly, we will need to ensure that the array is resized at
		// known times. 
		class Entity {
			string entityName;

			//ArrayList<EntityComponent> components; 
			std::unordered_map<std::type_index, EntityComponent> components;
			unsigned instanceCount = 0;
			    
		public:
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
				AddComponent<Enabled>();
			}

			template<typename T>
			void AddComponent() {
				EntityComponent component(sizeof(T), (unsigned)T::ID()); 
				// If we already have instances of this entity then initialize the components
				component.Populate(instanceCount);
				components.push_back(component);
			}

			template<typename T>
			Iterator<T> GetComponentIterator() {
				EntityComponent& ec = components.at(typeid(T));
				Iterator<T> iterator(ec.buffer.data<T>(), ec.count); 
				return iterator;
			} 
			 
			// How do we intent to get properties for the construction in here?
			// Or do I intent to have a function on the system that handles this?
			EntityInstance& CreateWithRef() {
				EntityInstance instance;
				for (std::pair<type_index, EntityComponent> value : components) {
					// Add the component
					// There has to be a better way! 
					// time to use std::Vector
					EntityComponent& buf = value.second; 
					BufferWriter writer(&buf.buffer);
					writer.Skip(buf.count * buf.sizeOf);
					unsigned head = writer.head();
					writer.Write(buf.sizeOf);
					writer.Skip(-buf.sizeOf); 
					 
					instance.component_instances
						.insert(std::pair<type_index, void*>(value.first, buf.buffer.data<char>() + head));
				} 
			} 

			/*! Destory a instance of an entity, this will deactivate the entity and flag it for removal */
			void Destroy() {

			}

		};
	}
}