#pragma once
#include <WinSock2.h>
class CMemoryStream
{
private:
	byte* m_buff;
	byte* m_writePointer;

	u_short m_size;
	u_short m_capacity;

public:
	CMemoryStream(u_short _capacity);
	CMemoryStream(const CMemoryStream& _memoryStream);
	~CMemoryStream();

	CMemoryStream& operator=(const CMemoryStream& _memoryStream);

	void Reset();
	bool Write(byte _src);
	bool Write(u_short _src);
	bool Write(float _src);
	bool Write(ULONG _src);
	bool Write(UINT64 _src);
	bool Write(const void* _src, u_short _srcSize);
	void WriteSizeToFront();
	void Read(byte* _des);

	byte* GetBuff();
	u_short GetSize();
	u_short GetCapacity();
};