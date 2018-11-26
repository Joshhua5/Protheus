#pragma once

#include <ArrayList.h>
#include <Buffer.h>
#include <BufferReader.h>
#include <BufferWriter.h>

#include "Component.h"
#include "Components/Enabled.h"
 
using namespace std;
using namespace Pro;
using namespace Util;

namespace Pro {
	namespace ECS {

		struct EntityComponent {
			unsigned componentID; 
			size_t sizeOf;
			size_t count;
			size_t capacity;
			Buffer data;
			EntityComponent() {} 
			EntityComponent(size_t _sizeOf, unsigned ID, size_t _capacity = 10) {
				data.Init(_sizeOf * _capacity);
				componentID = ID;
				sizeOf = _sizeOf;
				capacity = _capacity;
				count = 0;
			} 

			// NOTE: if the components are uninitialized from the create function, then
			// should we bother initializing them with the populate? and enforce the component 
			// reaction through systems? We'll see how this pans out, since initialization might require other
			// components, this might be the way to go. Then again, how do you create objects with different variables...
			template<typename T>
			void Populate(size_t count) {
				BufferWriter writer(data);
				for (unsigned i = 0; i < count; ++i) {
					writer.Write(T()); 
				} 
			}
		};

		class Entity {
			string entityName;

			ArrayList<EntityComponent> components;
			unsigned instanceCount = 0;

			
			inline EntityComponent& GetComponents(unsigned ID){
				for (int i = 0; i < components.size(); ++i)
					if (components[i].componentID == ID)
						return components[i];
			}

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
			BufferReader GetComponentReader() {
				auto components = GetComponents(T::ID());
				BufferReader reader(components.data());
				return reader;
			} 
			 
			// How do we intent to get properties for the construction in here?
			// Or do I intent to have a function on the system that handles this?
			void Create(unsigned amount = 1) {
				for (unsigned i = 0; i < components.size(); ++i) { 
					BufferWriter writer(components[i].data);
				}
			}

			/*! Destory a instance of an entity, this will deactivate the entity and flag it for removal */
			void Destroy() {

			}

		};
	}
}