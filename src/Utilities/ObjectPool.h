#include "LinkedList.h"
#include "smart_ptr.h"

namespace Pro {
	namespace Util {
		/*! ObjectPool keeps track of memory allocated for objects
			and constructs a new object without allocation when requested. */
		template<class T>
		class ObjectPool {
			LinkedList<T> list_;

		public:

			~ObjectPool() {
				while (!list_.empty()) 
					operator delete(list_.PopBack()); 
			}

			/*! Stores a object as unused for reuse*/
			void Store(T* obj) {
				obj->~T();
				list_.PushBack(obj);
			} 

			/*! Returns a old object, allocates a new one if the pool is empty */
			template<typename... Args>
			smart_ptr<T> Get(Args&&... args) {
				if (list_.empty())
					return new T(args...);
				else {
					smart_ptr<T>  obj = list_.PopBack();
					operator new(obj.get()) T(args...);
					return std::move(obj);
				}
			}

			smart_ptr<T> Get() {
				if (list_.empty())
					// If getting error here, make sure object has default constructor
					return new T();
				else {
					smart_ptr<T> obj = list_.PopBack();
					operator new(obj.get()) T();
					return std::move(obj);
				}
			}
		};
	}
}