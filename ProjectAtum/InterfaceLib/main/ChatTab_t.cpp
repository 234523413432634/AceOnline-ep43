#include "ChatTab_t.h"
#include <tchar.h>
#include "DbgOut_C.h"


ChatTab_t::~ChatTab_t()
{
	for(int i = 0; i < CHAT_BUFFER_NUMBER; i++)
	{
		if(m_ArrChatLine[i].pszChatString != NULL&& 
			strlen(m_ArrChatLine[i].pszChatString))
		{
			delete [] m_ArrChatLine[i].pszChatString;
			m_ArrChatLine[i].pszChatString = NULL;
		}
	}
}
BOOL ChatTab_t::IsValidChatLineIndex(int i_nIdx)
{
	if(0 <= i_nIdx
		&& CHAT_BUFFER_NUMBER > i_nIdx)
	{
		return TRUE;
	}

	return FALSE;
}

void ChatTab_t::PushChatLine(char *i_szChatLine, DWORD i_dwColorType, DWORD i_dwType /* = 0 */)
{
	//////////////////////////////////////////////////////////////////////////////
	// 에러 체크 -OverFlow-
	if((m_nStoreStartIndex+1)%CHAT_BUFFER_NUMBER == m_nStoreEndIndex)
	{
		m_nStoreEndIndex++;
		m_nStoreEndIndex %= CHAT_BUFFER_NUMBER;
		m_nStoredLineCounts--;
	}
	//
	//////////////////////////////////////////////////////////////////////////////

//	// 2006-04-13 by ispark, 테스트 길이가 SIZE_MAX_CHAT_MESSAGE보다 길면 다음줄로 찍는다.
	int nStrlen = strlen(i_szChatLine);

		if(m_ArrChatLine[m_nStoreStartIndex].pszChatString != NULL)
		{
			if(0 == strlen(m_ArrChatLine[m_nStoreStartIndex].pszChatString))
			{
				DBGOUT("=================> by cmkwon\r\n");
			}
			delete [] m_ArrChatLine[m_nStoreStartIndex].pszChatString;
			m_ArrChatLine[m_nStoreStartIndex].pszChatString = NULL;
		}
		m_ArrChatLine[m_nStoreStartIndex].pszChatString = new char[nStrlen+1];
		strcpy(m_ArrChatLine[m_nStoreStartIndex].pszChatString, i_szChatLine);
		m_ArrChatLine[m_nStoreStartIndex].dwColorType = i_dwColorType;
		m_ArrChatLine[m_nStoreStartIndex].dwType = i_dwType;

		m_nStoreStartIndex++;
		m_nStoreStartIndex %= CHAT_BUFFER_NUMBER;	
		m_nStoredLineCounts++;

		m_nRenderStartIndex++;
		m_nRenderStartIndex %= CHAT_BUFFER_NUMBER;

	//////////////////////////////////////////////////////////////////////////////
	// 에러 체크 -UnderFlow-
	if(m_nRenderStartIndex == m_nStoreStartIndex)
	{
		m_nRenderStartIndex--;
		if(m_nRenderStartIndex < 0)
		{
			m_nRenderStartIndex = CHAT_BUFFER_NUMBER-1;
		}             
	}
	//
	//////////////////////////////////////////////////////////////////////////////	
}

int ChatTab_t::ChatLineLength(int i_nIdx)
{
	if(FALSE == this->IsValidChatLineIndex(i_nIdx))
	{
		return 0;
	}

	return strlen(m_ArrChatLine[i_nIdx].pszChatString);
}

char* ChatTab_t::GetChatLine(int i_nIdx)
{
	if(FALSE == this->IsValidChatLineIndex(i_nIdx))
	{
		return NULL;
	}

	return m_ArrChatLine[i_nIdx].pszChatString;
}

DWORD ChatTab_t::GetColorType(int i_nIdx)
{
	if(FALSE == this->IsValidChatLineIndex(i_nIdx))
	{
		return 0;
	}

	return m_ArrChatLine[i_nIdx].dwColorType;
}
DWORD ChatTab_t::GetType(int i_nIdx)
{
	if(FALSE == this->IsValidChatLineIndex(i_nIdx))
	{
		return 0;
	}

	return m_ArrChatLine[i_nIdx].dwType;
}


