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

		template <typename Func, typename A, typename ...Args> struct Caller
		{
			static void call(Func& f, A&& a, Args&& ...args)
			{
				f(std::forward<A>(a));
				Caller<Func, Args...>::call(f, std::forward<Args>(args)...);
			}
		};

		template <typename Func, typename A> struct Caller<Func, A>
		{
			static void call(Func& f, A&& a)
			{
				f(std::forward<A>(a));
			}
		};

		template <typename Func, typename ...Args>
		void Call(Func& f, Args&& ...args)
		{
			Caller<Func, Args...>::call(f, std::forward<Args>(args)...);
		}
		 
		class SystemBase { 
		public: 
			void virtual Execute(Entity&) = 0;
		};
		
		// TODO I'd like to have systems that can feed other systems
		template<typename... Input> //, class Output = Components<>>
		class System : SystemBase {
			ComponentIterator<Input...>* iterator;   
			//std::function<void(System&)> executor;
			std::function<void(Input&...)> executor;


			template<typename T> using add_pointer = T*;
			template<typename T> using get_iterator = iterator->Get<Component>()++;
		public: 
			System() = default; 
			// If possible we want to accept a function that is called on each instanct 
			// parsing it's own components to the function
			// currently you need to write the function to process all entities.
			// this does allow for computation sharing, although the lassed lambda could allow that.
			System(function<void(Input&...)> _exe) : executor(_exe) {}
		
			inline void Reset(Entity& entity) {
				// This needs to change.
				// WHY we do this is that the iterator need to be constructed if it's not a pointer.
				// ComponentIterator doesn't have a default constructor since it stores a tuple which arguments
				// We can't pass in null for the tuple, since it stores Iterators that also don't have default constructors.
				// and require a reference type.
				// iterator = new ComponentIterator<Input...>(entity.Iterator<Input...>());
			}
		
			template<typename Component>
			Component& Next() { 
				return *iterator->Get<Component>()++;
			}  
			 
			void Execute(Entity& entity) {
				Reset(entity); 
				     
				//executor(next_pointer<Input...>());
				//executor(iterator->Next());
				std::apply(executor, iterator->Next());
			}
		};   
	}
}