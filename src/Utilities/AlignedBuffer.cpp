#include "AlignedBuffer.h"
 
using namespace Pro;

AlignedBuffer::AlignedBuffer(const unsigned size, const unsigned sizeOf,
	void* data, const unsigned alignment) {

	// Check sizeof

	if (sizeOf == 0 ||  size < sizeOf)
		return;

	// Calculate padding
	 
	m_alignment = alignment;
	m_sizeof = sizeOf;
	m_alignment_crossover = static_cast<unsigned char>(floor(m_sizeof / m_alignment)) + 1; 
	m_padding = (m_alignment * m_alignment_crossover) % m_sizeof;

	// initialize data for alignment

	m_data = new char[size + ((size / m_alignment / m_alignment_crossover) * m_padding) + m_alignment];
	long i_data = (long)m_data;
	m_offset = static_cast<unsigned char>(m_alignment - (i_data % m_alignment)); 
	m_data = static_cast<char*>(m_data) + m_offset;
	 
	// Copy the data into the new buffer,
	// adding in padding
	if (data != nullptr) {
		if(m_padding == 0)
			memcpy(m_data, data, size);
		else {
			char* m_read = (char*)data;
			char* m_writer = (char*)m_data;
			for (unsigned x = 0; x < size / m_sizeof; ++x) {
				memcpy(m_writer, m_read, m_sizeof);
				m_writer += m_sizeof + m_padding;
				m_read += m_sizeof;
			}
		}
	}
}

AlignedBuffer::~AlignedBuffer() {
	m_data = static_cast<char*>(m_data) - m_offset;
	delete[] m_data; 
} 

void* AlignedBuffer::atBoundary(unsigned pos) const {
	return static_cast<char*>(m_data) + (pos * m_alignment_crossover * m_alignment);
}

void* AlignedBuffer::at(unsigned pos) const {
	const unsigned offset = pos * m_sizeof;
	const unsigned position = (offset + ((offset / m_alignment) * (m_padding / m_alignment_crossover)));
	if (pos >= position)
		return nullptr;
	return static_cast<char*>(m_data) + position;
}

unsigned AlignedBuffer::alignment() const {
	return m_alignment;
}

void* AlignedBuffer::data() {
	return m_data;
}

unsigned AlignedBuffer::size() const {
	return m_size;
}

void* AlignedBuffer::operator[](unsigned pos) {
	const unsigned offset = pos * m_sizeof;
	const unsigned position = (offset + ((offset / m_alignment) * (m_padding / m_alignment_crossover)));
	return static_cast<char*>(m_data) + position;
} 