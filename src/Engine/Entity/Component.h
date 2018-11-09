#pragma once
 
#include "ComponentData.h"

namespace Pro {
	namespace Entity {     
		template<typename DataStruct>
		class Component {
		public:   
			static virtual unsigned UUID = 0;
			static virtual void Start(DataStruct* data) = 0;
			static virtual void Update(DataStruct* data) = 0;
		};
	}
}