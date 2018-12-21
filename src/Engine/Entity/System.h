#pragma once
/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2018.
 -------------------------------------------------------------------------

 Description:

 A system is 
  

 *************************************************************************/

#include <ArrayList.h>
#include "ComponentIterator.h"

namespace Pro {
	namespace ECS {

		// template <typename... Args>
		// struct variadic_typedef { };
		// 
		// template<typename... Inputs>
		// struct Components {
		// 	typedef std::tuple<Inputs...> type;
		// };
		// 
		// template<typename... Inputs>
		// struct Components{ 
		// 	using components = Inputs;
		// };  

		//template <typename... Args>
		//struct convert_in_tuple<variadic_typedef<Args...>>
		//{
		//	// expand the variadic_typedef back into
		//	// its arguments, via specialization
		//	// (doesn't rely on functionality to be provided
		//	// by the variadic_typedef struct itself, generic)
		//	typedef typename convert_in_tuple<Args...>::type type;
		//};
		 
		class SystemBase { 
		public:
			SystemBase() {}
			void virtual Execute(Entity&) = 0;
		};

		// TODO I'd like to have systems that can feed other systems
		template<typename... Input> //, class Output = Components<>>
		class System : SystemBase {
			template<typename T> using add_pointer = T*;  
			ComponentIterator<Input...>* iterator; 
			 
		protected: 

			inline void Reset(Entity& entity) {
				// This needs to change.
				// WHY we do this is that the iterator need to be constructed if it's not a pointer.
				// ComponentIterator doesn't have a default constructor since it stores a tuple which arguments
				// We can't pass in null for the tuple, since it stores Iterators that also don't have default constructors.
				// and require a reference type.
				iterator = new ComponentIterator<Input...>(entity.Iterator<Input...>());
			}

			template<typename Component>
			inline Component* Next() {
				return iterator->Get<Component>().Read();
			}  
		}; 
	}
}