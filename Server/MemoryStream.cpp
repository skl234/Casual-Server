#include "MemoryStream.h"
#include "SmartBuffer.h"
#include "eException.h"
CMemoryStream::CMemoryStream(u_short _capacity)
	:m_buff(new byte[_capacity]),
	m_writePointer(m_buff),
	m_size(0),
	m_capacity(_capacity)
{
	
}

CMemoryStream::CMemoryStream(const CMemoryStream& _memoryStream)
{
	m_size = _memoryStream.m_size;
	m_capacity = _memoryStream.m_capacity;

	//메모리 복사
	m_buff = new byte[m_capacity];
	memcpy(m_buff, _memoryStream.m_buff, m_size);
	m_writePointer = m_buff + m_size;
}

CMemoryStream::~CMemoryStream()
{
	if (m_buff) delete[] m_buff;
}

CMemoryStream& CMemoryStream::operator=(const CMemoryStream& _memoryStream)
{
	if (this == &_memoryStream) {
		return *this;
	}

	//기존 메모리 제거
	if (m_buff) delete[] m_buff;

	m_size = _memoryStream.m_size;
	m_capacity = _memoryStream.m_capacity;

	//메모리 복사
	m_buff = new byte[m_capacity];
	memcpy(m_buff, _memoryStream.m_buff, m_size);
	m_writePointer = m_buff + m_size;

	return *this;
}

void CMemoryStream::Reset()
{
	m_writePointer = m_buff;
	m_size = 0;
}

bool CMemoryStream::Write(byte _src)
{
	if (m_size + sizeof(byte) > m_capacity) return false;

	CSmartBuffer::Write(&m_writePointer, _src);
	m_size += sizeof(byte);

	return true;
}

bool CMemoryStream::Write(u_short _src)
{
	if (m_size + sizeof(u_short) > m_capacity) return false;

	CSmartBuffer::Write(&m_writePointer, _src);
	m_size += sizeof(u_short);

	return true;
}

bool CMemoryStream::Write(float _src)
{
	if (m_size + sizeof(float) > m_capacity) return false;

	CSmartBuffer::Write(&m_writePointer, _src);
	m_size += sizeof(float);

	return true;
}

bool CMemoryStream::Write(ULONG _src)
{
	if (m_size + sizeof(ULONG) > m_capacity) return false;

	CSmartBuffer::Write(&m_writePointer, _src);
	m_size += sizeof(ULONG);

	return true;
}

bool CMemoryStream::Write(UINT64 _src)
{
	if (m_size + sizeof(UINT64) > m_capacity) return false;

	CSmartBuffer::Write(&m_writePointer, _src);
	m_size += sizeof(UINT64);

	return true;
}

bool CMemoryStream::Write(const void* _src, u_short _dataSize)
{
	if (m_size + _dataSize > m_capacity) return false;

	CSmartBuffer::Write(&m_writePointer, _src, _dataSize);
	m_size += _dataSize;

	return true;
}

void CMemoryStream::WriteSizeToFront()
{
	if (m_capacity < sizeof(u_short)) *m_buff = static_cast<byte>(m_size);
	else *reinterpret_cast<u_short*>(m_buff) = m_size;
}

void CMemoryStream::Read(byte* _des)
{
	memcpy(_des, m_buff, m_size);
}

byte* CMemoryStream::GetBuff()
{
	return m_buff;
}

u_short CMemoryStream::GetSize()
{
	return m_size;
}

u_short CMemoryStream::GetCapacity()
{
	return m_capacity;
}