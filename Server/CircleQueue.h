#pragma once
#include <WinSock2.h>
class CCircleQueue
{
private:
	byte* m_begin;
	byte* m_end;
	byte* m_readPointer;
	byte* m_writePointer;

	size_t m_capacity;
	size_t m_size;

public:
	CCircleQueue(size_t _capacity);
	~CCircleQueue();

	size_t GetSize();
	size_t GetCapacity();
	byte* GetReadPointer();
	byte* GetWritePointer();
	size_t GetWriteAbleSize();
	size_t GetReadAbleSize();

	size_t AddSize(size_t _size);
	size_t Pop(size_t _size);

	bool Peek(byte* _destination, size_t _destinationSize);
	bool GetWord(u_short* _destination);
};