#pragma once
#include "MemoryStream.h"
class CPage
{
private:
	u_short m_index;
	CMemoryStream m_pageStream;
	
public:
	CPage(u_short _index, u_short _buffSize);
	~CPage();

	void Reset();
	bool Write(byte _src);
	bool Write(u_short _src);
	bool Write(byte* _src, u_short _srcSize);

	u_short GetIndex();
	CMemoryStream& GetPageStream();
};