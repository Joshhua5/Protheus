#pragma once
  
#include "BufferIO.h"

namespace Pro {
	namespace Util {
		/*! AlignedReader adds reading functionality to a preexisting AlignedBuffer 
			AlignedReader works in the sizeOf from the AlignedBuffer instead of bytes.
		*/
		class AlignedReader :
			BufferIO
		{
		public:
			AlignedReader(const AlignedReader&) = delete;

			AlignedReader(AlignedBuffer* buffer) {
				using_smart = false;
				m_head = 0;
				m_aligned_buffer = buffer;
			}
			AlignedReader(smart_pointer<AlignedBuffer> pointer) {
				using_smart = true;
				m_head = 0;
				m_aligned_buffer = pointer;
			}
			AlignedReader(AlignedReader&& buffer) {
				m_aligned_buffer = buffer.m_aligned_buffer;
				m_head = buffer.m_head;
				buffer.m_aligned_buffer = nullptr;
			}
			~AlignedReader() {
				m_head = 0;
				if (using_smart)
					m_aligned_buffer = nullptr;
				else
					m_aligned_buffer.dereference();
			}

			/*! Returns a pointer to the internal structure which is currently being read
				Does not skip over data
			*/
			inline void* read_raw() const {
				return m_aligned_buffer->at(m_head);
			}

			/*! Reads the an array of elements and copies the data while preserving the alignment */
			inline AlignedBuffer read(const unsigned elements) {
				AlignedBuffer out(elements * m_aligned_buffer->sizeOf(),
					m_aligned_buffer->sizeOf(), m_aligned_buffer->at(m_head));
				skip(elements);
				return out;
			}

			/*! Reads one element and returns the value */
			template<typename T>
			inline T read() {
				return *m_aligned_buffer->at(m_head++);
			}

			/*! Reads all values in one alignment
				Values are not copied and aren't safe to be deleted
				EG.
				An alignment of 32 will contain room for 8 floats
				In this case readAlignment() will return an array of 8 floats
			*/
			template<typename T>
			inline T* readAlignment() {
				T* out = static_cast<T*>(m_aligned_buffer->at(m_head));
				skip(
					floor(m_aligned_buffer->alignment() / m_aligned_buffer->sizeOf()) +
					m_aligned_buffer->m_alignment_crossover);
				return out;
			}

			/*! False if the m_head is at the end of the reader*/
			inline bool hasNext() const {
				return m_aligned_buffer->size() > m_head;
			} 
		};
	}
}