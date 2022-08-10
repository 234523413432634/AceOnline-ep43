// VMem.h: interface for the CVMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMEM_H__6FFF3C69_50F7_4B4B_A40F_D81A0A7713AC__INCLUDED_)
#define AFX_VMEM_H__6FFF3C69_50F7_4B4B_A40F_D81A0A7713AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <VECTOR>
#include <functional>
#include <algorithm>
#include "global.h"

#ifndef DBGOUT
#define DBGOUT (void(0))
#endif

using namespace std;

typedef ez_map<void*, BYTE>		ezmapVoidPtr8BYTE;		// 2008-07-24 by cmkwon, �޸�Ǯ üũ �߰�(����� ���ø�) - 

class CVMem
{
public:
	CVMem();
	~CVMem();

public:
	BOOL AllocVMem(DWORD dwTypeSize, DWORD dwPoolCount);
	BOOL FreeAllVMem(void);
	BOOL FreeVMem(void * pVMem);
	void SaveCurrentStatus(void);			// 2009-05-04 by cmkwon, �޸�Ǯ �������� �α� ����� - 

	inline void *ObjectNew();
	inline void ObjectDelete(void *p);

private:
	inline BOOL CommitVMem(void *pAddr, DWORD dwSize);
	inline BOOL DecommitVMem(void *pAddr, DWORD dwSize);

	inline void LockVMem(void);
	inline void UnlockVMem(void);

	inline BOOL InsertFree(void *p);
	inline void* PopFree();

private:
	CRITICAL_SECTION	m_csLock;						// Virtual Memory�� ����ȭ�� ���� ũ��Ƽ�� ����
	DWORD				m_dwSizeObjType;				// Virtual Memory�� �Ҵ�Ǵ� �ϳ��� ��ü ������ ����
	DWORD				m_dwCntCommitedObjPool;
	DWORD				m_dwCntCurrentAllocObjPool;		//
	DWORD				m_dwSizeCommitVMem;
	DWORD				m_dwTotalCommitedObjCnts;		// 2008-04-11 by cmkwon, �޸�Ǯ �ý��� �α� �߰� - 

