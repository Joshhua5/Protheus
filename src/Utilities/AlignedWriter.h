#pragma once
#include "BufferIO.h"

namespace Pro {
	namespace Util {
		/*! AlignedWriter adds writing functionality to a preexisting AlignedWriter
			AlignedWriter works in the sizeOf from the AlignedWriter instead of bytes.
		*/
		class AlignedWriter : 
			BufferIO {
		public:
			AlignedWriter(AlignedBuffer* buffer) {
				using_smart = false;
				aligned_buffer_ = buffer;
				head_ = 0;
			}
			AlignedWriter(smart_ptr<AlignedBuffer> pointer) {
				using_smart = true;
				aligned_buffer_ = pointer;
				head_ = 0;
			}
			~AlignedWriter() {
				head_ = 0;
				if (using_smart)
					aligned_buffer_ = nullptr;
				else
					aligned_buffer_.dereference();
			}

			/*! Writes a single value into the Pro::Util::AlignedBuffer and iterates the head*/
			template<typename T>
			inline void Write(const T& value){
				*aligned_buffer_->At(head_++) = value;
			}

			/*! Writes a single value into the Pro::Util::AlignedBuffer and iterates the head*/
			template<typename T>
			inline void Write(const T&& value) {
				*aligned_buffer_->At(head_++) = std::move(value);
			}

			/*! Writes an array into the Pro::Util::AlignedBuffer.
				The Array must be at least of size count.
			*/
			template<typename T>
			inline void WriteElements(T* elements, unsigned count) {
				for (unsigned x = 0; x < count; ++x)
					*aligned_buffer_->At(head_++) = elements[x];
			}
		};
	}
}