#include "Page.h"

CPage::CPage(u_short _index, u_short _buffSize)
	:m_index(_index),
	m_pageStream(_buffSize)
{

}

CPage::~CPage()
{
	
}

void CPage::Reset()
{
	m_pageStream.Reset();
}

bool CPage::Write(byte _src)
{
	return m_pageStream.Write(_src);
}

bool CPage::Write(u_short _src)
{
	return m_pageStream.Write(_src);
}

bool CPage::Write(byte* _src, u_short _srcSize)
{
	return m_pageStream.Write(_src, _srcSize);
}

u_short CPage::GetIndex()
{
	return m_index;
}

CMemoryStream& CPage::GetPageStream()
{
	return m_pageStream;
}