/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2016.
-------------------------------------------------------------------------

Description:
	abstract extern class for buffers
-------------------------------------------------------------------------
History:
- 13:07:2014: Waring J.
*************************************************************************/
#pragma once

#include <memory> 
#include <cstring>
#include <mutex>
#include <math.h>
#include "Log.h" 

namespace Pro {
	namespace Util {
		class BufferBase
		{
		protected:
			/*! Remembers if the buffer was copied to prevent the
				deconstructor from deleting data which it doesn't own
			*/
			bool was_copied_;
			/*! Size of the buffer in bytes */
			size_t size_;
			/*! Pointer to the data in the heap*/
			void* data_; 
		public:
			BufferBase() { 
				data_ = nullptr;
			}
			  
			inline size_t size() const {
				return size_;
			}

			template<typename T>
			inline T* data() {
				return reinterpret_cast<T*>(data_);
			}

			template<typename T>
			inline const T* data() const {
				return reinterpret_cast<T*>(data_);
			}

			inline void* data() {
				return data_;
			}

			inline const void* data() const {
				return data_;
			}
		};
	}
}