#include "CircleQueue.h"
#include <memory>
#include <vector>
#include "eException.h"
CCircleQueue::CCircleQueue(size_t _capacity)
	:m_begin(new byte[_capacity]),
	m_end(m_begin + _capacity),
	m_readPointer(m_begin),
	m_writePointer(m_begin),
	m_capacity(_capacity),
	m_size(0)
{
	
}

CCircleQueue::~CCircleQueue()
{
	delete[] m_begin;
}

size_t CCircleQueue::GetSize()
{
	return m_size;
}

size_t CCircleQueue::GetCapacity()
{
	return m_capacity;
}

byte* CCircleQueue::GetReadPointer()
{
	return m_readPointer;
}

byte* CCircleQueue::GetWritePointer()
{
	return m_writePointer;
}

size_t CCircleQueue::GetWriteAbleSize()
{
	if (m_size == 0 || m_readPointer < m_writePointer) return m_end - m_writePointer;

	return m_readPointer - m_writePointer;
}

size_t CCircleQueue::GetReadAbleSize()
{
	if (m_size == 0 || m_readPointer < m_writePointer) return m_size;

	return m_end - m_readPointer;
}

size_t CCircleQueue::AddSize(size_t _size)
{
	if (m_capacity - m_size < _size) throw eException::Error_QueueOverFlow;

	m_size += _size;
	m_writePointer += _size;
	if (m_writePointer >= m_end)
	{
		m_writePointer -= m_capacity;
	}

	return m_size;
}

size_t CCircleQueue::Pop(size_t _size)
{
	if (m_size < _size) throw eException::Error_QueueUnderFlow;

	m_size -= _size;
	m_readPointer += _size;
	if (m_readPointer >= m_end)
	{
		m_readPointer -= m_capacity;
	}

	return m_size;
}

bool CCircleQueue::Peek(byte* _destination, size_t _destinationSize)
{
	if (_destinationSize > m_size) return false;

	size_t directReadSize = GetReadAbleSize();
	if (_destinationSize <= directReadSize)
	{
		memcpy_s(_destination, _destinationSize, m_readPointer, _destinationSize);
		return true;
	}

	size_t remainSize = _destinationSize - directReadSize;
	memcpy_s(_destination, directReadSize, m_readPointer, directReadSize);
	memcpy_s(_destination + directReadSize, remainSize, m_begin, remainSize);

	return true;
}

bool CCircleQueue::GetWord(u_short* _destination)
{
	if (m_size < sizeof(u_short)) return false;

	if (GetReadAbleSize() < sizeof(u_short))
	{
		byte* temp = reinterpret_cast<byte*>(_destination);
		*temp = *m_readPointer;
		*(temp + 1) = *m_begin;
		return true;
	}

	*_destination = *reinterpret_cast<u_short*>(m_readPointer);

	return true;
}