int ChatTab_t::IncreaseRenderStartIndex(void)
{	
	if(0 < m_nStoredLineCounts
		&& m_nStoreStartIndex != (m_nRenderStartIndex+1)%CHAT_BUFFER_NUMBER)
	{
		m_nRenderStartIndex++;
		m_nRenderStartIndex %= CHAT_BUFFER_NUMBER;
	}	

	return m_nRenderStartIndex;
}

int ChatTab_t::DecreaseRenderStartIndex(void)
{
	if(m_nStoreEndIndex != m_nRenderStartIndex)
	{
		m_nRenderStartIndex--;
		if(0 > m_nRenderStartIndex)
		{
			m_nRenderStartIndex = CHAT_BUFFER_NUMBER-1;
		}
	}
	
	return m_nRenderStartIndex;
}

int ChatTab_t::SetRenderStartIndexWithScrollPositon(int i_nScrMoveRange, int i_nCurScrPosition)	// 채팅박스에 들어갈수있는 라인수 현재스크롤 위치 Y
{
	if(m_nStoredLineCounts > 1)
	{	
		float	fLinePixel = (float)i_nScrMoveRange/(float)m_nStoredLineCounts;
		int		nIndexGap = (float)i_nCurScrPosition/fLinePixel;
		if(nIndexGap == m_nStoredLineCounts)
		{
			nIndexGap--;
		}
		m_nRenderStartIndex = m_nStoreEndIndex + nIndexGap;
		m_nRenderStartIndex %= CHAT_BUFFER_NUMBER;		
	}
	return m_nRenderStartIndex; // 현재 스크롤포지션에 대한 스타트 인덱스를 리턴한다 
}


int ChatTab_t::GetScrollPositionWithRenderStartIndex(int i_nScrMoveRange, int i_nRanderLine) // 채팅 박스에 들어갈수있는 픽셀수와 라인수 
{
	int nRet = i_nScrMoveRange;
	if(m_nStoredLineCounts > 1
		&& m_nStoredLineCounts > i_nRanderLine/2)
	{
		float	fLinePixel = (float)i_nScrMoveRange/(float)m_nStoredLineCounts;	// 픽셀당 들어 갈수있는 라인수 
		int		nIndexGap; // RenderIndex와 StoreEndIndex 사이에 있는 라인 수( 200라인을 기준으로 한번 루프할때 생기는 갭을 생각해서)
		if(m_nRenderStartIndex >= m_nStoreEndIndex)
		{
			nIndexGap = m_nRenderStartIndex - m_nStoreEndIndex + 1;
		}
		else
		{
			nIndexGap = CHAT_BUFFER_NUMBER - m_nStoreEndIndex + m_nRenderStartIndex + 1;
		}

		nRet = fLinePixel*nIndexGap;// 한라인당 픽셀수 * 현재 스크롤의 위치 = 현재 스크롤 위치에 픽셀 값
	}

 	return nRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		전체 채팅정보를 가져온다.
/// \author		// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵업
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void ChatTab_t::GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER])
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < CHAT_BUFFER_NUMBER;nCnt++)
	{
		char* pSrc = m_ArrChatLine[nCnt].pszChatString;
		if(NULL == pSrc)
		{
			continue;
		}
		int nStrlen = strlen(pSrc);
		SAFE_DELETE(pArrChatLine[nCnt].pszChatString);
		pArrChatLine[nCnt].pszChatString = new char[nStrlen+1];
		strcpy(pArrChatLine[nCnt].pszChatString, pSrc);		

		pArrChatLine[nCnt].dwColorType = m_ArrChatLine[nCnt].dwColorType;

		// 2009-05-06 by bhsohn 아레나 이동시, 채팅 뺵업 안되는 문제 해결
		pArrChatLine[nCnt].dwType = m_ArrChatLine[nCnt].dwType;
		// end 2009-05-06 by bhsohn 아레나 이동시, 채팅 뺵업 안되는 문제 해결
	}
}