	vector<void *>		m_vectorVMemPtr;
	vector<void *>		m_vectorMemoryPointer;
#if defined(_DEBUG)
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, �޸�Ǯ üũ �߰�(����� ���ø�) - 
	ezmapVoidPtr8BYTE	m_ezmapUseFlagList;				// 2008-07-24 by cmkwon, �޸�Ǯ üũ �߰�(����� ���ø�) - 
#endif
};


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::LockVMem
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : void
// �� �� �� ��  : inline �Լ�
//					CVMem ��ü�� ũ��Ƽ�ü����� ����Ѵ�
void CVMem::LockVMem(void)
{
	EnterCriticalSection(&m_csLock);
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::UnlockVMem
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : void
// �� �� �� ��  : inline �Լ�
//					CVMem ��ü�� ũ��Ƽ�ü����� �����Ѵ�
void CVMem::UnlockVMem(void)
{
	LeaveCriticalSection(&m_csLock);
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::CommitVMem
// ��ȯ�Ǵ� ��  : BOOL
// �� �� �� ��  : void *pAddr
// �� �� �� ��  : DWORD dwSize
// �� �� �� ��  : inline �Լ�
//					����� �ּҸ� ����ϱ� ���� Commit�ϴ� �Լ�
BOOL CVMem::CommitVMem(void *pAddr, DWORD dwSize)
{
	void * pCommit = VirtualAlloc(pAddr, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if(NULL == pCommit)
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "CVMem::CommitVMem VirtualAlloc() LastError[%d] pAddr[0x%X] TotalSize[%d]\r\n",
			nErr, pAddr, dwSize);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);

		return FALSE;
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::DecommitVMem
// ��ȯ�Ǵ� ��  : BOOL
// �� �� �� ��  : void *pAddr
// �� �� �� ��  : DWORD dwSize
// �� �� �� ��  : inline �Լ�
//					Commit ���� �޸𸮸� Decommit�ϱ� ���� �Լ�
BOOL CVMem::DecommitVMem(void *pAddr, DWORD dwSize)
{
	BOOL bRet = VirtualFree(pAddr, dwSize, MEM_DECOMMIT);
	if(FALSE == bRet)
	{
		int nErr = GetLastError();
		SetLastError(0);

		char	szError[1024];
		sprintf(szError, "CVMem::DecommitVMem VirtualAlloc() LastError[%d] pAddr[0x%X] TotalSize[%d]\r\n",
			nErr, pAddr, dwSize);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);

		return FALSE;
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::InsertFree
// ��ȯ�Ǵ� ��  : BOOL
// �� �� �� ��  : DWORD id
// �� �� �� ��  : inline �Լ�
//					�޸𸮰� �����Ǹ� Stack�� ������ �޸𸮿� �ش��ϴ� id�� �߰��ϴ� �Լ�
BOOL CVMem::InsertFree(void *p)
{
//	vector<void *>::iterator itr = find(m_vectorMemoryPointer.begin(), m_vectorMemoryPointer.end(), p);
//	if(itr != m_vectorMemoryPointer.end())
//	{
//		return FALSE;
//	}

#if defined(_DEBUG)
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, �޸�Ǯ üũ �߰�(����� ���ø�) - 
	BYTE *pbyUseFlag = m_ezmapUseFlagList.findEZ_ptr(p);
	if(NULL == pbyUseFlag)
	{
		m_ezmapUseFlagList.insertEZ(p, FALSE);
	}
	else
	{
		if(FALSE == *pbyUseFlag)
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] MemPool InsertFree error !!, m_dwSizeObjType(%d)\r\n", m_dwSizeObjType);
			return FALSE;
		}
		*pbyUseFlag = FALSE;
	}
#endif

	m_vectorMemoryPointer.push_back(p);
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::PopFree
// ��ȯ�Ǵ� ��  : DWORD
// �� �� �� ��  : inline �Լ�
//					�޸� �Ҵ��� ���� Stack���� id�� ������ �����ϴ� �Լ�
void * CVMem::PopFree()
{
	if(m_vectorMemoryPointer.empty() == true)
	{	// ����ִ�. Underflow

		if(AllocVMem(m_dwSizeObjType, m_dwCntCommitedObjPool) == FALSE)
		{
			char	szError[1024];
			sprintf(szError, "CVMem::PopFree Queue Underflow MemoryPointerSize[%d]\r\n",
				m_vectorMemoryPointer.size());
			g_pGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return NULL;
		}
	}
	void *p = m_vectorMemoryPointer.back();
	m_vectorMemoryPointer.pop_back();

#if defined(_DEBUG)
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, �޸�Ǯ üũ �߰�(����� ���ø�) - 
	BYTE *pbyUseFlag = m_ezmapUseFlagList.findEZ_ptr(p);
	if(NULL == pbyUseFlag)
	{
		m_ezmapUseFlagList.insertEZ(p, TRUE);
	}
	else
	{		
		*pbyUseFlag		= TRUE;
	}
#endif

	return p;
}



////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::ObjectNew
// ��ȯ�Ǵ� ��  : void *
// �� �� �� ��  : inline �Լ�
//					�޸� Ǯ���� �޸𸮸� �ϳ� �Ҵ��Ͽ� �����ϴ� �Լ�
void * CVMem::ObjectNew()
{
	LockVMem();
	void* pNew = PopFree();					// �����ִ� ���� �ּҷ� ��´�.
	if(pNew != NULL)
	{
		m_dwCntCurrentAllocObjPool++;
	}
	UnlockVMem();

	return pNew;
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CVMem::ObjectDelete
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : void *p
// �� �� �� ��  : inline �Լ�
//					����� �޸𸮸� �����ϴ� �Լ�(�޸� Ǯ�� �߰���)
void CVMem::ObjectDelete(void *p)
{
	LockVMem();
	if(InsertFree(p))
	{
		m_dwCntCurrentAllocObjPool--;
	}
	UnlockVMem();
}
#endif // !defined(AFX_VMEM_H__6FFF3C69_50F7_4B4B_A40F_D81A0A7713AC__INCLUDED_)
