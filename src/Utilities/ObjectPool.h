#include "LinkedList.h"

namespace Pro{
	namespace Util{
		template<class T>
		class ObjectPool{
			LinkedList<T> list;
			
		public:  

			/*! Stores a object as unused for reuse*/
			void store(T* obj){
				list.push_back(obj);
			}
			/*! Returns a old object, allocates a new one if the pool is empty */
			template<typename... Args>
			T* get(Args&&... args){
				if (list.empty())
					return new T(args...);
				else{
					auto obj = list.pop_back();
					*obj = T(args...);
					return obj;
				}
			}

			T* get(){
				if (list.empty())
					// If getting error here, make sure object has default constructor
					return new T();
				else
					return list.pop_back();
			}
		};
	}
}