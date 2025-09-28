#include "CircleBuff.h"
#include <memory>
CCircleBuff::CCircleBuff(unsigned int _capacity)
	:m_begin(new char[_capacity]),
	m_end(m_begin + _capacity),
	m_readPointer(m_begin),
	m_writePointer(m_begin),
	m_capacity(_capacity),
	m_size(0)
{

}

CCircleBuff::~CCircleBuff()
{
	delete[] m_begin;
}

unsigned int CCircleBuff::GetSize()
{
	return m_size;
}

unsigned int CCircleBuff::GetCapacity()
{
	return m_capacity;
}

char* CCircleBuff::GetReadPointer()
{
	return m_readPointer;
}

char* CCircleBuff::GetWritePointer()
{
	return m_writePointer;
}

unsigned int CCircleBuff::GetWriteAbleSize()
{
	if (m_size == 0 || m_readPointer < m_writePointer) return m_end - m_writePointer;

	return m_readPointer - m_writePointer;
}

unsigned int CCircleBuff::GetReadAbleSize()
{
	if (m_size == 0 || m_readPointer < m_writePointer) return m_size;

	return m_end - m_readPointer;
}

bool CCircleBuff::AddSize(unsigned int _size)
{
	if (m_capacity - m_size < _size) return false;

	m_size += _size;
	m_writePointer += _size;
	if (m_writePointer >= m_end)
	{
		m_writePointer -= m_capacity;
	}

	return true;
}

bool CCircleBuff::Pop(unsigned int _size)
{
	if (m_size < _size) return false;

	m_size -= _size;
	m_readPointer += _size;
	if (m_readPointer >= m_end)
	{
		m_readPointer -= m_capacity;
	}

	return true;
}

bool CCircleBuff::Peek(char* _destination, unsigned int _destinationSize)
{
	if (_destinationSize > m_size) return false;

	int directReadSize = GetReadAbleSize();
	if (_destinationSize <= directReadSize)
	{
		memcpy_s(_destination, _destinationSize, m_readPointer, _destinationSize);
		return true;
	}

	int remainSize = _destinationSize - directReadSize;
	memcpy_s(_destination, directReadSize, m_readPointer, directReadSize);
	memcpy_s(_destination + directReadSize, remainSize, m_begin, remainSize);

	return true;
}

unsigned int CCircleBuff::CopyWord(unsigned short* _destination)
{
	if (m_size < sizeof(unsigned short)) return NULL;

	if (GetReadAbleSize() < sizeof(unsigned short))
	{
		char* temp = reinterpret_cast<char*>(_destination);
		*temp = *m_readPointer;
		*(temp + 1) = *m_begin;
		return m_size;
	}

	*_destination = *reinterpret_cast<unsigned short*>(m_readPointer);

	return m_size;
}