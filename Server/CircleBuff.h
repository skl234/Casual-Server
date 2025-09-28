#pragma once
class CCircleBuff
{
private:
	char* m_begin;
	char* m_end;
	char* m_readPointer;
	char* m_writePointer;

	unsigned int m_capacity;
	unsigned int m_size;

public:
	CCircleBuff(unsigned int _capacity);
	~CCircleBuff();

	unsigned int GetSize();
	unsigned int GetCapacity();
	char* GetReadPointer();
	char* GetWritePointer();
	unsigned int GetWriteAbleSize();
	unsigned int GetReadAbleSize();

	bool AddSize(unsigned int _size);
	bool Pop(unsigned int _size);

	bool Peek(char* _destination, unsigned int _destinationSize);
	unsigned int CopyWord(unsigned short* _destination);
};