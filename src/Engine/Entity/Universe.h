#pragma once

#include "System.h"

namespace Pro {
	namespace ECS {
		// The universe is a collection of entities and systems, it's responsible for maintaining the data structures and coorinating the execution of systems.
		class Universe {
			struct PipelineNode {
				SystemBase* system;
				vector<type_index> dependencies;
				vector<type_index> components;
			};


			// Areas to focus on
			// Should systems have entity ownership? If so, we need a type of iterator that will link entities and their components
			// How to store/create the order of execution, including system execution order
			// Automatic Multithreading?
			// 
		public:
			void Attach(type_index type, SystemBase* system, vector<type_index> dependencies){}
			void Attach(Entity* entity) {}

		};
	}
}