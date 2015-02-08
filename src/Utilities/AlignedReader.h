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
				head_ = 0;
				aligned_buffer_ = buffer;
			}
			AlignedReader(smart_ptr<AlignedBuffer> pointer) {
				using_smart = true;
				head_ = 0;
				aligned_buffer_ = pointer;
			}
			AlignedReader(AlignedReader&& buffer) {
				aligned_buffer_ = buffer.aligned_buffer_;
				head_ = buffer.head_;
				buffer.aligned_buffer_ = nullptr;
			}
			~AlignedReader() {
				head_ = 0;
				if (using_smart)
					aligned_buffer_ = nullptr;
				else
					aligned_buffer_.dereference();
			}

			/*! Returns a pointer to the internal structure which is currently being read
				Does not skip over data
			*/
			inline void* ReadRaw() const {
				return aligned_buffer_->At(head_);
			}

			/*! Reads the an array of elements and copies the data while preserving the alignment */
			inline AlignedBuffer Read(const unsigned elements) {
				AlignedBuffer out(elements * aligned_buffer_->sizeOf(),
					aligned_buffer_->sizeOf(), aligned_buffer_->At(head_));
				Skip(elements);
				return out;
			}

			/*! Reads one element and returns the value */
			template<typename T>
			inline T Read() {
				return *aligned_buffer_->At(head_++);
			}

			/*! Reads all values in one alignment
				Values are not copied and aren't safe to be deleted
				EG.
				An alignment of 32 will contain room for 8 floats
				In this case readAlignment() will return an array of 8 floats
			*/
			template<typename T>
			inline T* ReadAlignment() {
				T* out = static_cast<T*>(aligned_buffer_->At(head_));
				Skip(
					floor(aligned_buffer_->alignment() / aligned_buffer_->sizeOf()) +
					aligned_buffer_->alignment_crossover_);
				return out;
			}

			/*! False if the m_head is at the end of the reader*/
			inline bool HasNext() const {
				return aligned_buffer_->size() > head_;
			} 
		};
	}
}