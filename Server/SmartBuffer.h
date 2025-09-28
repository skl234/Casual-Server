#pragma once
#include <WinSock2.h>
class CSmartBuffer
{
public :
	static void Read(void* _des, byte** _pSrc, size_t _size)
	{
		memcpy(_des, *_pSrc, _size);
		*_pSrc += _size;
	}
	static byte ReadByte(byte** _pSrc)
	{
		byte value = **_pSrc;
		++* _pSrc;
		return value;
	}
	static u_short ReadWord(byte** _pSrc)
	{
		u_short value = *reinterpret_cast<u_short*>(*_pSrc);
		*_pSrc += sizeof(u_short);
		return value;
	}
	static float ReadFloat(byte** _pSrc)
	{
		float value = *reinterpret_cast<float*>(*_pSrc);
		*_pSrc += sizeof(float);
		return value;
	}
	static ULONG ReadULONG(byte** _pSrc)
	{
		ULONG value = *reinterpret_cast<ULONG*>(*_pSrc);
		*_pSrc += sizeof(ULONG);
		return value;
	}
	static UINT64 ReadLongLong(byte** _pSrc)
	{
		UINT64 value = *reinterpret_cast<UINT64*>(*_pSrc);
		*_pSrc += sizeof(UINT64);
		return value;
	}
	static void Write(byte** _pDes, byte _value)
	{
		**_pDes = _value;
		++* _pDes;
	}
	static void Write(byte** _pDes, u_short _value)
	{
		*reinterpret_cast<u_short*>(*_pDes) = _value;
		*_pDes += sizeof(u_short);
	}
	static void Write(byte** _pDes, float _value)
	{
		*reinterpret_cast<float*>(*_pDes) = _value;
		*_pDes += sizeof(float);
	}
	static void Write(byte** _pDes, ULONG _value)
	{
		*reinterpret_cast<ULONG*>(*_pDes) = _value;
		*_pDes += sizeof(ULONG);
	}
	static void Write(byte** _pDes, UINT64 _value)
	{
		*reinterpret_cast<UINT64*>(*_pDes) = _value;
		*_pDes += sizeof(UINT64);
	}
	static void Write(byte** _pDes, const void* _src, size_t _size)
	{
		memcpy(*_pDes, _src, _size);
		*_pDes += _size;
	}
	static void SetWord(byte* _des, u_short _value)
	{
		*reinterpret_cast<u_short*>(_des) = _value;
	}
};