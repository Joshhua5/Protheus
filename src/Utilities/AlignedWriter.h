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
				m_aligned_buffer = buffer;
				m_head = 0;
			}
			AlignedWriter(smart_ptr<AlignedBuffer> pointer) {
				using_smart = true;
				m_aligned_buffer = pointer;
				m_head = 0;
			}
			~AlignedWriter() {
				m_head = 0;
				if (using_smart)
					m_aligned_buffer = nullptr;
				else
					m_aligned_buffer.dereference();
			}

			/*! Writes a single value into the Pro::Util::AlignedBuffer and iterates the head*/
			template<typename T>
			inline void write(const T& value){
				*m_aligned_buffer->at(m_head++) = value;
			}

			/*! Writes a single value into the Pro::Util::AlignedBuffer and iterates the head*/
			template<typename T>
			inline void write(const T&& value) {
				*m_aligned_buffer->at(m_head++) = std::move(value);
			}

			/*! Writes an array into the Pro::Util::AlignedBuffer.
				The Array must be at least of size count.
			*/
			template<typename T>
			inline void write_elements(T* elements, unsigned count) {
				for (unsigned x = 0; x < count; ++x)
					*m_aligned_buffer->at(m_head++) = elements[x];
			}
		};
	}
}