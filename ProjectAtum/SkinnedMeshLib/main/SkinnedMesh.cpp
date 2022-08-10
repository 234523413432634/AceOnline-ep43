// SkinnedMesh.cpp: implementation of the CSkinnedMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkinnedMesh.h"
#include "rmxfguid.h"
#include <malloc.h> // _alloca
#include "D3DApp.h"
#include "tchar.h"
#include "stdio.h"
#include "AtumApplication.h"
#include "d3dutil.h"
#include "dxutil.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Frustum.h"
#include "Camera.h"
#include "SceneData.h"
#include "rmxftmpl.h"
#include "dxfile.h"
#include "Chat.h"
#include "FxSystem.h"

// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
#include "AtumDatabase.h"
// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

#define D3DRM_XTEMPLATE_BYTES 3278
#define D3DFVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
#ifdef _DEBUG 
#define LOADING_TICK 30
#else
#define LOADING_TICK 25
#endif
// �޽� �ε��� 20 ms �Ѿ��� ��� ���� �۾��� �����ϰ� �Ѱ��ش�.
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

//class CAtumApplication;
//extern 	CAtumApplication	*g_pD3dApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern LPDIRECT3DDEVICE9	g_pD3dDev ;
extern unsigned char D3DRM_XTEMPLATES[];

char *szTemplates = "xof 0303txt 0032\
template XSkinMeshHeader {\
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>\
 WORD nMaxSkinWeightsPerVertex;\
 WORD nMaxSkinWeightsPerFace;\
 WORD nBones;}\
template VertexDuplicationIndices {\
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>\
 DWORD nIndices;\
 DWORD nOriginalVertices;\
 array DWORD indices[nIndices];}\
template SkinWeights {\
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>\
 STRING transformNodeName;\
 DWORD nWeights;\
 array DWORD vertexIndices[nWeights];\
 array FLOAT weights[nWeights];\
 Matrix4x4 matrixOffset;}";


void SFrame::SetTime(float fGlobalTime)
{
    UINT iKey;
    UINT dwp2;
    UINT dwp3;
    D3DXMATRIXA16 matResult;
    D3DXMATRIXA16 matTemp;
    float fTime1;
    float fTime2;
    float fLerpValue;
    D3DXVECTOR3 vScale;
    D3DXVECTOR3 vPos;
    D3DXQUATERNION quat;
    BOOL bAnimate = false;
    float fTime;

    if (m_pMatrixKeys )
    {
        fTime = (float)fmod(fGlobalTime, m_pMatrixKeys[m_cMatrixKeys-1].dwTime);

        for (iKey = 0 ;iKey < m_cMatrixKeys ; iKey++)
        {
            if ((float)m_pMatrixKeys[iKey].dwTime > fTime)
            {
                dwp3 = iKey;

                if (iKey > 0)
                {
                    dwp2= iKey - 1;
                }
                else  // when iKey == 0, then dwp2 == 0
                {
                    dwp2 = iKey;
                }

                break;
            }
        }
        fTime1 = (float)m_pMatrixKeys[dwp2].dwTime;
        fTime2 = (float)m_pMatrixKeys[dwp3].dwTime;

        if ((fTime2 - fTime1) ==0)
            fLerpValue = 0;
        else
            fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

        if (fLerpValue > 0.5)
        {
            iKey = dwp3;
        }
        else
        {
            iKey = dwp2;
        }

        pframeToAnimate->matRot = m_pMatrixKeys[iKey].mat;
    }
    else
    {
        D3DXMatrixIdentity(&matResult);

        if (m_pScaleKeys)
        {
            dwp2 = dwp3 = 0;

            fTime = (float)fmod(fGlobalTime, m_pScaleKeys[m_cScaleKeys-1].dwTime);

            for (iKey = 0 ;iKey < m_cScaleKeys ; iKey++)
            {
                if ((float)m_pScaleKeys[iKey].dwTime > fTime)
                {
                    dwp3 = iKey;

                    if (iKey > 0)
                    {
                        dwp2= iKey - 1;
                    }
                    else  // when iKey == 0, then dwp2 == 0
                    {
                        dwp2 = iKey;
                    }

                    break;
                }
            }
            fTime1 = (float)m_pScaleKeys[dwp2].dwTime;
            fTime2 = (float)m_pScaleKeys[dwp3].dwTime;

            if ((fTime2 - fTime1) ==0)
                fLerpValue = 0;
            else
                fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);

            D3DXVec3Lerp(&vScale,
                    &m_pScaleKeys[dwp2].vScale,
                    &m_pScaleKeys[dwp3].vScale,
                    fLerpValue);


            D3DXMatrixScaling(&matTemp, vScale.x, vScale.y, vScale.z);

            D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

            bAnimate = true;
        }

        //check rot keys
        if (m_pRotateKeys )
        {
            int i1 = 0;
            int i2 = 0;

            fTime = (float)fmod(fGlobalTime, m_pRotateKeys[m_cRotateKeys-1].dwTime);

            for (iKey = 0 ;iKey < m_cRotateKeys ; iKey++)
            {
                if ((float)m_pRotateKeys[iKey].dwTime > fTime)
                {
                    i1 = (iKey > 0) ? iKey - 1 : 0;
                    i2 = iKey;
                    break;
                }
            }

            fTime1 = (float)m_pRotateKeys[i1].dwTime;
            fTime2 = (float)m_pRotateKeys[i2].dwTime;

            if ((fTime2 - fTime1) ==0)
                fLerpValue = 0;
            else
                fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);
           D3DXQuaternionSlerp(&quat, &m_pRotateKeys[i1].quatRotate, &m_pRotateKeys[i2].quatRotate, fLerpValue);

            quat.w = -quat.w;
            D3DXMatrixRotationQuaternion(&matTemp, &quat);
            D3DXMatrixMultiply(&matResult, &matResult, &matTemp);

            bAnimate = true;
        }

        if (m_pPositionKeys)
        {
            dwp2=dwp3=0;

            fTime = (float)fmod(fGlobalTime, m_pPositionKeys[m_cPositionKeys-1].dwTime);

            for (iKey = 0 ;iKey < m_cPositionKeys ; iKey++)
            {
                if ((float)m_pPositionKeys[iKey].dwTime > fTime)
                {
                    dwp3 = iKey;

                    if (iKey > 0)
                    {
                        dwp2= iKey - 1;
                    }
                    else  // when iKey == 0, then dwp2 == 0
                    {
                        dwp2 = iKey;
                    }

                    break;
                }
            }
            fTime1 = (float)m_pPositionKeys[dwp2].dwTime;
            fTime2 = (float)m_pPositionKeys[dwp3].dwTime;

            if ((fTime2 - fTime1) ==0)
                fLerpValue = 0;
            else
                fLerpValue =  (fTime - fTime1)  / (fTime2 - fTime1);


            D3DXVec3Lerp((D3DXVECTOR3*)&vPos,
                    &m_pPositionKeys[dwp2].vPos,
                    &m_pPositionKeys[dwp3].vPos,
                    fLerpValue);

//			m_vCurrentPos = vPos;
            D3DXMatrixTranslation(&matTemp, vPos.x, vPos.y, vPos.z);

            D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
            bAnimate = true;
        }
        else
        {
            D3DXMatrixTranslation(&matTemp, pframeToAnimate->matRotOrig._41, pframeToAnimate->matRotOrig._42, pframeToAnimate->matRotOrig._43);

            D3DXMatrixMultiply(&matResult, &matResult, &matTemp);
        }

        if (bAnimate)
        {
            pframeToAnimate->matRot = matResult;
        }
    }
}

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����	
// �����ڸ� �Լ��� ���� ����
//////////////////////////////////////////////////////////////////
// CSkinnedMesh 
CSkinnedMesh::CSkinnedMesh()
{
	FLOG( "CSkinnedMesh()" );
	m_bProgressiveMesh = 0;
	InitSkinnedMesh();
}
// bProgressiveMesh : ���� �ܺο��� material�� ������ ������ ���Ұ����� �����Ѵ�.
// ���Ŀ� Progress�� �����ϰ� �Ǹ� �ٽ� �����ؾ� �Ѵ�.
// Effect���� material�� �����ϱ� ������ Effect������ �� ���� ������ TRUE�ε�,
// Effect���� Specular�� ����ϰ� �Ǹ� FALSE�� ��� �Ѵ�.
CSkinnedMesh::CSkinnedMesh(BOOL bProgressiveMesh)
{
	FLOG( "CSkinnedMesh(BOOL bProgressiveMesh)" );
	m_bProgressiveMesh = bProgressiveMesh;
	InitSkinnedMesh();
	
}

void CSkinnedMesh::InitSkinnedMesh()
{
	m_pd3dDevice = NULL;
	
    m_pmcSelectedMesh = NULL;
    m_pframeSelected = NULL;
    m_pdeHead = NULL;
    m_pdeSelected = NULL;
    m_pBoneMatrices = NULL;
    m_maxBones = 0;
    m_method = SOFTWARE;
	//    m_method = D3DNONINDEXED;
    m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1;
	
	m_bTotalTextureNum = 0;
	m_bTextureNum = 0;
	m_pTexture = NULL;
	
	m_pGameData = NULL;
	m_pGameDataToDecal = NULL;
	m_pDataHeader = NULL;
	m_nRenderCount = 0;
	m_bMaterial = FALSE;
	m_bCheckBlend = FALSE;	
	D3DUtil_InitMaterial( m_material, 1.0f, 1.0f, 1.0f );// �ӽ� �ڵ�
	m_material.Specular.r = 1.0f;
	m_material.Specular.g = 1.0f;
	m_material.Specular.b = 1.0f;
	m_material.Power = 1.0f;
	m_bMultiTexture = FALSE;
	m_pOrderTexture = NULL;
	m_fRadius = 0.0f;
	m_vCenter = D3DXVECTOR3(0,0,0);

	for(int i=0; i<MAX_DECAL_NUM; i++)
	{
		m_nSelectDecalNum[i] = 0;		
	}
//	m_pDecalTextures  = NULL;
	m_nDecalCount = 0;
	m_unTexColor = 0;
	m_unTexSelectColor = 0;
	m_unUnitTexOld = 0;

	// 2007-08-29 by bhsohn �⺻ �ƸӸ� �ε��ϰԲ� ����
	m_pLoadMem = NULL;	
	// 2007-11-08 by bhsohn �κ� ����Ʈ ���� ó��

	memset(m_szMeshObjFileName, 0x00, 256);

	pxofapi			= NULL;
	pxofenum		= NULL;
	
	pdeMesh			= NULL;
	pMemory			= NULL;
	dwOptions;
	pxofobjCur		= NULL;
	pHeader			= NULL;
	m_bIsLoadingFlag  = FALSE;
	m_StartTime		= NULL;
}
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����


CSkinnedMesh::~CSkinnedMesh()
{
	FLOG( "~CSkinnedMesh()" );
	SAFE_DELETE_ARRAY(m_pDataHeader);
	SAFE_DELETE(m_pGameDataToDecal);

	// 2007-08-29 by bhsohn �⺻ �ƸӸ� �ε��ϰԲ� ����
	SAFE_DELETE_ARRAY(m_pLoadMem);
}

HRESULT CSkinnedMesh::InitDeviceObjects()
{
	FLOG( "CSkinnedMesh::InitDeviceObjects()" );
	m_pd3dDevice = g_pD3dDev;
	m_d3dCaps = g_pD3dApp->GetDeviceCaps();
    //m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );
	return CAtumNode::InitDeviceObjects();
}

HRESULT CSkinnedMesh::RestoreDeviceObjects()
{
 	FLOG( "CSkinnedMesh::RestoreDeviceObjects()" );
   HRESULT hr = S_OK;

	return CAtumNode::RestoreDeviceObjects();
}

HRESULT CSkinnedMesh::InvalidateDeviceObjects()
{
	FLOG( "CSkinnedMesh::InvalidateDeviceObjects()" );
	for (SDrawElement* pdeCurr = m_pdeHead; pdeCurr != NULL; pdeCurr = pdeCurr->pdeNext)
	{
		ReleaseDeviceDependentMeshes(pdeCurr->pframeRoot);
	}
	return CAtumNode::InvalidateDeviceObjects();
}
HRESULT CSkinnedMesh::DeleteDeviceObjects()
{
	FLOG( "CSkinnedMesh::DeleteDeviceObjects()" );
	if( m_pdeSelected == m_pdeHead )
		m_pdeSelected = NULL;
	delete m_pdeHead;
	m_pdeHead = NULL;
	delete [] m_pBoneMatrices;
	m_pBoneMatrices = NULL;
	if(m_pTexture)
	{
		// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
//		for(int n=0 ; n<m_bTotalTextureNum ; n++) 
//		{
//			SAFE_RELEASE(m_pTexture[n]);
//		}
		for(int n=0 ; n<m_bTotalTextureNum ; n++) 
		{
			if(m_pTexture[n]) 
			{ 
				if(IsLoadTexture(n))
				{
					m_pTexture[n]->Release(); 
				}
				m_pTexture[n]=NULL; 
			}
		}
		if(m_bTotalTextureNum > 0)
			SAFE_DELETE_ARRAY(m_pTexture);
		
		// 2007-08-29 by bhsohn �⺻ �ƸӸ� �ε��ϰԲ� ����
		SAFE_DELETE_ARRAY(m_pLoadMem);
	}
	
//	if(m_pDecalTextures)
//	{
//		for(int i=0; i<MAX_DECAL_NUM; i++)
//		{		
//			SAFE_RELEASE(m_pDecalTextures[i]);
//		}
//		SAFE_DELETE_ARRAY(m_pDecalTextures);
//	}
	
	SAFE_DELETE_ARRAY(m_pDataHeader);
	m_pOrderTexture = NULL;
	SAFE_DELETE(m_pGameDataToDecal);
	
	return CAtumNode::DeleteDeviceObjects();
}

void CSkinnedMesh::Tick(float fElapsedTime)
{
	FLOG( "CSkinnedMesh::Tick(float fElapsedTime)" );
	SDrawElement *pdeCur;
	SFrame *pframeCur;
	
	pdeCur = m_pdeHead;
	while (pdeCur != NULL)
	{
		pdeCur->fCurTime = fElapsedTime*160;
		pframeCur = pdeCur->pframeAnimHead;
		while (pframeCur != NULL)
		{
			pframeCur->SetTime(pdeCur->fCurTime);
			pframeCur = pframeCur->pframeAnimNext;
		}

		pdeCur = pdeCur->pdeNext;
	}

}

HRESULT CSkinnedMesh::SetProjectionMatrix()
{
//	FLOG( "CSkinnedMesh::SetProjectionMatrix()" );
	D3DXMATRIX mat;

	if (m_pdeHead == NULL)
		return S_OK;

	FLOAT fAspect = 1.0f;
	D3DXMatrixPerspectiveFovRH(&mat, 0.25f*3.141592654f, fAspect, m_pdeSelected->fRadius / 64, m_pdeSelected->fRadius * 200);
	HRESULT hr = m_pd3dDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&mat );
	if (FAILED(hr))
		return hr;
	// Set Projection Matrix for vertex shader
	D3DXMatrixTranspose(&mat, &mat);
	return S_OK;
}

COLLISION_RESULT CSkinnedMesh::CheckCollision(D3DXMATRIX mat,D3DXVECTOR3 vPos, float fCheckDistance,BOOL bUpdateFrame,BOOL bWithNormal)
{
//	FLOG( "CSkinnedMesh::CheckCollision(D3DXMATRIX mat)" );
	COLLISION_RESULT collResult,checkcollResult;
	SDrawElement *pdeCur;
    pdeCur = m_pdeHead;
//	collResult.fDist = 10000.0f;
//	collResult.vNormalVector = D3DXVECTOR3(0,0,0);
    while (pdeCur != NULL)
    {
		if(bUpdateFrame)
		{
			UpdateFrames(pdeCur->pframeRoot, m_mWorld,vPos,fCheckDistance);
		}
		checkcollResult = CheckCollDist(pdeCur->pframeRoot,mat,vPos,fCheckDistance,bWithNormal);
		if(collResult.fDist > checkcollResult.fDist)
		{
			collResult = checkcollResult;
			
		}
        pdeCur = pdeCur->pdeNext;
    }
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDist(SFrame *pframeCur,D3DXMATRIX mat,D3DXVECTOR3 vPos,float fCheckDistance,BOOL bWithNormal)
{
//	FLOG( "CSkinnedMesh::CheckCollDist(SFrame *pframeCur,D3DXMATRIX mat)" );
	COLLISION_RESULT collResult,checkcollResult;
	SMeshContainer *pmcMesh;
	SFrame *pframeChild;
	if (pframeCur->pmcMesh != NULL)
	{
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &pframeCur->matCombined);
	}
	pmcMesh = pframeCur->pmcMesh;

	D3DXVECTOR3 vMeshCenter;

	while (pmcMesh != NULL)
	{
//		D3DXVec3TransformCoord(&vMeshCenter,&pmcMesh->vCenter,&m_mWorld);				
//		if(D3DXVec3Length(&(vMeshCenter - g_pShuttleChild->m_vPos)) - pmcMesh->fRadius < fCheckDistance )
//		{
//			checkcollResult = CheckCollDistDetail(pmcMesh,mat,bWithNormal);
//			if(collResult.fDist > checkcollResult.fDist)
//				collResult = checkcollResult;
//		}
		checkcollResult = CheckCollDistDetail(pmcMesh,mat,bWithNormal);
		if(collResult.fDist > checkcollResult.fDist)
			collResult = checkcollResult;

		pmcMesh = pmcMesh->pmcNext;
	}
	pframeChild = pframeCur->pframeFirstChild;
	
	while (pframeChild != NULL)
	{
		// 2005-01-05 by jschoi
		pmcMesh = pframeChild->pmcMesh;				
		if(	pmcMesh )
		{
			D3DXVec3TransformCoord(&vMeshCenter,&pmcMesh->vCenter,&m_mWorld);
			if(D3DXVec3Length(&(vMeshCenter - vPos)) - pmcMesh->fRadius < fCheckDistance)
			{
				checkcollResult = CheckCollDist(pframeChild,mat,vPos,fCheckDistance,bWithNormal);
				if(collResult.fDist > checkcollResult.fDist)
					collResult = checkcollResult;
			}
		}
		else
		{
			checkcollResult = CheckCollDist(pframeChild,mat,vPos,fCheckDistance,bWithNormal);
			if(collResult.fDist > checkcollResult.fDist)
				collResult = checkcollResult;
		}

//		checkcollResult = CheckCollDist(pframeChild,mat,fCheckDistance,bWithNormal);
//		if(collResult.fDist > checkcollResult.fDist)
//			collResult = checkcollResult;

		pframeChild = pframeChild->pframeSibling;
	}
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDistDetail(SMeshContainer *pmcMesh,D3DXMATRIX mat,BOOL bWithNormal)
{
//	FLOG( "CSkinnedMesh::CheckCollDistDetail(SMeshContainer *pmcMesh,D3DXMATRIX mat)" );
	COLLISION_RESULT collResult;
//	collResult.fDist = DEFAULT_COLLISION_DISTANCE;
    if (pmcMesh->m_pSkinMesh)
    {
        if (m_method == SOFTWARE)
        {
            D3DXMATRIX  Identity;
            DWORD       cBones  = pmcMesh->m_pSkinMeshInfo->GetNumBones();
            // set up bone transforms
            for (DWORD iBone = 0; iBone < cBones; ++iBone)
            {
                D3DXMatrixMultiply
                (
                    &m_pBoneMatrices[iBone],                 // output
                    &pmcMesh->m_pBoneOffsetMat[iBone], 
                    pmcMesh->m_pBoneMatrix[iBone]
                );
            }
            // set world transform
            D3DXMatrixIdentity(&Identity);
            m_pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);
            // generate skinned mesh
			if(!pmcMesh->pMesh)
			{
//				collResult.fDist = DEFAULT_COLLISION_DISTANCE;
				return collResult;
			}
//            pmcMesh->m_pSkinMesh->UpdateSkinnedMesh(m_pBoneMatrices, NULL, pmcMesh->pMesh);
		}
	}

    BOOL bHit,bIntersections;
    DWORD dwFace;
	FLOAT fBary1, fBary2, fDist;

//	D3DXMATRIX matProj;
	D3DXVECTOR3 vPickRayDir, vPickRayOrig;
	D3DXMATRIX m, matWorld;
//	g_pD3dDev->GetTransform( D3DTS_PROJECTION, &matProj );
	// Get the inverse view matrix
	g_pD3dDev->GetTransform( D3DTS_WORLD, &matWorld );
	m = matWorld*mat;
	D3DXMatrixInverse( &m, NULL, &m );
	vPickRayDir.x = m._31;
	vPickRayDir.y = m._32;	
	vPickRayDir.z = m._33;	
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
	
	D3DXIntersect(pmcMesh->pMesh, &vPickRayOrig, &vPickRayDir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, 
		NULL, NULL);
	if( bHit )
	{
		bIntersections = TRUE;
		m_Intersection.dwFace = dwFace;
		m_Intersection.fBary1 = fBary1;
		m_Intersection.fBary2 = fBary2;
		m_Intersection.fDist = fDist;
	}
	else
	{
		bIntersections = FALSE;
	}
		
	if( bIntersections )
	{
		if(bWithNormal) // 2004-11-03 by jschoi ���� ���Ͱ� �ʿ��Ҷ��� ���Ѵ�.
		{
			WORD*			pIndices;
			D3DVERTEX*		pVertices;
			
			D3DVERTEX vThisTri[3];
			WORD* iThisTri;

//			LPDIRECT3DVERTEXBUFFER9 pVB;
//			LPDIRECT3DINDEXBUFFER9  pIB;	
//			pmcMesh->pMesh->GetVertexBuffer(&pVB);
//			pmcMesh->pMesh->GetIndexBuffer( &pIB );
//			pIB->Lock( 0,0,(void**)&pIndices, 0 );
//			pVB->Lock( 0,0,(void**)&pVertices, 0 );

			pmcMesh->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
			pmcMesh->pMesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndices);

			iThisTri = &pIndices[3*m_Intersection.dwFace];
			// get vertices hit
			vThisTri[0] = pVertices[iThisTri[0]];
			vThisTri[1] = pVertices[iThisTri[1]];
			vThisTri[2] = pVertices[iThisTri[2]];

			pmcMesh->pMesh->UnlockVertexBuffer();
			pmcMesh->pMesh->UnlockIndexBuffer();
//			pVB->Unlock();
//			pIB->Unlock();
			
//			pVB->Release();
//			pIB->Release();	

			D3DXVec3TransformCoord(&vThisTri[0].p, &vThisTri[0].p, &matWorld);
			D3DXVec3TransformCoord(&vThisTri[1].p, &vThisTri[1].p, &matWorld);
			D3DXVec3TransformCoord(&vThisTri[2].p, &vThisTri[2].p, &matWorld);
//			D3DXVec3TransformNormal(&vThisTri[0].n, &vThisTri[0].n, &matWorld);
//			D3DXVec3TransformNormal(&vThisTri[1].n, &vThisTri[1].n, &matWorld);
//			D3DXVec3TransformNormal(&vThisTri[2].n, &vThisTri[2].n, &matWorld);

			// ������ ������.
		
			D3DXVECTOR3 vTempNormal, vNormalVector;
			D3DXVECTOR3 vCross1,vCross2;
			vCross1 = vThisTri[0].p - vThisTri[1].p;
			vCross2 = vThisTri[1].p - vThisTri[2].p;
			D3DXVec3Cross(&vTempNormal,&vCross1,&vCross2);
			D3DXVec3Normalize(&vNormalVector,&vTempNormal);

//			vNormalVector = vThisTri[0].n + vThisTri[1].n + vThisTri[2].n;
//			D3DXVec3Normalize(&vNormalVector,&vNormalVector);

			collResult.vNormalVector = vNormalVector;
		}

		collResult.fDist = fDist;

		return collResult; // ��¼�� ����ü;
	}
	
	return collResult;// ��¼�� ����ü �浹 ����.;

}

void CSkinnedMesh::Render(BOOL bMultiTexture, DWORD nType)
{
	FLOG( "CSkinnedMesh::Render(BOOL bMultiTexture)" );
	if(nType == _SHUTTLE)
		m_unTexColor = 0;

	m_bMultiTexture = bMultiTexture;
    UINT cTriangles = 0;
    HRESULT hr;
    SDrawElement *pdeCur;
    D3DXVECTOR3 vTemp;
    pdeCur = m_pdeHead;
    while (pdeCur != NULL)
    {
		hr = UpdateFrames(pdeCur->pframeRoot, m_mWorld);
        if (FAILED(hr))
            return ;
        hr = DrawFrames(pdeCur->pframeRoot, cTriangles, nType);
        if (FAILED(hr))
            return ;

        pdeCur = pdeCur->pdeNext;
    }
}

// by dhkwon, 030917, �ؽ��� ���� ������ ���� ���, ������ �Ŀ��� ���Լ��� ���� ������� ������ �Ѵ�.
LPDIRECT3DTEXTURE9 CSkinnedMesh::SetTexture(LPDIRECT3DTEXTURE9 pTexture
											, BYTE tex_num)
{
	FLOG( "CSkinnedMesh::SetTexture(LPDIRECT3DTEXTURE8 pTexture, BYTE tex_num)" );
	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	if(FALSE == IsLoadTexture(tex_num))
	{
		tex_num = 0;
	}
	// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����

	LPDIRECT3DTEXTURE9 pTemp = m_pTexture[tex_num];
	m_pTexture[tex_num] = pTexture;
	return pTemp;
}

HRESULT CSkinnedMesh::LoadMeshHierarchy(char* szFileName)
{
	FLOG( "CSkinnedMesh::LoadMeshHierarchy(char* szFileName)" );
    TCHAR* pszFile = szFileName;
    SDrawElement *pdeMesh = NULL;
    HRESULT hr = S_OK;
    LPDIRECTXFILE pxofapi = NULL;
    LPDIRECTXFILEENUMOBJECT pxofenum = NULL;
    LPDIRECTXFILEDATA pxofobjCur = NULL;
    DWORD dwOptions;
    int cchFileName;

    if (pszFile == NULL)
        return E_INVALIDARG;
    
    pdeMesh = new SDrawElement();
    
    delete pdeMesh->pframeRoot;
    pdeMesh->pframeAnimHead = NULL;
    
    pdeMesh->pframeRoot = new SFrame();
    if (pdeMesh->pframeRoot == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    
    
    dwOptions = D3DXMESH_MANAGED;
    
    cchFileName = strlen(pszFile);
    if (cchFileName < 2)
    {
        hr = E_FAIL;
        goto e_Exit;
    }
    
    hr = DirectXFileCreate(&pxofapi);
    if (FAILED(hr))
        goto e_Exit;  
    
    // Register templates for d3drm.
    hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES,
        D3DRM_XTEMPLATE_BYTES);
    if (FAILED(hr))
        goto e_Exit;
 

    hr = pxofapi->RegisterTemplates((LPVOID)szTemplates,
        strlen(szTemplates));
    if (FAILED(hr))
        goto e_Exit;

	
    // Create enum object.
    hr = pxofapi->CreateEnumObject((LPVOID)pszFile,
        DXFILELOAD_FROMFILE,
        &pxofenum);
    if (FAILED(hr))
        goto e_Exit;
     
    
    // Enumerate top level objects.
    // Top level objects are always data object.
    while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))
    {
        hr = LoadFrames(pxofobjCur, pdeMesh, dwOptions, m_dwFVF,
            m_pd3dDevice,
            pdeMesh->pframeRoot);
        GXRELEASE(pxofobjCur);
        
        if (FAILED(hr))
            goto e_Exit;
    }
    
    hr = FindBones(pdeMesh->pframeRoot, pdeMesh);
    if (FAILED(hr))
        goto e_Exit;
    
    
    delete []pdeMesh->szName;
    pdeMesh->szName = new char[cchFileName+1];
    if (pdeMesh->szName == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memcpy(pdeMesh->szName, pszFile, cchFileName+1);

    // delete the current mesh, now that the load has succeeded
    DeleteSelectedMesh();

    // link into the draw list
    pdeMesh->pdeNext = m_pdeHead;
    m_pdeHead = pdeMesh;
    
    m_pdeSelected = pdeMesh;
    m_pmcSelectedMesh = pdeMesh->pframeRoot->pmcMesh;
    
    
    m_pframeSelected = pdeMesh->pframeRoot;
    
    hr = CalculateBoundingSphere(pdeMesh);
	m_fRadius = pdeMesh->fRadius;
	m_vCenter = pdeMesh->vCenter;

    if (FAILED(hr))
        goto e_Exit;
    
    SetProjectionMatrix();
    
    m_pdeSelected->fCurTime = 0.0f;
    m_pdeSelected->fMaxTime = 2000.0f;
    
    D3DXMatrixTranslation(&m_pdeSelected->pframeRoot->matRot,
        -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);
    m_pdeSelected->pframeRoot->matRotOrig = m_pdeSelected->pframeRoot->matRot;
    
e_Exit:
// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
//     GXRELEASE(pxofobjCur);
//     GXRELEASE(pxofenum);
//     GXRELEASE(pxofapi);
//     
//     if (FAILED(hr))
//     {
// 		// ���� �߻�
// 		g_pD3dApp->CheckError(CLIENT_ERR_MESHLOAD);  
//         delete pdeMesh;
//     }
// 
//     SDrawElement *pdeCur;
//     pdeCur = m_pdeHead;
//     while (pdeCur != NULL)
//     {
//         D3DXMatrixIdentity(&pdeCur->pframeRoot->matRot);
//         D3DXMatrixIdentity(&pdeCur->pframeRoot->matTrans);
//         pdeCur = pdeCur->pdeNext;
//     }
	ReleaseData(hr);
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
    return hr;
}

//HRESULT CSkinnedMesh::LoadMeshHierarchyFromMem(CGameData* pGameData)
HRESULT CSkinnedMesh::LoadMeshHierarchyFromMem(CGameData* pGameData, int nMeshIndex, D3DXVECTOR3 vObjScale)	// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
{
	FLOG( "CSkinnedMesh::LoadMeshHierarchyFromMem(CGameData* pGameData)" );
//	char strPath[256];
//	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "decal.tex");
//	SetResourceFile(strPath);
// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
// 	DataHeader	* pDataHeader = NULL;
// 	char buf[32];
// 	memset(buf, 0x00, 32);
// 	
// 	SDrawElement *pdeMesh = NULL;
// 	HRESULT hr = S_OK;
// 	LPDIRECTXFILE pxofapi = NULL;
// 	LPDIRECTXFILEENUMOBJECT pxofenum = NULL;
// 	LPDIRECTXFILEDATA pxofobjCur = NULL;
// 	LPDXFILELOADMEMORY pMemory = NULL;
// 	DWORD dwOptions;
	char buf[32];
	memset(buf, 0x00, 32);
	HRESULT hr = S_OK;
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����	

    pdeMesh = new SDrawElement();
	pMemory = new DXFILELOADMEMORY;
    
    delete pdeMesh->pframeRoot;
    pdeMesh->pframeAnimHead = NULL;
    
    pdeMesh->pframeRoot = new SFrame();

	// object file loading
	char* pPointer = NULL;
	m_pGameData = pGameData;

	// 2007-11-08 by bhsohn �κ� ����Ʈ ���� ó��
	UpdateMeshObjName(m_szMeshObjFileName);

	int totalNum = m_pGameData->GetTotalNumber();
	m_pDataHeader = new DataHeader[totalNum];
	
	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����			
	{
		SAFE_DELETE_ARRAY(m_pLoadMem);		
		m_pLoadMem = new BOOL[totalNum];
		int nCnt = 0;
		for(nCnt = 0;nCnt <totalNum;nCnt++)
		{
			m_pLoadMem[nCnt] = TRUE;
		}
		// 2007-10-18 by bhsohn �� �ƸӴ� ������ �� �ε�
		// �� �ƸӴ� ������ �� �ε�
		BOOL bMyArmor = FALSE;
		if(IsArmorMesh(nMeshIndex))
		{
			if(_SELECT == g_pD3dApp->GetGameState())				
			{
				bMyArmor = TRUE;
			}
			else if(NULL != g_pShuttleChild && nMeshIndex == g_pShuttleChild->GetUnitNum())
			{
				bMyArmor = TRUE;
			}
		}
		// end 2007-10-18 by bhsohn �� �ƸӴ� ������ �� �ε�

		// 2009-03-09 by bhsohn �Ϻ��� �⺻ �ƸӸ� �ε�
		BOOL bDefaultArmor = (IsArmorMesh(nMeshIndex) && IsLowQuality() && (FALSE == bMyArmor));
		if(IsArmorMesh(nMeshIndex))
			bDefaultArmor = TRUE;

		// end 2009. 07. 27 by ckPark ���絵 �⺻ �Ƹ� �ؽ��ĸ� �ε��ϵ��� ����
		
		// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		// �κ��Ƹ� �ؽ��� 2�� ���
		if( IsArmorMesh( nMeshIndex ) )
		{
			char szArmorNum[32];
			itoa( nMeshIndex, szArmorNum, 10 );
			szArmorNum[0] = '0';
			szArmorNum[1] = '8';
			
			int  nArmorItemNum = atoi( szArmorNum );
			
			ITEM* pItem = g_pDatabase->GetServerItemInfo( nArmorItemNum );
			if( pItem && COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) )
				bDefaultArmor = FALSE;
		}
		// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		
		//if(IsArmorMesh(nMeshIndex) && IsLowQuality() && (FALSE == bMyArmor))
		if(bDefaultArmor)
		{			
			for(nCnt = 1;nCnt <totalNum-2;nCnt++)
			{
				// ù���� �ƸӸ� ���� ��� Disable
				m_pLoadMem[nCnt] = FALSE;
			}
		}
		// end 2009-03-09 by bhsohn �Ϻ��� �⺻ �ƸӸ� �ε�
	}
	// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	DataHeader* pHeader = m_pGameData->GetStartPosition();
	int i=0;
	while(pHeader)
	{
		m_pDataHeader[i] = *pHeader;
		m_pDataHeader[i++].m_pData = pHeader->m_pData;
		pHeader = m_pGameData->GetNext();
	}

    if (pdeMesh->pframeRoot == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    
//    dwOptions = 0;
	dwOptions = D3DXMESH_MANAGED;

    hr = DirectXFileCreate(&pxofapi);
    if (FAILED(hr))
        goto e_Exit;  
    
    // Register templates for d3drm.
    hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES,
        D3DRM_XTEMPLATE_BYTES);
    if (FAILED(hr))
        goto e_Exit;

    hr = pxofapi->RegisterTemplates((LPVOID)szTemplates,
        strlen(szTemplates));
    if (FAILED(hr))
        goto e_Exit;

	pMemory->lpMemory = m_pDataHeader[0].m_pData;
	pMemory->dSize = m_pDataHeader[0].m_DataSize;
    // Create enum object.
    hr = pxofapi->CreateEnumObject((LPVOID)pMemory,
        DXFILELOAD_FROMMEMORY,
        &pxofenum);
    if (FAILED(hr))
        goto e_Exit;
    // Create enum texture
	if(totalNum > 1)
	{
		m_pTexture = new LPDIRECT3DTEXTURE9[totalNum-1];
		m_bTotalTextureNum = totalNum-1;
		for(i=1; i<totalNum; i++)
		{
			// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
			int nLoadIdx = i-1;
			if(FALSE == IsLoadTexture(nLoadIdx))
			{
				continue;
			}
			// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����			
			
			if( FAILED( D3DXCreateTextureFromFileInMemory(m_pd3dDevice, (LPCVOID)m_pDataHeader[i].m_pData, 
				m_pDataHeader[i].m_DataSize, &(m_pTexture[i-1])))) 
			{
				hr = E_FAIL;
				if(g_pD3dApp && g_pD3dApp->m_pChat)
				{
					char errStr[512];
					sprintf_s(errStr, 512, STRERR_C_RESOURCE_0012, m_pDataHeader[i].m_FileName);
					g_pD3dApp->m_pChat->CreateChatChild(errStr, COLOR_ERROR);// "���ҽ� ���� �ε��� ������ �־ ������Ʈ ������ �ߴܵ�."
				}
				DBGOUT("\n ERROR : Loading Resource File .\n\n");
				goto e_Exit;
			}
		}
	}
	else
	{
		m_pTexture = new LPDIRECT3DTEXTURE9[1];
		m_pTexture[0] = NULL;
	}

	// ��Į �׽�Ʈ �κ�.
//	m_pDecalTextures = new LPDIRECT3DTEXTURE9[MAX_DECAL_NUM];
//	for(i=0;i<MAX_DECAL_NUM;i++)
//	{
//		wsprintf( buf, "de%04d", i+1);
//		pDataHeader = FindResource(buf);
//		if(pDataHeader == NULL)	{
////			DBGOUT("\n ERROR : Decal Loading Resource File Not Found.\n\n");
//			break;
//		}
//		
//		if( FAILED( D3DXCreateTextureFromFileInMemory(m_pd3dDevice, (LPCVOID)pDataHeader->m_pData, 
//			pDataHeader->m_DataSize, &m_pDecalTextures[i]))) 
//		{
//			hr = E_FAIL;
//			if(g_pD3dApp && g_pD3dApp->m_pChat)
//			{
//				g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_RESOURCE_0012,COLOR_ERROR);// "���ҽ� ���� �ε��� ������ �־ ������Ʈ ������ �ߴܵ�."
//			}
//			DBGOUT("\n ERROR : Decal Loading Resource File .\n\n");
//			goto e_Exit;
//		}
//	}
//
	ASSERT_ASSERT(m_pTexture);
//	ASSERT_ASSERT(m_pDecalTextures);

    // Enumerate top level objects.
    // Top level objects are always data object.
    while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))
    {
        hr = LoadFrames(pxofobjCur, pdeMesh, dwOptions, m_dwFVF,
            m_pd3dDevice,
            pdeMesh->pframeRoot);
        GXRELEASE(pxofobjCur);
        
        if (FAILED(hr))
            goto e_Exit;
    }
    
    hr = FindBones(pdeMesh->pframeRoot, pdeMesh);
    if (FAILED(hr))
        goto e_Exit;
    
    
    delete []pdeMesh->szName;
	pdeMesh->szName = NULL;

    // delete the current mesh, now that the load has succeeded
    DeleteSelectedMesh();

    // link into the draw list
    pdeMesh->pdeNext = m_pdeHead;
    m_pdeHead = pdeMesh;
    
    m_pdeSelected = pdeMesh;
    m_pmcSelectedMesh = pdeMesh->pframeRoot->pmcMesh;
    
    
    m_pframeSelected = pdeMesh->pframeRoot;
    
    
	hr = CalculateBoundingSphere(pdeMesh,vObjScale);		// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
  	m_fRadius = pdeMesh->fRadius;
	m_vCenter = pdeMesh->vCenter;

	if (FAILED(hr))
        goto e_Exit;
    
    SetProjectionMatrix();
    
    m_pdeSelected->fCurTime = 0.0f;
    m_pdeSelected->fMaxTime = 2000.0f;
    
    D3DXMatrixTranslation(&m_pdeSelected->pframeRoot->matRot,
        -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);
    m_pdeSelected->pframeRoot->matRotOrig = m_pdeSelected->pframeRoot->matRot;
    
e_Exit:
// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
//	SAFE_DELETE(m_pGameData);// m_pDataHeader�� m_pData�� ��ȿ�ϰ�, �������� ��ȿ�ϴ�.
// 	SAFE_DELETE(pMemory);
// 	for(i=0; i<totalNum; i++)
// 		m_pDataHeader[i].m_pData = NULL;
//     GXRELEASE(pxofobjCur);
//     GXRELEASE(pxofenum);
//     GXRELEASE(pxofapi);
//     
//     if (FAILED(hr))
//     {
// 		// ���� �߻�
//      delete pdeMesh;
// 		pdeMesh = NULL;
//     }
// 
//     SDrawElement *pdeCur;
//     pdeCur = m_pdeHead;
//     while (pdeCur != NULL)
//     {
//         D3DXMatrixIdentity(&pdeCur->pframeRoot->matRot);
//         D3DXMatrixIdentity(&pdeCur->pframeRoot->matTrans);
//         pdeCur = pdeCur->pdeNext;
//     }
	ReleaseData(hr);
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
    return hr;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
/// \date		2007-08-29 ~ 2007-08-29
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkinnedMesh::IsArmorMesh(int nMeshIndex)
{
	if(nMeshIndex >= 12000000 && nMeshIndex < 13000000)
	{
		return TRUE;
	}
	return FALSE;
}

HRESULT CSkinnedMesh::LoadTexture(char* szfileName, 
								  BYTE tex_num)
{
	FLOG( "CSkinnedMesh::LoadTexture(char* szfileName, BYTE tex_num)" );
	if( FAILED( D3DXCreateTextureFromFile(m_pd3dDevice, szfileName, &m_pTexture[tex_num-1]))) return E_FAIL;
		return TRUE;
}

HRESULT CSkinnedMesh::LoadFrames(LPDIRECTXFILEDATA pxofobjCur, 
								 SDrawElement *pde, 
								 DWORD options, DWORD fvf, 
								 LPDIRECT3DDEVICE9 pD3DDevice, 
								 SFrame *pframeParent)
{
//	FLOG( "CSkinnedMesh::LoadFrames(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)" );
    HRESULT hr = S_OK;
    LPDIRECTXFILEDATA pxofobjChild = NULL;
    LPDIRECTXFILEOBJECT pxofChild = NULL;
    const GUID *type;
    DWORD cbSize;
    D3DXMATRIX *pmatNew;
    SFrame *pframeCur;
    DWORD cchName;
    
    // Get the type of the object
    hr = pxofobjCur->GetType(&type);
    if (FAILED(hr))
        goto e_Exit;
    
    if (*type == TID_D3DRMMesh)
    {
        hr = LoadMesh(pxofobjCur, options, fvf, pD3DDevice, pframeParent);
        if (FAILED(hr))
            goto e_Exit;
    }
    else if (*type == TID_D3DRMFrameTransformMatrix)
    {
        hr = pxofobjCur->GetData(NULL, &cbSize, (PVOID*)&pmatNew);
        if (FAILED(hr))
            goto e_Exit;
        
        // update the parents matrix with the new one
        pframeParent->matRot = *pmatNew;
        pframeParent->matRotOrig = *pmatNew;
    }
    else if (*type == TID_D3DRMAnimationSet)
    {
        LoadAnimationSet(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
    }
    else if (*type == TID_D3DRMAnimation)
    {
        LoadAnimation(pxofobjCur, pde, options, fvf, pD3DDevice, pframeParent);
    }
    else if (*type == TID_D3DRMFrame)
    {
        pframeCur = new SFrame();
        if (pframeCur == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
        
        hr = pxofobjCur->GetName(NULL, &cchName);
        if (FAILED(hr))
            goto e_Exit;
        
        if (cchName > 0)
        {
            pframeCur->szName = new char[cchName];
            if (pframeCur->szName == NULL)
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
            
            hr = pxofobjCur->GetName(pframeCur->szName, &cchName);
            if (FAILED(hr))
                goto e_Exit;
        }
        
        pframeParent->AddFrame(pframeCur);
        
        // Enumerate child objects.
        // Child object can be data, data reference or binary.
        // Use QueryInterface() to find what type of object a child is.
        while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))
        {
            // Query the child for it's FileData
            hr = pxofChild->QueryInterface(IID_IDirectXFileData,
                (LPVOID *)&pxofobjChild);
            if (SUCCEEDED(hr))
            {
                hr = LoadFrames(pxofobjChild, pde, options, fvf, pD3DDevice, pframeCur);
                if (FAILED(hr))
                    goto e_Exit;
                
                GXRELEASE(pxofobjChild);
            }
            
            GXRELEASE(pxofChild);
        }
        
    }
	
e_Exit:
    GXRELEASE(pxofobjChild);
    GXRELEASE(pxofChild);
    return hr;
}

HRESULT CSkinnedMesh::LoadMesh(LPDIRECTXFILEDATA pxofobjCur,
                                    DWORD options, DWORD fvf, 
									LPDIRECT3DDEVICE9 pD3DDevice,
                                    SFrame *pframeParent)
{
//	FLOG( "CSkinnedMesh::LoadMesh(LPDIRECTXFILEDATA pxofobjCur, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)" );
    HRESULT hr = S_OK;
    SMeshContainer *pmcMesh = NULL;
    LPD3DXBUFFER pbufMaterials = NULL;
    LPD3DXBUFFER pbufAdjacency = NULL;
    DWORD cchName;
    UINT cFaces;
    UINT iMaterial;
    LPDIRECT3DDEVICE9 m_pDevice = m_pd3dDevice;
    LPDWORD pAdjacencyIn;
//    LPD3DXPMESH  pPMesh = NULL;
 //   LPD3DXPMESH  pPMeshTemp = NULL;
 //   LPD3DXMESH   pMesh = NULL;
//    LPD3DXMESH   pTempMesh = NULL;
	int i;  
//	DWORD num;

	// 2005-01-05 by jschoi
	UINT iBone, cBones;
  
    pmcMesh = new SMeshContainer();
//	for(i = 0;i < 5;i++)
//		pmcMesh->pMeshes[i] = NULL;
	if (pmcMesh == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    
    hr = pxofobjCur->GetName(NULL, &cchName);
    if (FAILED(hr))
        goto e_Exit;
    
    if (cchName > 0)
    {
        pmcMesh->szName = new char[cchName];
        if (pmcMesh->szName == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
        
        hr = pxofobjCur->GetName(pmcMesh->szName, &cchName);
        if (FAILED(hr))
            goto e_Exit;
    }
    
	// 2005-01-04 by jschoi
//    hr = D3DXLoadSkinMeshFromXof(pxofobjCur, options, pD3DDevice, &pbufAdjacency, &pbufMaterials, &pmcMesh->cMaterials, 
//        &pmcMesh->m_pBoneNamesBuf, &pmcMesh->m_pBoneOffsetBuf, &pmcMesh->m_pSkinMesh);
    hr = D3DXLoadSkinMeshFromXof(pxofobjCur, options, pD3DDevice, &pbufAdjacency, &pbufMaterials, NULL, &pmcMesh->cMaterials, 
        &pmcMesh->m_pSkinMeshInfo, &pmcMesh->m_pSkinMesh);


//	DWORD num;//,num2,num3;

    if (FAILED(hr))
    {
        if (hr == D3DXERR_LOADEDMESHASNODATA)
            hr = S_OK;
        goto e_Exit;
    }
	
	
    cFaces = pmcMesh->m_pSkinMesh->GetNumFaces();

    pAdjacencyIn = static_cast<LPDWORD>(pbufAdjacency->GetBufferPointer());

    pmcMesh->m_rgiAdjacency = new DWORD[cFaces * 3];

    if (pmcMesh->m_rgiAdjacency == NULL)
    {
        hr = E_OUTOFMEMORY;

        goto e_Exit;
    }

    memcpy(pmcMesh->m_rgiAdjacency, pAdjacencyIn, cFaces * 3 * sizeof(DWORD));

    // Process skinning data
    if (pmcMesh->m_pSkinMeshInfo->GetNumBones())
    {
		cBones = pmcMesh->m_pSkinMeshInfo->GetNumBones();
        // Update max bones of any mesh in the app
        m_maxBones = max(cBones, m_maxBones);
        
        pmcMesh->m_pBoneMatrix = new D3DXMATRIX*[cBones];
		pmcMesh->m_pBoneOffsetMat = new D3DXMATRIX[cBones];
        if (pmcMesh->m_pBoneMatrix == NULL)
            goto e_Exit;
		// 2005-01-05 by jschoi
//      pmcMesh->m_pBoneOffsetMat = reinterpret_cast<D3DXMATRIX*>(pmcMesh->m_pBoneOffsetBuf->GetBufferPointer());
        for (iBone = 0; iBone < cBones; iBone++)
        {
			pmcMesh->m_pBoneOffsetMat[iBone] = *(pmcMesh->m_pSkinMeshInfo->GetBoneOffsetMatrix(iBone));
        }
        
        hr = GenerateMesh(pmcMesh);

        if (FAILED(hr))
            goto e_Exit;
		/////////////////////// ������Ÿ�� �����ϰ� Temp Mesh�� �����ϰ� Progressive �� �����Ѵ� /////////////////////////////////////
//		if(m_bProgressiveMesh)
//		{
//	        pmcMesh->m_pSkinMesh->GetOriginalMesh(&pTempMesh);
//			D3DXCleanMesh(pTempMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), &pMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), NULL );
//		}
    }
    else
    {
		// 2005-01-04 by jschoi
//        pmcMesh->m_pSkinMesh->GetOriginalMesh(&(pmcMesh->pMesh));
//		pmcMesh->m_pOrigMesh = pmcMesh->pMesh;
		pmcMesh->pMesh = pmcMesh->m_pSkinMesh;
//        pmcMesh->m_pSkinMesh->Release();
        pmcMesh->m_pSkinMesh = NULL;
		SAFE_RELEASE(pmcMesh->m_pSkinMeshInfo);
		pmcMesh->m_pSkinMeshInfo = NULL;
		
        pmcMesh->cpattr = pmcMesh->cMaterials;
		/////////////////////// ������Ÿ�� ������ Temp Mesh�� �����ϰ� Progressive �� �����Ѵ� ///////////////////////////////////////
//		if(m_bProgressiveMesh)
//			D3DXCleanMesh(pmcMesh->pMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), &pMesh, (DWORD*)pbufAdjacency->GetBufferPointer(), NULL );
    }
    if ((pbufMaterials == NULL) || (pmcMesh->cMaterials == 0))
    {
        pmcMesh->rgMaterials = new D3DMATERIAL9[1];
        pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[1];
        if (pmcMesh->rgMaterials == NULL || pmcMesh->pTextures == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
        
        memset(pmcMesh->rgMaterials, 0, sizeof(D3DMATERIAL9));
        pmcMesh->rgMaterials[0].Diffuse.r = 1.0f;
        pmcMesh->rgMaterials[0].Diffuse.g = 1.0f;
        pmcMesh->rgMaterials[0].Diffuse.b = 1.0f;
        pmcMesh->rgMaterials[0].Ambient = pmcMesh->rgMaterials[0].Diffuse;
        pmcMesh->pTextures[0] = NULL;
    }
    else
    {
        pmcMesh->rgMaterials = new D3DMATERIAL9[pmcMesh->cMaterials];
        pmcMesh->pTextures = new LPDIRECT3DTEXTURE9[pmcMesh->cMaterials];
        if (pmcMesh->rgMaterials == NULL || pmcMesh->pTextures == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
        
        LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)pbufMaterials->GetBufferPointer();
        
        for (iMaterial = 0; iMaterial < pmcMesh->cMaterials; iMaterial++)
        {
            
            pmcMesh->rgMaterials[iMaterial] = pMaterials[iMaterial].MatD3D;
			pmcMesh->rgMaterials[iMaterial].Ambient.r = 1.0f;
 			pmcMesh->rgMaterials[iMaterial].Ambient.g = 1.0f;
 			pmcMesh->rgMaterials[iMaterial].Ambient.b = 1.0f;
          
            pmcMesh->pTextures[iMaterial] = NULL;
            if (pMaterials[iMaterial].pTextureFilename != NULL)
            {
//				ChangeFilePath(pMaterials[iMaterial].pTextureFilename);
				if(m_pDataHeader && m_pGameData)
				{
					int num = m_pGameData->GetTotalNumber();
					for(i=1; i<num; i++ )
					{
						// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
						int nLoadIdx = i-1;
						if(FALSE == IsLoadTexture(nLoadIdx))
						{
							continue;
						}
						// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����

//						ChangeFilePath(m_pDataHeader[i].m_FileName);
						int nCount = strlen(pMaterials[iMaterial].pTextureFilename) - strlen(m_pDataHeader[i].m_FileName);
						if(strcmp(m_pDataHeader[i].m_FileName, pMaterials[iMaterial].pTextureFilename) == 0 && m_pTexture)
						{
							// 2005-03-17 by jschoi - ��üũ
							if(m_pTexture[i-1])
							{
								m_pTexture[i-1]->AddRef();
							}
							else
							{
								DBGOUT("Texture is NULL.\n");
							}
							pmcMesh->pTextures[iMaterial] = m_pTexture[i-1];
							break;
						}
					}

				}
				else 
				{
					TCHAR szPath[MAX_PATH];
				//	DXUtil_FindMediaFile(szPath, pMaterials[iMaterial].pTextureFilename);
					DXUtil_FindMediaFileCb(szPath,sizeof(szPath),pMaterials[iMaterial].pTextureFilename);
					hr = D3DXCreateTextureFromFile(m_pDevice, szPath, &(pmcMesh->pTextures[iMaterial]));
					if (FAILED(hr))
						pmcMesh->pTextures[iMaterial] = NULL;
				}
            }
        }
    }
    // add the mesh to the parent frame
    pframeParent->AddMesh(pmcMesh);
/* 	// �ӽ�
	if(m_bProgressiveMesh)
	{
		DWORD maxnum = pMesh->GetNumVertices();
		DWORD minnum = maxnum/10;
		num = (DWORD)((maxnum-minnum)/5);
		for(i = 0;i < 5;i++)
			D3DXSimplifyMesh(pMesh,(DWORD*)pbufAdjacency->GetBufferPointer(),NULL,NULL,(maxnum-(num*(i+1))),D3DXMESHSIMP_VERTEX,&pmcMesh->pMeshes[i]);
		SAFE_RELEASE(pTempMesh);
		SAFE_RELEASE(pPMesh);
	}
*/	pmcMesh = NULL;
   
e_Exit:
	SAFE_DELETE(pmcMesh);	// 2006-07-05 by ispark
//    delete pmcMesh;
//	pmcMesh = NULL;
    
//    SAFE_RELEASE( pMesh );
//    SAFE_RELEASE( pPMesh );
    GXRELEASE(pbufAdjacency);
    GXRELEASE(pbufMaterials);
    return hr;
}


HRESULT CSkinnedMesh::GenerateMesh(SMeshContainer *pmcMesh)
{
//	FLOG( "CSkinnedMesh::GenerateMesh(SMeshContainer *pmcMesh)" );
	// ASSUMPTION:  pmcMesh->m_rgiAdjacency contains the current adjacency
	HRESULT hr = S_OK;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	DWORD cFaces = pmcMesh->m_pSkinMesh->GetNumFaces();
//	DWORD* rgiAdjacency;

	hr  = pmcMesh->m_pSkinMesh->GetDevice(&pDevice);
	if (FAILED(hr))
		goto e_Exit;

	GXRELEASE(pmcMesh->pMesh);
	delete [] m_pBoneMatrices;

	pmcMesh->pMesh      = NULL;
	m_pBoneMatrices     = NULL;
    
	if (m_method == D3DNONINDEXED)
	{
		LPD3DXBONECOMBINATION   rgBoneCombinations;

		hr = pmcMesh->m_pSkinMeshInfo->ConvertToBlendedMesh
                                   (
									   pmcMesh->m_pSkinMesh,
                                       D3DXMESH_MANAGED|D3DXMESHOPT_VERTEXCACHE, 
                                       pmcMesh->m_rgiAdjacency, 
                                       NULL, 
                                       NULL,	//&pmcMesh->cpattr
									   NULL,
									   &pmcMesh->m_maxFaceInfl,
									   &pmcMesh->m_numBoneComb,
                                       &pmcMesh->m_pBoneCombinationBuf, 
                                       &pmcMesh->pMesh
                                   );
		
		if (FAILED(hr))
		{
			goto e_Exit;
		}

		// calculate the max face influence count

		if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
		{
			pmcMesh->m_maxFaceInfl = 1 + ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
		}
		else
		{
			pmcMesh->m_maxFaceInfl = 1;
		}

		/* If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
			Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
			drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing. */
		if (m_d3dCaps.MaxVertexBlendMatrices <= 2)       
		{
			// calculate the index of the attribute table to split on
			rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());

			for (pmcMesh->iAttrSplit = 0; pmcMesh->iAttrSplit < pmcMesh->cpattr; pmcMesh->iAttrSplit++)
			{
                DWORD   cInfl   = 0;

                for (DWORD iInfl = 0; iInfl < pmcMesh->m_maxFaceInfl; iInfl++)
                {
                    if (rgBoneCombinations[pmcMesh->iAttrSplit].BoneId[iInfl] != UINT_MAX)
                    {
                        ++cInfl;
                    }
                }

                if (cInfl > m_d3dCaps.MaxVertexBlendMatrices)
                {
                    break;
                }
            }

            // if there is both HW and SW, add the Software Processing flag
            if (pmcMesh->iAttrSplit < pmcMesh->cpattr)
            {
                LPD3DXMESH pMeshTmp;
                hr = pmcMesh->pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING | pmcMesh->pMesh->GetOptions(), 
                                                    pmcMesh->pMesh->GetFVF(),
                                                    pDevice, &pMeshTmp);
                if (FAILED(hr))
                {
                    goto e_Exit;
                }

                pmcMesh->pMesh->Release();
                pmcMesh->pMesh = pMeshTmp;
                pMeshTmp = NULL;
            }
        }
        else
        {
            pmcMesh->iAttrSplit = pmcMesh->cpattr;
        }
    }
    else if (m_method == D3DINDEXEDVS)
    {
        // Get palette size
        pmcMesh->m_paletteSize = min(28, pmcMesh->m_pSkinMeshInfo->GetNumBones());

        DWORD flags = D3DXMESHOPT_VERTEXCACHE;
        if (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
        {
            pmcMesh->m_bUseSW = false;
            flags |= D3DXMESH_MANAGED;
        }
        else
        {
            pmcMesh->m_bUseSW = true;
            flags |= D3DXMESH_SYSTEMMEM;
        }

        hr = pmcMesh->m_pSkinMeshInfo->ConvertToIndexedBlendedMesh(pmcMesh->m_pSkinMesh,
																flags, 
																pmcMesh->m_paletteSize, 
																pmcMesh->m_rgiAdjacency,
																NULL, 
																NULL, //&pmcMesh->cpattr
																NULL,
																&pmcMesh->m_maxFaceInfl,
																&pmcMesh->m_numBoneComb,
																&pmcMesh->m_pBoneCombinationBuf, 
																&pmcMesh->pMesh);
        if (FAILED(hr))
            goto e_Exit;

        if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
        {
            pmcMesh->m_maxFaceInfl = ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
        }
        else
        {
            pmcMesh->m_maxFaceInfl = 1;
        }

        // FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
        DWORD newFVF = (pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
        if (newFVF != pmcMesh->pMesh->GetFVF())
        {
            LPD3DXMESH pMesh;
            hr = pmcMesh->pMesh->CloneMeshFVF(pmcMesh->pMesh->GetOptions(), newFVF, pDevice, &pMesh);
            if (!FAILED(hr))
            {
                pmcMesh->pMesh->Release();
                pmcMesh->pMesh = pMesh;
                pMesh = NULL;
            }
        }
    }
    else if (m_method == D3DINDEXED)
    {
        DWORD maxFaceInfl;
        DWORD flags = D3DXMESHOPT_VERTEXCACHE;

		// 2005-01-04 by jschoi
//        hr = pmcMesh->m_pSkinMesh->GetMaxFaceInfluences(&maxFaceInfl);
//        if (FAILED(hr))
//            goto e_Exit;
		maxFaceInfl = pmcMesh->m_maxFaceInfl;

        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
        // can be handled
        maxFaceInfl = min(maxFaceInfl, 12);

        if (m_d3dCaps.MaxVertexBlendMatrixIndex + 1 < maxFaceInfl)
        {
            // HW does not support indexed vertex blending. Use SW instead
            pmcMesh->m_paletteSize = min(256, pmcMesh->m_pSkinMeshInfo->GetNumBones());
            pmcMesh->m_bUseSW = true;
            flags |= D3DXMESH_SYSTEMMEM;
        }
        else
        {
            pmcMesh->m_paletteSize = min(m_d3dCaps.MaxVertexBlendMatrixIndex + 1, pmcMesh->m_pSkinMeshInfo->GetNumBones());
            pmcMesh->m_bUseSW = false;
            flags |= D3DXMESH_MANAGED;
        }

        hr = pmcMesh->m_pSkinMeshInfo->ConvertToIndexedBlendedMesh(pmcMesh->m_pSkinMesh,
																	flags, 
																	pmcMesh->m_paletteSize, 
																	pmcMesh->m_rgiAdjacency, 
																	NULL,
																	NULL, //&pmcMesh->cpattr
																	NULL,
																	&pmcMesh->m_maxFaceInfl,
																	&pmcMesh->m_numBoneComb,
																	&pmcMesh->m_pBoneCombinationBuf, 
																	&pmcMesh->pMesh);
        if (FAILED(hr))
            goto e_Exit;

        // Here we are talking of max vertex influence which we determine from 
        // the FVF of the returned mesh
        if ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
        {
            pmcMesh->m_maxFaceInfl = ((pmcMesh->pMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
        }
        else
        {
            pmcMesh->m_maxFaceInfl = 1;
        }
    }
    else if (m_method == SOFTWARE)
    {
		// 2005-01-04 by jschoi
//        hr = pmcMesh->m_pSkinMeshInfo->GenerateSkinnedMesh
//                                   (
//                                       D3DXMESH_DYNAMIC,          // options
//                                       0.0f,                        // minimum bone weight allowed
//                                       pmcMesh->m_rgiAdjacency,     // adjacency of in-mesh
//                                       NULL,     // adjacency of out-mesh
//                                       NULL,     // face remap array
//                                       NULL,     // vertex remap buffer
//                                       &pmcMesh->pMesh              // out-mesh
//                                   );
        hr = pmcMesh->m_pSkinMesh->CloneMeshFVF(D3DXMESH_DYNAMIC, pmcMesh->m_pSkinMesh->GetFVF(),	
                                            m_pd3dDevice, &pmcMesh->pMesh);

        if (FAILED(hr))
            goto e_Exit;

        hr = pmcMesh->pMesh->GetAttributeTable(NULL, &pmcMesh->cpattr);
        if (FAILED(hr))
            goto e_Exit;

        delete[] pmcMesh->m_pAttrTable;
        pmcMesh->m_pAttrTable  = new D3DXATTRIBUTERANGE[pmcMesh->cpattr];
        if (pmcMesh->m_pAttrTable == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        hr = pmcMesh->pMesh->GetAttributeTable(pmcMesh->m_pAttrTable, NULL);
        if (FAILED(hr))
            goto e_Exit;

		// 2005-01-04 by jschoi
//		hr = pmcMesh->m_pSkinMeshInfo->GetMaxFaceInfluences(&pmcMesh->m_maxFaceInfl);
        LPDIRECT3DINDEXBUFFER9 pIB;
        hr = pmcMesh->m_pSkinMesh->GetIndexBuffer(&pIB);
        if (FAILED(hr))
            goto e_Exit;
        hr = pmcMesh->m_pSkinMeshInfo->GetMaxFaceInfluences(pIB, pmcMesh->m_pSkinMesh->GetNumFaces(), &pmcMesh->m_maxFaceInfl);
		pIB->Release();

        if (FAILED(hr))
            goto e_Exit;

        // Allocate space for blend matrices
        m_pBoneMatrices  = new D3DXMATRIXA16[m_maxBones];
        if (m_pBoneMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
    }
    pmcMesh->m_Method = m_method;

e_Exit:

    GXRELEASE(pDevice);
    return hr;
}


HRESULT CSkinnedMesh::FindBones(SFrame *pframeCur, 
								SDrawElement *pde)
{
	FLOG( "CSkinnedMesh::FindBones(SFrame *pframeCur, SDrawElement *pde)" );
    HRESULT hr = S_OK;
    SMeshContainer *pmcMesh;
    SFrame *pframeChild;
    
    pmcMesh = pframeCur->pmcMesh;
    while (pmcMesh != NULL)
    {
        if (pmcMesh->m_pSkinMesh)
        {
			// 2005-01-04 by jschoi
//            char** pBoneName = static_cast<char**>(pmcMesh->m_pBoneNamesBuf->GetBufferPointer());
            for (DWORD i = 0; i < pmcMesh->m_pSkinMeshInfo->GetNumBones(); ++i)
            {
				char * strBoneName = (LPTSTR)pmcMesh->m_pSkinMeshInfo->GetBoneName(i);
                SFrame* pFrame = pde->FindFrame(strBoneName);
                pmcMesh->m_pBoneMatrix[i] = &(pFrame->matCombined);
            }
        }
        pmcMesh = pmcMesh->pmcNext;
    }
    
    pframeChild = pframeCur->pframeFirstChild;
    while (pframeChild != NULL)
    {
        hr = FindBones(pframeChild, pde);
        if (FAILED(hr))
            return hr;
        
        pframeChild = pframeChild->pframeSibling;
    }
    
    return S_OK;
}

HRESULT CSkinnedMesh::LoadAnimation(LPDIRECTXFILEDATA pxofobjCur, 
									SDrawElement *pde,
                                    DWORD options, DWORD fvf, 
									LPDIRECT3DDEVICE9 pD3DDevice,
                                    SFrame *pframeParent)
{
//	FLOG( "CSkinnedMesh::LoadAnimation(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent)" );
    HRESULT hr = S_OK;
    SRotateKeyXFile *pFileRotateKey;
    SScaleKeyXFile *pFileScaleKey;
    SPositionKeyXFile *pFilePosKey;
    SMatrixKeyXFile *pFileMatrixKey;
    SFrame *pframeCur;
    LPDIRECTXFILEDATA pxofobjChild = NULL;
    LPDIRECTXFILEOBJECT pxofChild = NULL;
    LPDIRECTXFILEDATAREFERENCE pxofobjChildRef = NULL;
    const GUID *type;
    DWORD dwSize;
    PBYTE pData;
    DWORD dwKeyType;
    DWORD cKeys;
    DWORD iKey;
    DWORD cchName;
    char *szFrameName;
    
    pframeCur = new SFrame();
    if (pframeCur == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    pframeCur->bAnimationFrame = true;
    
    pframeParent->AddFrame(pframeCur);
    pde->AddAnimationFrame(pframeCur);
    
    // Enumerate child objects.
    // Child object can be data, data reference or binary.
    // Use QueryInterface() to find what type of object a child is.
    while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))
    {
        // Query the child for it's FileDataReference
        hr = pxofChild->QueryInterface(IID_IDirectXFileDataReference,
            (LPVOID *)&pxofobjChildRef);
        if (SUCCEEDED(hr))
        {
            hr = pxofobjChildRef->Resolve(&pxofobjChild);
            if (SUCCEEDED(hr))
            {
                hr = pxofobjChild->GetType(&type);
                if (FAILED(hr))
                    goto e_Exit;
                
                if( TID_D3DRMFrame == *type )
                {
                    if (pframeCur->pframeToAnimate != NULL)
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                    }
                    
                    hr = pxofobjChild->GetName(NULL, &cchName);
                    if (FAILED(hr))
                        goto e_Exit;
                    
                    if (cchName == 0)
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                        
                    }
                    
                    szFrameName = (char*)_alloca(cchName);
                    if (szFrameName == NULL)
                    {
                        hr = E_OUTOFMEMORY;
                        goto e_Exit;
                    }
                    
                    hr = pxofobjChild->GetName(szFrameName, &cchName);
                    if (FAILED(hr))
                        goto e_Exit;
                    
                    pframeCur->pframeToAnimate = pde->FindFrame(szFrameName);
                    if (pframeCur->pframeToAnimate == NULL)
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                    }
                }
                
                GXRELEASE(pxofobjChild);
            }
            
            GXRELEASE(pxofobjChildRef);
        }
        else
        {
            
            // Query the child for it's FileData
            hr = pxofChild->QueryInterface(IID_IDirectXFileData,
                (LPVOID *)&pxofobjChild);
            if (SUCCEEDED(hr))
            {
                hr = pxofobjChild->GetType(&type);
                if (FAILED(hr))
                    goto e_Exit;
                
                if ( TID_D3DRMFrame == *type )
                {
                    hr = LoadFrames(pxofobjChild, pde, options, fvf, pD3DDevice, pframeCur);
                    if (FAILED(hr))
                        goto e_Exit;
                }
                else if ( TID_D3DRMAnimationOptions == *type )
                {
                }
                else if ( TID_D3DRMAnimationKey == *type )
                {
                    hr = pxofobjChild->GetData( NULL, &dwSize, (PVOID*)&pData );
                    if (FAILED(hr))
                        goto e_Exit;
                    
                    dwKeyType = ((DWORD*)pData)[0];
                    cKeys = ((DWORD*)pData)[1];
                    
                    if (dwKeyType == 0)
                    {
                        if (pframeCur->m_pRotateKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_pRotateKeys = new SRotateKey[cKeys];
                        if (pframeCur->m_pRotateKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_cRotateKeys = cKeys;
                        //NOTE x files are w x y z and QUATERNIONS are x y z w
                        
                        pFileRotateKey =  (SRotateKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pframeCur->m_pRotateKeys[iKey].dwTime = pFileRotateKey->dwTime;
                            pframeCur->m_pRotateKeys[iKey].quatRotate.x = pFileRotateKey->x;
                            pframeCur->m_pRotateKeys[iKey].quatRotate.y = pFileRotateKey->y;
                            pframeCur->m_pRotateKeys[iKey].quatRotate.z = pFileRotateKey->z;
                            pframeCur->m_pRotateKeys[iKey].quatRotate.w = pFileRotateKey->w;
                            
                            pFileRotateKey += 1;
                        }
                    }
                    else if (dwKeyType == 1)
                    {
                        if (pframeCur->m_pScaleKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_pScaleKeys = new SScaleKey[cKeys];
                        if (pframeCur->m_pScaleKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_cScaleKeys = cKeys;
                        
                        pFileScaleKey =  (SScaleKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pframeCur->m_pScaleKeys[iKey].dwTime = pFileScaleKey->dwTime;
                            pframeCur->m_pScaleKeys[iKey].vScale = pFileScaleKey->vScale;
                            
                            pFileScaleKey += 1;
                        }
                    }
                    else if (dwKeyType == 2)
                    {
                        if (pframeCur->m_pPositionKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_pPositionKeys = new SPositionKey[cKeys];
                        if (pframeCur->m_pPositionKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_cPositionKeys = cKeys;
                        
                        pFilePosKey =  (SPositionKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pframeCur->m_pPositionKeys[iKey].dwTime = pFilePosKey->dwTime;
                            pframeCur->m_pPositionKeys[iKey].vPos = pFilePosKey->vPos;
                            
                            pFilePosKey += 1;
                        }
                    }
                    else if (dwKeyType == 4)
                    {
                        if (pframeCur->m_pMatrixKeys != NULL)
                        {
                            hr = E_INVALIDARG;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_pMatrixKeys = new SMatrixKey[cKeys];
                        if (pframeCur->m_pMatrixKeys == NULL)
                        {
                            hr = E_OUTOFMEMORY;
                            goto e_Exit;
                        }
                        
                        pframeCur->m_cMatrixKeys = cKeys;
                        
                        pFileMatrixKey =  (SMatrixKeyXFile*)(pData + (sizeof(DWORD) * 2));
                        for (iKey = 0;iKey < cKeys; iKey++)
                        {
                            pframeCur->m_pMatrixKeys[iKey].dwTime = pFileMatrixKey->dwTime;
                            pframeCur->m_pMatrixKeys[iKey].mat = pFileMatrixKey->mat;
                            
                            pFileMatrixKey += 1;
                        }
                    }
                    else
                    {
                        hr = E_INVALIDARG;
                        goto e_Exit;
                    }
                }
                
                GXRELEASE(pxofobjChild);
            }
        }
        
        GXRELEASE(pxofChild);
    }
    
e_Exit:
    GXRELEASE(pxofobjChild);
    GXRELEASE(pxofChild);
    GXRELEASE(pxofobjChildRef);
    return hr;
}

HRESULT CSkinnedMesh::LoadAnimationSet(LPDIRECTXFILEDATA pxofobjCur, 
									   SDrawElement *pde,
                                       DWORD options, 
									   DWORD fvf, 
									   LPDIRECT3DDEVICE9 pD3DDevice,
                                       SFrame *pframeParent)
{
//	FLOG( "CSkinnedMesh::LoadAnimationSet(LPDIRECTXFILEDATA pxofobjCur, SDrawElement *pde,DWORD options, DWORD fvf, LPDIRECT3DDEVICE8 pD3DDevice, SFrame *pframeParent)" );
    SFrame *pframeCur;
    const GUID *type;
    HRESULT hr = S_OK;
    LPDIRECTXFILEDATA pxofobjChild = NULL;
    LPDIRECTXFILEOBJECT pxofChild = NULL;
    DWORD cchName;
    
    pframeCur = new SFrame();
    if (pframeCur == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    pframeCur->bAnimationFrame = true;
    
    pframeParent->AddFrame(pframeCur);
    
    hr = pxofobjCur->GetName(NULL, &cchName);
    if (FAILED(hr))
        goto e_Exit;
    
    if (cchName > 0)
    {
        pframeCur->szName = new char[cchName];
        if (pframeCur->szName == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
        
        hr = pxofobjCur->GetName(pframeCur->szName, &cchName);
        if (FAILED(hr))
            goto e_Exit;
    }
   // Enumerate child objects.
    // Child object can be data, data reference or binary.
    // Use QueryInterface() to find what type of object a child is.
    while (SUCCEEDED(pxofobjCur->GetNextObject(&pxofChild)))
    {
        // Query the child for it's FileData
        hr = pxofChild->QueryInterface(IID_IDirectXFileData,
            (LPVOID *)&pxofobjChild);
        if (SUCCEEDED(hr))
        {
            hr = pxofobjChild->GetType(&type);
            if (FAILED(hr))
                goto e_Exit;
            
            if( TID_D3DRMAnimation == *type )
            {
                hr = LoadAnimation(pxofobjChild, pde, options, fvf, pD3DDevice, pframeCur);
                if (FAILED(hr))
                    goto e_Exit;
            }
            
            GXRELEASE(pxofobjChild);
        }
        
        GXRELEASE(pxofChild);
    }
    
e_Exit:
    GXRELEASE(pxofobjChild);
    GXRELEASE(pxofChild);
    return hr;
}

HRESULT CSkinnedMesh::UpdateFrames(SFrame *pframeCur, 
								   D3DXMATRIX &matCur)
{
 //   HRESULT hr = S_OK;
//    pframeCur->matCombined = matCur;
    D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matRot, &matCur);
    D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matCombined, &pframeCur->matTrans );
    SFrame *pframeChild = pframeCur->pframeFirstChild;
	
	// 2005-01-11 by jschoi
	SMeshContainer *pmcMesh;
	BOOL bResult = TRUE;
	D3DXVECTOR3 vMeshPos;
	D3DXMATRIX matTemp;

    while (pframeChild != NULL)
    {
//        hr = UpdateFrames(pframeChild, pframeCur->matCombined);
//        if (FAILED(hr))
//            return hr;

		// 2005-01-11 by jschoi
		pmcMesh = pframeChild->pmcMesh;
		bResult = TRUE;
		if(	g_bDetailDrawFrame == TRUE &&		// ���� �ø� ����� ����ϰ�
			pmcMesh &&						// �޽ð� �����ϰ�
			m_bProgressiveMesh == FALSE)	// ����Ʈ�� �ƴϸ� ���� �ø� �õ��Ѵ�.
		{
			D3DXMatrixIdentity(&matTemp);
			matTemp._41 = pmcMesh->vCenter.x;
			matTemp._42 = pmcMesh->vCenter.y;
			matTemp._43 = pmcMesh->vCenter.z;
			
			D3DXMatrixMultiply(&matTemp, &matTemp, &m_mWorld);
			
			vMeshPos.x = matTemp._41;
			vMeshPos.y = matTemp._42;
			vMeshPos.z = matTemp._43;
			
			bResult = g_pFrustum->CheckSphere( vMeshPos.x, vMeshPos.y, vMeshPos.z, pmcMesh->fRadius);
			
			if( bResult &&
				D3DXVec3Length(&(vMeshPos - g_pCamera->m_vCamCurrentPos)) - pmcMesh->fRadius > g_pScene->m_fFogEndValue )
			{
				bResult = FALSE;
			}
		}
		
		if(bResult)
		{
			UpdateFrames(pframeChild, pframeCur->matCombined);
		}
        pframeChild = pframeChild->pframeSibling;
    }
	
    return S_OK;
}

HRESULT CSkinnedMesh::UpdateFrames(SFrame *pframeCur, 
								   D3DXMATRIX &matCur,D3DXVECTOR3 vPos,float fCheckDistance)
{
 //   HRESULT hr = S_OK;
//    pframeCur->matCombined = matCur;
    D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matRot, &matCur);
    D3DXMatrixMultiply(&pframeCur->matCombined, &pframeCur->matCombined, &pframeCur->matTrans );
    SFrame *pframeChild = pframeCur->pframeFirstChild;
	SMeshContainer *pmcMesh;
	D3DXVECTOR3 vMeshCenter;
    while (pframeChild != NULL)
    {
		pmcMesh = pframeChild->pmcMesh;				
		if(	pmcMesh )
		{
			D3DXVec3TransformCoord(&vMeshCenter,&pmcMesh->vCenter,&m_mWorld);
			if(D3DXVec3Length(&(vMeshCenter - vPos)) - pmcMesh->fRadius < fCheckDistance)
			{
				UpdateFrames(pframeChild, pframeCur->matCombined,vPos,fCheckDistance);
			}
		}
		else
		{
			UpdateFrames(pframeChild, pframeCur->matCombined,vPos,fCheckDistance);
		}
 
        pframeChild = pframeChild->pframeSibling;
    }
	
    return S_OK;
}

HRESULT CSkinnedMesh::DrawMeshContainer(SMeshContainer *pmcMesh, DWORD nType)
{
    UINT ipattr;
    HRESULT hr = S_OK;

	if (pmcMesh->m_pSkinMeshInfo != NULL)
	{
		D3DXMATRIX mat;
		LPD3DXBONECOMBINATION pBoneComb;
		DWORD AttribIdPrev;

		// 2005-01-05 by jschoi
//		if (m_method != pmcMesh->m_Method)
//		{
//			GenerateMesh(pmcMesh);
//		}

		if (m_method == D3DNONINDEXED)
		{
			AttribIdPrev = UNUSED32; 
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
			// Draw using default vtx processing of the device (typically HW)
			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				DWORD numBlend = 0;
				for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
				{
					if (pBoneComb[ipattr].BoneId[i] != UINT_MAX)
					{
						numBlend = i;
					}
				}

				if (m_d3dCaps.MaxVertexBlendMatrices >= numBlend + 1)
				{
					for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
					{
						DWORD matid = pBoneComb[ipattr].BoneId[i];
						if (matid != UINT_MAX)
						{
							m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
							m_pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
						}
					}

					m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, numBlend);

					if ((AttribIdPrev != pBoneComb[ipattr].AttribId) || (AttribIdPrev == UNUSED32))
					{
						m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));
						if(pmcMesh->pTextures[pBoneComb[ipattr].AttribId]) 
							m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
						else 
							m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum-1]);
						AttribIdPrev = pBoneComb[ipattr].AttribId;
					}

					// 2005-01-05 by jschoi - ���� �޽� ���� ��� ����
// 					if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_dwGameState == _GAME && m_bCheckBlend)
//					{
//						SetMeshRenderState(pmcMesh);
//					}

					pmcMesh->pMesh->DrawSubset( ipattr );
				}
			}

			// If necessary, draw parts that HW could not handle using SW
			if (pmcMesh->iAttrSplit < pmcMesh->cpattr)
			{
				AttribIdPrev = UNUSED32; 
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);
				
				for (ipattr = pmcMesh->iAttrSplit; ipattr < pmcMesh->cpattr; ipattr++)
				{
					DWORD numBlend = 0;
					for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
					{
						if (pBoneComb[ipattr].BoneId[i] != UINT_MAX)
						{
							numBlend = i;
						}
					}

					if (m_d3dCaps.MaxVertexBlendMatrices < numBlend + 1)
					{
						for (DWORD i = 0; i < pmcMesh->m_maxFaceInfl; ++i)
						{
							DWORD matid = pBoneComb[ipattr].BoneId[i];
							if (matid != UINT_MAX)
							{
								m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
								m_pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
							}
						}

						m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, numBlend);

						if ((AttribIdPrev != pBoneComb[ipattr].AttribId) || (AttribIdPrev == UNUSED32))
						{
							m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));

							if(pmcMesh->pTextures[pBoneComb[ipattr].AttribId]) 
							{
								m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
							}
							else 
							{
								m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum-1]);
							}
							AttribIdPrev = pBoneComb[ipattr].AttribId;
						}

						// 2005-01-05 by jschoi ���� �޽� ���� ��� ����
// 						if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_dwGameState == _GAME && m_bCheckBlend)
//						{
//							SetMeshRenderState(pmcMesh);
//						}
						pmcMesh->pMesh->DrawSubset( ipattr );
					}
				}
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);

			}
			m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
		}
		else if (m_method == D3DINDEXEDVS) // VertexShader�� ����� ���
		{
			// Use COLOR instead of UBYTE4 since Geforce3 does not support it
			// vConst.w should be 3, but due to about hack, mul by 255 and add epsilon
			D3DXVECTOR4 vConst( 1.0f, 0.0f, 0.0f, 765.01f );
			LPDIRECT3DVERTEXBUFFER9 pVB;
			LPDIRECT3DINDEXBUFFER9 pIB;

			if (pmcMesh->m_bUseSW)
			{
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);
			}

			pmcMesh->pMesh->GetVertexBuffer(&pVB);
			pmcMesh->pMesh->GetIndexBuffer(&pIB);
			hr = m_pd3dDevice->SetStreamSource(0, pVB,0, D3DXGetFVFVertexSize(pmcMesh->pMesh->GetFVF()));
			if(FAILED(hr))
				return hr;
			hr = m_pd3dDevice->SetIndices(pIB);
			if(FAILED(hr))
				return hr;
			pVB->Release();
			pIB->Release();

			hr = m_pd3dDevice->SetFVF(m_dwIndexedVertexShader[pmcMesh->m_maxFaceInfl - 1]);
			if(FAILED(hr))
				return hr;

			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				for (DWORD i = 0; i < pmcMesh->m_paletteSize; ++i)
				{
					DWORD matid = pBoneComb[ipattr].BoneId[i];
					if (matid != UINT_MAX)
					{
						D3DXMATRIXA16 mat;
						D3DXMatrixMultiply(&mat, &pmcMesh->m_pBoneOffsetMat[matid], pmcMesh->m_pBoneMatrix[matid]);
					}
				}

				// Sum of all ambient and emissive contribution
				D3DXCOLOR ambEmm;
				D3DXColorModulate(&ambEmm, &D3DXCOLOR(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId].Ambient),&D3DXCOLOR(.25, .25, .25, 1.0));
				ambEmm += D3DXCOLOR(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId].Emissive);
				vConst.y = pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId].Power;

				if(pmcMesh->pTextures[pBoneComb[ipattr].AttribId]) 
				{
					m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
				}
				else 
				{
					m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum-1]);
				}

				// 2005-01-05 by jschoi - ���� �޽� ������
// 				if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_dwGameState == _GAME && m_bCheckBlend)
//				{
//					SetMeshRenderState(pmcMesh);
//				}

				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,
											 pBoneComb[ipattr].VertexStart, pBoneComb[ipattr].VertexCount,
											 pBoneComb[ipattr].FaceStart * 3, pBoneComb[ipattr].FaceCount);
			}

			// VertexProcessing ����
			if (pmcMesh->m_bUseSW)
			{
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);
			}
		}
		else if (m_method == D3DINDEXED)
		{
			// Vertex processing�� �������� �ʴ� �׷��� ī���� Software Vertex Processing ���
			if (pmcMesh->m_bUseSW)
			{
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(TRUE);
			}

			// set the number of vertex blend indices to be blended
			if (pmcMesh->m_maxFaceInfl == 1)
			{
				m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
			}
			else
			{
				m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, pmcMesh->m_maxFaceInfl - 1);
			}
			if (pmcMesh->m_maxFaceInfl)
			{
				m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
			}

			// for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pmcMesh->m_pBoneCombinationBuf->GetBufferPointer());
			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				for (DWORD i = 0; i < pmcMesh->m_paletteSize; ++i)
				{
					DWORD matid = pBoneComb[ipattr].BoneId[i];
					if (matid != UINT_MAX)
					{
						m_pd3dDevice->SetTransform(D3DTS_WORLDMATRIX(i), pmcMesh->m_pBoneMatrix[matid]);
						m_pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(i), &pmcMesh->m_pBoneOffsetMat[matid]);
					}
				}
            
				m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[pBoneComb[ipattr].AttribId]));

				if(pmcMesh->pTextures[pBoneComb[ipattr].AttribId]) 
				{
					m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[pBoneComb[ipattr].AttribId]);
				}
				else 
				{
					m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum-1]);
				}

				// 2005-01-05 by jschoi - ���� �޽�
// 				if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_dwGameState == _GAME && m_bCheckBlend)
//				{
//					SetMeshRenderState(pmcMesh);
//				}
				
				pmcMesh->pMesh->DrawSubset( ipattr );
			}
			m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
			m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

			// �ٽ� ������� �������ش�. �ݵ��..
			if (pmcMesh->m_bUseSW)
			{
				// 2005-01-04 by jschoi
				m_pd3dDevice->SetSoftwareVertexProcessing(FALSE);
			}
		}
		else if (m_method == SOFTWARE)
		{
			D3DXMATRIX  Identity;
			DWORD       cBones  = pmcMesh->m_pSkinMeshInfo->GetNumBones();
            PBYTE       pbVerticesSrc;
            PBYTE       pbVerticesDest;

			// set up bone transforms
			for (DWORD iBone = 0; iBone < cBones; ++iBone)
			{
				D3DXMatrixMultiply
				(
					&m_pBoneMatrices[iBone],                 // output
					&pmcMesh->m_pBoneOffsetMat[iBone], 
					pmcMesh->m_pBoneMatrix[iBone]
				);
			}

			// set world transform
			D3DXMatrixIdentity(&Identity);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);

			// 2005-01-04 by jschoi - UpdateSkinnedMesh 
			if(pmcMesh->pMesh == NULL)	// 2005-01-05 by jschoi - ����̽��� �����鼭 pMesh�� NULL�� �ȴ�.
			{
				hr = pmcMesh->m_pSkinMesh->CloneMeshFVF(D3DXMESH_DYNAMIC, pmcMesh->m_pSkinMesh->GetFVF(),
                                            m_pd3dDevice, &pmcMesh->pMesh);
				if(FAILED(hr))
					return hr;					
			}
			pmcMesh->m_pSkinMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
			pmcMesh->pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);
			pmcMesh->m_pSkinMeshInfo->UpdateSkinnedMesh(m_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest );	
			pmcMesh->m_pSkinMesh->UnlockVertexBuffer();
			pmcMesh->pMesh->UnlockVertexBuffer();

			for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
			{
				if(m_bProgressiveMesh)
				{
					m_pd3dDevice->SetMaterial(&m_material);
				}
				else
				{
					m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[ipattr]));
				}

				if(pmcMesh->pTextures[pmcMesh->m_pAttrTable[ipattr].AttribId])
				{
					g_pD3dDev->SetTexture(0, pmcMesh->pTextures[pmcMesh->m_pAttrTable[ipattr].AttribId]);
				}
				else
				{
					g_pD3dDev->SetTexture(0, m_pTexture[m_bTextureNum-1]);
				}

				// 2005-01-05 by jschoi - ���� �޽� ���� ������
//				if(g_pD3dApp->m_pShuttleChild && g_pD3dApp->m_dwGameState == _GAME && m_bCheckBlend)
//				{
//					SetMeshRenderState(pmcMesh);//,TRUE);
//				}
				
				pmcMesh->pMesh->DrawSubset( ipattr );			
			}
		}
		else
		{
			// 2005-01-05 by jschoi - ���� ��Ȳ
			return E_FAIL;
		}
	}
	else
	{
		// 2005-01-05 by jschoi		
		for (ipattr = 0; ipattr < pmcMesh->cpattr; ipattr++)
		{
			if(m_bProgressiveMesh)
				m_pd3dDevice->SetMaterial(&m_material);
			else
				m_pd3dDevice->SetMaterial(&(pmcMesh->rgMaterials[ipattr]));

			// by dhkwon 2003.10.26, ���� �ؽ��� ����
			if(m_pOrderTexture)
			{
				m_pd3dDevice->SetTexture(0, m_pOrderTexture);
			}
			else
			{
				if(nType == _SHUTTLE || nType == _ENEMY)
				{					
					// ����� ��ȿ���� ��� ���� �ٸ������ ������ �ȳ� ;;
					// ���� ��ü ���
					// 2006-02-17 by ispark
//					m_unTexSelectColor = g_pD3dApp->GetUnitArmorColorNum();
					// 2006-04-05 by ispark, �Ƹ� ���� �ʿ��ϹǷ� �� �ؽ��� �������� �׸��� �ؽ��Ĵ� ���־�� �Ѵ�.
// 2007-10-08 by dgwoo ĳ���Ϳ� UnitRender���� ó��.
//					g_pD3dApp->SetUnitArmorColorMax(m_bTotalTextureNum - 1);
					
					if(m_unTexSelectColor >= m_bTotalTextureNum)
					{
						m_unTexSelectColor = 0;
					}
					if(m_unTexSelectColor<=0)
					{
						m_unTexSelectColor = 1;
					}
						

					// ���� �ؽ��� �κ�
					// �ؽ��� ���� �κ�.
					if(pmcMesh->pTextures[ipattr])
					{
						//////////////////////////////////////////////////////////////////////////
						// ���� ��� �ؽ���
						// m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[ipattr]);
						//
						//////////////////////////////////////////////////////////////////////////

						
						//////////////////////////////////////////////////////////////////////////
						// 1�ؽ��� ��� 						 
						 // 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
						 //m_pd3dDevice->SetTexture(0, m_pTexture[m_unTexSelectColor-1]);
						int nLoadIdx = m_unTexSelectColor-1;
						if(FALSE == IsLoadTexture(nLoadIdx))
						{
							nLoadIdx = 0;
						}
						m_pd3dDevice->SetTexture(0, m_pTexture[nLoadIdx]);
						// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
						//
						//////////////////////////////////////////////////////////////////////////
												
						//////////////////////////////////////////////////////////////////////////
						// 2�ؽ��� ��� 
//						if(m_unTexColor == 0)
//							m_pd3dDevice->SetTexture(0, m_pTexture[m_unTexColor+1+m_unTexSelectColor]);
//						else if(m_unTexColor == 1)
//							m_pd3dDevice->SetTexture(0, m_pTexture[m_unTexColor-1+m_unTexSelectColor]);
//						else
//							m_pd3dDevice->SetTexture(0, m_pTexture[m_unTexSelectColor]);
						//
						//////////////////////////////////////////////////////////////////////////
						
						//////////////////////////////////////////////////////////////////////////
						// 3�ؽ���
						// if(m_unTexColor == 0)
						//    m_pd3dDevice->SetTexture(0, m_pTexture[m_unTexColor+2+m_unTexSelectColor]);
						// else if(m_unTexColor == 1)
						// 	m_pd3dDevice->SetTexture(0, m_pTexture[m_unTexColor-1+m_unTexSelectColor]);
						// else if(m_unTexColor == 2)
						//	m_pd3dDevice->SetTexture(0, m_pTexture[m_unTexColor-1+m_unTexSelectColor]);
						// else
						// {
						//	 if(m_unUnitTexOld == 0)
						//	 {
						//		m_pd3dDevice->SetTexture(0, m_pTexture[m_unUnitTexOld]);
						//		m_unUnitTexOld = 1;
						//	 }
						//	 else
						//	 {
						//		m_pd3dDevice->SetTexture(0, m_pTexture[m_unUnitTexOld]);
						//		m_unUnitTexOld = 0;
						//	 }
						// }
						//
						//////////////////////////////////////////////////////////////////////////
						
						m_unTexColor++;						
					}
					else
					{
						// ��Į ������						
//						if(m_nDecalCount >= MAX_DECAL_NUM)
//							m_nDecalCount = MAX_DECAL_NUM-1;
//						
//						m_pd3dDevice->SetTexture(0, m_pDecalTextures[m_nDecalCount]);
//						m_nDecalCount++;
					}
				}
				else
				{
					if(pmcMesh->pTextures[ipattr])
						m_pd3dDevice->SetTexture(0, pmcMesh->pTextures[ipattr]);
					else 
					{
						int nTextureNum = m_bTextureNum - 1;
						if(nTextureNum >= 0)
							m_pd3dDevice->SetTexture(0, m_pTexture[m_bTextureNum-1]);
					}
				}

				
				// by dhkwon 2003.11.18 ��Ƽ �ؽ��� ����
				if(m_bMultiTexture)
				{
					// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
					//m_pd3dDevice->SetTexture(1, m_pTexture[1]);
					int nLoadIdx = 1;
					if(FALSE == IsLoadTexture(nLoadIdx))
					{
						nLoadIdx = 0;
					}
					m_pd3dDevice->SetTexture(1, m_pTexture[nLoadIdx]);
				}
			}			
			
			if(g_pD3dApp->m_pFxSystem->GetMetarSurface())
			{
				D3DXMATRIX matView, matWorld, matProj;
				D3DXMatrixIdentity( &matView );
				D3DXMatrixIdentity( &matWorld );
				D3DXMatrixIdentity( &matProj );

				g_pD3dDev->GetTransform( D3DTS_PROJECTION,	&matProj );
				g_pD3dDev->GetTransform( D3DTS_VIEW,		&matView );
				g_pD3dDev->GetTransform( D3DTS_WORLD,		&matWorld );

				g_pD3dApp->m_pFxSystem->SetMeshFrame(pmcMesh);
				g_pD3dApp->m_pFxSystem->DrawMetalFilterBegin(matWorld, matView, matProj, ipattr, nType);
			}
			else if(g_pD3dApp->m_pFxSystem->GetEnvSurface())
			{
				D3DXMATRIX matView, matWorld, matProj;
				D3DXMatrixIdentity( &matView );
				D3DXMatrixIdentity( &matWorld );
				D3DXMatrixIdentity( &matProj );

				g_pD3dDev->GetTransform( D3DTS_PROJECTION,	&matProj );
				g_pD3dDev->GetTransform( D3DTS_VIEW,		&matView );
				g_pD3dDev->GetTransform( D3DTS_WORLD,		&matWorld );

				g_pD3dApp->m_pFxSystem->SetMeshFrame(pmcMesh);
				g_pD3dApp->m_pFxSystem->DrawEvnFilterBegin(matWorld, matView, matProj, ipattr, nType);

			}
			else
			{
				pmcMesh->pMesh->DrawSubset( ipattr );
			}
		}
	}
    return S_OK;
}

void CSkinnedMesh::SetMeshRenderState(SMeshContainer *pmcMesh)//,BOOL bSkinMesh)
{
	FLOG( "CSkinnedMesh::SetMeshRenderState(SMeshContainer *pmcMesh)" );
	BYTE bBlendType = 0;
	BOOL bHit = FALSE;
	float fDistance = g_pD3dApp->m_pShuttleChild->m_fDistanceCamera;
	D3DXMATRIX matProj;
	D3DXVECTOR3 vPickRayDir, vPickRayOrig;//,v2,vSide;
	D3DXVECTOR3 v;
	D3DXMATRIX matView, m, matWorld, matTemp;
	g_pD3dDev->GetTransform( D3DTS_PROJECTION, &matProj );
	// Get the inverse view matrix
	g_pD3dDev->GetTransform( D3DTS_VIEW, &matView );
	g_pD3dDev->GetTransform( D3DTS_WORLD, &matWorld );
	m = matWorld*matView;
	D3DXMatrixInverse( &m, NULL, &m );
	// ��Ʋ�� �߾Ӱ� ���� üũ
	// Compute the vector of the pick ray in screen space
	v.x =  ( ( ( 2.0f * g_pD3dApp->m_pShuttleChild->m_nObjScreenX ) / g_pD3dApp->GetBackBufferDesc().Width  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * g_pD3dApp->m_pShuttleChild->m_nObjScreenY ) / g_pD3dApp->GetBackBufferDesc().Height ) - 1 ) / matProj._22;
	v.z =  1.0f;
	// Transform the screen space pick ray into 3D space
	vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
	D3DXIntersect(pmcMesh->pMesh,&(vPickRayOrig), &(vPickRayDir) , &bHit, NULL, NULL, NULL, &m_fCollDist, NULL, NULL);
	if(bHit)
	{
		if(m_fCollDist < fDistance)
		{
			m_bCheckBlend = FALSE;
//			g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
//			g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//			g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
//			g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   TRUE );
//			g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
//			g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			g_pShuttleChild->m_bCheckBlend = TRUE;
//			bBlendType = 1;
		}
		else
		{
//			g_pShuttleChild->m_bCheckBlend = FALSE;
		}
	}
	else
	{
//		g_pShuttleChild->m_bCheckBlend = FALSE;
	}
/*
	if(bBlendType==1)
	{
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
//		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	}
*/
}


HRESULT CSkinnedMesh::DrawFrames(SFrame *pframeCur, 
								 UINT &cTriangles,
								 DWORD nType)
{
//	FLOG( "CSkinnedMesh::DrawFrames(SFrame *pframeCur, UINT &cTriangles)" );
    HRESULT hr = S_OK;
    SMeshContainer *pmcMesh;
    SFrame *pframeChild;

    if (pframeCur->pmcMesh != NULL)
    {
        hr = m_pd3dDevice->SetTransform(D3DTS_WORLD, &pframeCur->matCombined);
        if(FAILED(hr))
            return hr;
    }
    pmcMesh = pframeCur->pmcMesh;

	BOOL bResult = TRUE;
	D3DXVECTOR3 vMeshPos;
	D3DXMATRIX matTemp;

    while (pmcMesh != NULL)
    {
		// 2004-10-29 by jschoi
		// �޽ÿ��� ���� �޽ÿ� ���� ���� ����ü �ø��� �Ѵ�.(����ü �ø� + �Ÿ� �ø�)
		// bResult = TRUE;
		
//		if(m_bProgressiveMesh == FALSE && pmcMesh->fRadius > 500.0f)	// ����Ʈ�� ũ�Ⱑ 200.0f ������ �޽ÿ� ���ؼ��� ���� �ø��� ���� �ʴ´�.
//		{
//			D3DXMatrixIdentity(&matTemp);
//			matTemp._41 = pmcMesh->vCenter.x;
//			matTemp._42 = pmcMesh->vCenter.y;
//			matTemp._43 = pmcMesh->vCenter.z;
//			
//			D3DXMatrixMultiply(&matTemp, &matTemp, &m_mWorld);
//			
//			vMeshPos.x = matTemp._41;
//			vMeshPos.y = matTemp._42;
//			vMeshPos.z = matTemp._43;
//			
//			bResult = g_pFrustum->CheckSphere( vMeshPos.x, vMeshPos.y, vMeshPos.z, pmcMesh->fRadius);
//			
//			if( bResult &&
//				D3DXVec3Length(&(vMeshPos - g_pCamera->m_vCamCurrentPos)) - pmcMesh->fRadius > g_pScene->m_fFogEndValue )
//			{
//				bResult = FALSE;
//			}
//		}
//		
//		if(bResult)
//		{
//			hr = DrawMeshContainer(pmcMesh);
//			if (FAILED(hr))
//				return hr;
//			cTriangles += pmcMesh->pMesh->GetNumFaces();
//			
// 		}

		hr = DrawMeshContainer(pmcMesh, nType);
		if (FAILED(hr))
			return hr;
		cTriangles += pmcMesh->pMesh->GetNumFaces();

        pmcMesh = pmcMesh->pmcNext;
    }
	
    pframeChild = pframeCur->pframeFirstChild;
	
    while (pframeChild != NULL)
    {
		// 2005-01-05 by jschoi
		pmcMesh = pframeChild->pmcMesh;
		bResult = TRUE;
		if(	g_bDetailDrawFrame == TRUE &&		// ���� �ø� ����� ����ϰ�
			pmcMesh &&						// �޽ð� �����ϰ�
			m_bProgressiveMesh == FALSE)	// ����Ʈ�� �ƴϸ� ���� �ø� �õ��Ѵ�.
		{
			D3DXMatrixIdentity(&matTemp);
			matTemp._41 = pmcMesh->vCenter.x;
			matTemp._42 = pmcMesh->vCenter.y;
			matTemp._43 = pmcMesh->vCenter.z;
			
			D3DXMatrixMultiply(&matTemp, &matTemp, &m_mWorld);
			
			vMeshPos.x = matTemp._41;
			vMeshPos.y = matTemp._42;
			vMeshPos.z = matTemp._43;
			// 2011-06-10 by jhahn ���� ���� �� ������Ʈ �ø� ���� 
			//bResult = g_pFrustum->CheckSphere( vMeshPos.x, vMeshPos.y, vMeshPos.z, pmcMesh->fRadius);
			//end 2011-06-10 by jhahn ���� ���� �� ������Ʈ �ø� ���� 
			
			if( bResult &&
				D3DXVec3Length(&(vMeshPos - g_pCamera->m_vCamCurrentPos)) - pmcMesh->fRadius > g_pScene->m_fFogEndValue )
			{
				bResult = FALSE;
			}
		}
		
		if(bResult)
		{
			hr = DrawFrames(pframeChild, cTriangles, nType);
			if (FAILED(hr))
				return hr;
		}
        pframeChild = pframeChild->pframeSibling;
    }
	
    return S_OK;
}


HRESULT  CSkinnedMesh::DeleteSelectedMesh()
{
	FLOG( "CSkinnedMesh::DeleteSelectedMesh()" );
	if (m_pdeSelected != NULL)
    {
        SDrawElement *pdeCur = m_pdeHead;
        SDrawElement *pdePrev = NULL;
        while ((pdeCur != NULL) && (pdeCur != m_pdeSelected))
        {
            pdePrev = pdeCur;
            pdeCur = pdeCur->pdeNext;
        }

        if (pdePrev == NULL)
        {
            m_pdeHead = m_pdeHead->pdeNext;
        }
        else
        {
            pdePrev->pdeNext = pdeCur->pdeNext;
        }

        m_pdeSelected->pdeNext = NULL;
        if (m_pdeHead == m_pdeSelected)
            m_pdeHead = NULL;
        delete m_pdeSelected;
        m_pdeSelected = NULL;
    }

    return S_OK;
}

void CSkinnedMesh::ReleaseDeviceDependentMeshes(SFrame* pframe)
{
//	FLOG( "CSkinnedMesh::ReleaseDeviceDependentMeshes(SFrame* pframe)" );
    if (pframe->pmcMesh != NULL)
    {
        for (SMeshContainer* pmcCurr = pframe->pmcMesh; pmcCurr != NULL; pmcCurr = pmcCurr->pmcNext)
        {
            if (pmcCurr->m_pSkinMesh != NULL)
            {
                GXRELEASE(pmcCurr->pMesh);
				pmcCurr->m_Method = NONE;
			}
/*			for(i=0;i<5;i++)
			{
				if(pmcCurr->pMeshes[i])
					GXRELEASE(pmcCurr->pMeshes[i]);
			}
*/        }
    }

    if (pframe->pframeFirstChild != NULL)
        ReleaseDeviceDependentMeshes(pframe->pframeFirstChild);

    if (pframe->pframeSibling != NULL)
        ReleaseDeviceDependentMeshes(pframe->pframeSibling);
}

// global func

HRESULT CalculateBoundingSphere(SDrawElement *pdeCur, D3DXVECTOR3 vObjScaleSphere)							// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
{
    HRESULT hr = S_OK;
    UINT cVertices = 0;
    float fRadiusSq = 0;
    D3DXMATRIXA16 matCur;
        D3DXMatrixIdentity(&matCur);

	// �޽� �Ϻ� ������ ��� ���鼭 �� ����� vCenter ��ǥ�� ������� ������ ���� �޽��� �߽� ��ǥ�� SMeshContainer �� ����Ѵ�.
    hr = CalculateSum(pdeCur->pframeRoot, &matCur, &D3DXVECTOR3(0,0,0), &cVertices, vObjScaleSphere);		// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
	
	// �޽� �Ϻ� ������ ��� ���鼭 �������� ���� ������ SMeshContainer ���� �޽ÿ� �������� �ش� SMeshContainer �� ����Ѵ�.
    hr = CalculateRadius(pdeCur->pframeRoot, &matCur, &D3DXVECTOR3(0,0,0), &fRadiusSq, vObjScaleSphere);	// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
        if (FAILED(hr))
            goto e_Exit;
    
	// �� ���(��ü �޽�)�� �������� SDrawElement �� ����Ѵ�. �� ����� ��� vCenter�� �׻� (0,0,0)�̴�.
 
    D3DXMatrixIdentity(&matCur);

    if (FAILED(hr))
        goto e_Exit;
    
    pdeCur->fRadius = (float)sqrt((double)fRadiusSq);
	pdeCur->fRadius *= max(max(vObjScaleSphere.x, vObjScaleSphere.y), vObjScaleSphere.z);	  // 2012-09-20 by jhahn ���� ������Ʈ Ȯ����� �浹���� ���� ����
    pdeCur->vCenter = D3DXVECTOR3(0,0,0);
    
e_Exit:
    return hr;
}

HRESULT CalculateSum(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter,	// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
                     UINT *pcVertices, D3DXVECTOR3 vObjScaleSphereSum)
{
    HRESULT hr = S_OK;
    PBYTE pbPoints = NULL;
    UINT cVerticesLocal = 0;
    PBYTE pbCur;
    D3DXVECTOR3 *pvCur;
    D3DXVECTOR3 vTransformedCur;
    UINT iPoint;
    SMeshContainer *pmcCur;
    SFrame *pframeCur;
    UINT cVertices;
    D3DXMATRIXA16 matLocal;

//	D3DXMATRIXA16 matObjScale;
//	D3DXMatrixIdentity(&matObjScale);
//	D3DXMatrixScaling(&matObjScale, vObjScaleSphereSum.x, vObjScaleSphereSum.y, vObjScaleSphereSum.z);	    
//	D3DXMatrixMultiply(&matLocal, &matObjScale, pmatCur);						// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
    D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);
    
    pmcCur = pframe->pmcMesh;

	while (pmcCur != NULL )
    {
        D3DXVECTOR3 vLocalCenter(0,0,0);
		UINT nVerticesCount = 0;
        DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());
        
        cVertices = pmcCur->pMesh->GetNumVertices();
        
        hr = pmcCur->pMesh->LockVertexBuffer(0, (LPVOID*)&pbPoints);
        if (FAILED(hr))
            goto e_Exit;
        
        for( iPoint=0, pbCur = pbPoints; iPoint < cVertices; iPoint++, pbCur += fvfsize)
        {
            pvCur = (D3DXVECTOR3*)pbCur;
            
            if ((pvCur->x != 0.0) || (pvCur->y != 0.0) || (pvCur->z != 0.0))
            {
                cVerticesLocal++;
                
                D3DXVec3TransformCoord(&vTransformedCur, pvCur, &matLocal);
                
                pvCenter->x += vTransformedCur.x;
                pvCenter->y += vTransformedCur.y;
                pvCenter->z += vTransformedCur.z;

                nVerticesCount++;
 				vLocalCenter += vTransformedCur;
            }
        }
        
        
        pmcCur->pMesh->UnlockVertexBuffer();
        pbPoints = NULL;
        
		pmcCur->vCenter = vLocalCenter/(float)nVerticesCount;
        
        pmcCur = pmcCur->pmcNext;
    }
    
    *pcVertices += cVerticesLocal;
    
    pframeCur = pframe->pframeFirstChild;
    while (pframeCur != NULL)
    {
        hr = CalculateSum(pframeCur, &matLocal, pvCenter, pcVertices);
        if (FAILED(hr))
            goto e_Exit;
        
        pframeCur = pframeCur->pframeSibling;
    }
    
e_Exit:
    if (pbPoints != NULL)
    {
        pmcCur->pMesh->UnlockVertexBuffer();
    }
    
    return hr;
}




HRESULT CalculateRadius(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, 
                        float *pfRadiusSq, D3DXVECTOR3 vObjScaleSphereRadius)			// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
{
    HRESULT hr = S_OK;
    PBYTE pbPoints = NULL;
    PBYTE pbCur;
    D3DXVECTOR3 *pvCur;
    D3DXVECTOR3 vDist;
    UINT iPoint;
    UINT cVertices;
    SMeshContainer *pmcCur;
    SFrame *pframeCur;
    float fRadiusLocalSq;
    float fDistSq;
    D3DXMATRIXA16 matLocal;
    
	// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
//	D3DXMATRIXA16 matObjScale;
//	D3DXMatrixIdentity(&matObjScale);												
//	D3DXMatrixScaling(&matObjScale, vObjScaleSphereRadius.x, vObjScaleSphereRadius.y, vObjScaleSphereRadius.z);
//	D3DXMatrixMultiply(pmatCur,&matObjScale,pmatCur);
	// end 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����

	D3DXMatrixMultiply(&matLocal, &pframe->matRot, pmatCur);
    
    pmcCur = pframe->pmcMesh;
    fRadiusLocalSq = *pfRadiusSq;
	

    while (pmcCur != NULL)
    {
		float fRealLocalRadius = 0;
		float fLocalDistSq = 0;
		D3DXVECTOR3 vLocalDist(0,0,0);
        DWORD fvfsize = D3DXGetFVFVertexSize(pmcCur->pMesh->GetFVF());
        
        cVertices = pmcCur->pMesh->GetNumVertices();
        
        hr = pmcCur->pMesh->LockVertexBuffer(0, (LPVOID*)&pbPoints);
        if (FAILED(hr))
            goto e_Exit;
        
        for( iPoint=0, pbCur = pbPoints; iPoint < cVertices; iPoint++, pbCur += fvfsize )
        {
            pvCur = (D3DXVECTOR3*)pbCur;
            
            if ((pvCur->x == 0.0) && (pvCur->y == 0.0) && (pvCur->z == 0.0))
                continue;
            
            D3DXVec3TransformCoord(&vDist, pvCur, &matLocal);
            
            vDist -= *pvCenter;
            fDistSq = D3DXVec3LengthSq(&vDist);
            
            if( fDistSq > fRadiusLocalSq )
			{
                fRadiusLocalSq = fDistSq;
        }
        
			D3DXVec3TransformCoord(&vLocalDist, pvCur, &matLocal);
            vLocalDist -= pmcCur->vCenter;
			fLocalDistSq = D3DXVec3LengthSq(&vLocalDist);

			if( fRealLocalRadius < fLocalDistSq)
			{
				fRealLocalRadius = fLocalDistSq;
			}

        }
        
        pmcCur->pMesh->UnlockVertexBuffer();
        pbPoints = NULL;

        pmcCur->fRadius = (float)sqrt((double)fRealLocalRadius);	// ���� �޽� ������ ����

        
        pmcCur = pmcCur->pmcNext;
    }
    
    *pfRadiusSq = fRadiusLocalSq;
    
    pframeCur = pframe->pframeFirstChild;
	
    while (pframeCur != NULL)
    {
        hr = CalculateRadius(pframeCur, &matLocal, pvCenter, pfRadiusSq);
        if (FAILED(hr))
            goto e_Exit;
        
        pframeCur = pframeCur->pframeSibling;
    }
    
e_Exit:
    if (pbPoints != NULL)
    {
        pmcCur->pMesh->UnlockVertexBuffer();
    }
    
    return hr;
}

void CSkinnedMesh::ChangeFilePath(char* strFilePath)
{
	char strTemp[9];

	int nStrSize = strlen(strFilePath);
	if(nStrSize > 8)
	{
		nStrSize -= 5;
		memset(strTemp,0x30,8);
		strTemp[8] = '\0';
		for(int i = 7 ; i >= 0 ; i--)
		{
			strTemp[i] = strFilePath[nStrSize];
			nStrSize--;
			if(	nStrSize == 0 ||
				strFilePath[nStrSize] =='\\')
			{
				break;
			}
		}
		DBGOUT("strFilePath : %s\n",strFilePath);
		DBGOUT("strTemp : %s\n\n",strTemp);
		strcpy(strFilePath,strTemp);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		ispark
/// \date		2005-07-14 ~ 2005-07-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
COLLISION_RESULT CSkinnedMesh::Pick(SMeshContainer *pmcMesh, float fx, float fy)
{
	COLLISION_RESULT collResult;

//    if (pmcMesh->m_pSkinMesh)
//    {
//        if (m_method == SOFTWARE)
//        {
//            D3DXMATRIX  Identity;
//            DWORD       cBones  = pmcMesh->m_pSkinMeshInfo->GetNumBones();
//            // set up bone transforms
//            for (DWORD iBone = 0; iBone < cBones; ++iBone)
//            {
//                D3DXMatrixMultiply
//                (
//                    &m_pBoneMatrices[iBone],                 // output
//                    &pmcMesh->m_pBoneOffsetMat[iBone], 
//                    pmcMesh->m_pBoneMatrix[iBone]
//                );
//            }
//            // set world transform
//            D3DXMatrixIdentity(&Identity);
//            m_pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);
//            // generate skinned mesh
//			if(!pmcMesh->pMesh)
//			{
////				collResult.fDist = DEFAULT_COLLISION_DISTANCE;
//				return collResult;
//			}
////            pmcMesh->m_pSkinMesh->UpdateSkinnedMesh(m_pBoneMatrices, NULL, pmcMesh->pMesh);
//		}
//	}


    BOOL bHit,bIntersections;
    DWORD dwFace;
	FLOAT fBary1, fBary2, fDist;
	D3DXVECTOR3 vPickRayDir, vPickRayOrig;
    D3DXVECTOR3 v;

    D3DXMATRIX matProj;
    m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

    v.x =  ( ( ( 2.0f * fx ) / g_pD3dApp->GetBackBufferDesc().Width  ) - 1 ) / matProj._11;
    v.y = -( ( ( 2.0f * fy ) / g_pD3dApp->GetBackBufferDesc().Height ) - 1 ) / matProj._22;
    v.z =  1.0f;

	// Get the inverse view matrix
    D3DXMATRIX matView, matWorld, m;
    m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
//    D3DXMatrixInverse( &m, NULL, &matView );

	g_pD3dDev->GetTransform( D3DTS_WORLD, &matWorld );
	m = matWorld*matView;
	D3DXMatrixInverse( &m, NULL, &m );

    // Transform the screen space pick ray into 3D space
    vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    vPickRayOrig.x = m._41;
    vPickRayOrig.y = m._42;
    vPickRayOrig.z = m._43;

	D3DXIntersect(pmcMesh->pMesh, &vPickRayOrig, &vPickRayDir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, NULL, NULL);
	
	if(bHit)
	{
		bIntersections = TRUE;
		m_Intersection.dwFace = dwFace;
		m_Intersection.fBary1 = fBary1;
		m_Intersection.fBary2 = fBary2;
		m_Intersection.fDist = fDist;

		collResult.fDist = fDist;
		collResult.vPicking = vPickRayOrig + vPickRayDir * fDist;	// Picking�� ��ġ ��
		D3DXVec3TransformCoord(&collResult.vPicking, &collResult.vPicking, &matWorld);
//		DBGOUT("2D %f, %f ->", fx, fy);
//		DBGOUT("Pick %f, %f, %f\n", collResult.vPicking.x, collResult.vPicking.y, collResult.vPicking.z);

		// 2005-07-26 by ispark 
		// Normal ���� �ʿ��ϹǷ� 
		WORD*			pIndices;
		D3DVERTEX*		pVertices;
		
		D3DVERTEX vThisTri[3];
		WORD* iThisTri;

		pmcMesh->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
		pmcMesh->pMesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndices);

		iThisTri = &pIndices[3*m_Intersection.dwFace];
		// get vertices hit
		vThisTri[0] = pVertices[iThisTri[0]];
		vThisTri[1] = pVertices[iThisTri[1]];
		vThisTri[2] = pVertices[iThisTri[2]];

		pmcMesh->pMesh->UnlockVertexBuffer();
		pmcMesh->pMesh->UnlockIndexBuffer();

		D3DXVec3TransformCoord(&vThisTri[0].p, &vThisTri[0].p, &matWorld);
		D3DXVec3TransformCoord(&vThisTri[1].p, &vThisTri[1].p, &matWorld);
		D3DXVec3TransformCoord(&vThisTri[2].p, &vThisTri[2].p, &matWorld);
//		D3DXVec3TransformNormal(&vThisTri[0].n, &vThisTri[0].n, &matWorld);
//		D3DXVec3TransformNormal(&vThisTri[1].n, &vThisTri[1].n, &matWorld);
//		D3DXVec3TransformNormal(&vThisTri[2].n, &vThisTri[2].n, &matWorld);

		// ������ ������.
	
		D3DXVECTOR3 vTempNormal, vNormalVector;
		D3DXVECTOR3 vCross1,vCross2;
		vCross1 = vThisTri[0].p - vThisTri[1].p;
		vCross2 = vThisTri[1].p - vThisTri[2].p;
		D3DXVec3Cross(&vTempNormal,&vCross1,&vCross2);
		D3DXVec3Normalize(&vNormalVector,&vTempNormal);

//		vNormalVector = vThisTri[0].n + vThisTri[1].n + vThisTri[2].n;
//		D3DXVec3Normalize(&vNormalVector,&vNormalVector);

		collResult.vNormalVector = vNormalVector;
	}
	else
	{
		bIntersections = FALSE;
	}

	return collResult;	
}


COLLISION_RESULT CSkinnedMesh::CheckCollision(float fx, float fy, BOOL bUpdateFrame)
{
//	FLOG( "CSkinnedMesh::CheckCollision(D3DXMATRIX mat)" );
	COLLISION_RESULT collResult,checkcollResult;
	SDrawElement *pdeCur;
    pdeCur = m_pdeHead;
    while (pdeCur != NULL)
    {
		if(bUpdateFrame)
		{
			UpdateFrames(pdeCur->pframeRoot, m_mWorld);
		}
		checkcollResult = CheckCollDist(pdeCur->pframeRoot, fx, fy);
		if(collResult.fDist > checkcollResult.fDist)
		{
			collResult = checkcollResult;
			
		}
        pdeCur = pdeCur->pdeNext;
    }
	return collResult;
}

COLLISION_RESULT CSkinnedMesh::CheckCollDist(SFrame *pframeCur, float fx, float fy)
{
//	FLOG( "CSkinnedMesh::CheckCollDist(SFrame *pframeCur,D3DXMATRIX mat)" );
	COLLISION_RESULT collResult,checkcollResult;
	SMeshContainer *pmcMesh;
	SFrame *pframeChild;

	if (pframeCur->pmcMesh != NULL)
	{
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &pframeCur->matCombined);
	}
	pmcMesh = pframeCur->pmcMesh;

	D3DXVECTOR3 vMeshCenter;

	while(pmcMesh != NULL && pmcMesh->pMesh != NULL)
	{
		checkcollResult = Pick(pmcMesh, fx, fy);
		if(collResult.fDist > checkcollResult.fDist)
			collResult = checkcollResult;

		pmcMesh = pmcMesh->pmcNext;
	}
	pframeChild = pframeCur->pframeFirstChild;
	
	while (pframeChild != NULL)
	{
		// 2005-01-05 by jschoi
		pmcMesh = pframeChild->pmcMesh;				
		if(	pmcMesh )
		{
			D3DXVec3TransformCoord(&vMeshCenter,&pmcMesh->vCenter,&m_mWorld);
//			if(D3DXVec3Length(&(vMeshCenter - vPos)) - pmcMesh->fRadius < fCheckDistance)
//			{
				checkcollResult = CheckCollDist(pframeChild, fx, fy);
				if(collResult.fDist > checkcollResult.fDist)
					collResult = checkcollResult;
// 			}
		}
		else
		{
			checkcollResult = CheckCollDist(pframeChild, fx, fy);
			if(collResult.fDist > checkcollResult.fDist)
				collResult = checkcollResult;
		}
		
		pframeChild = pframeChild->pframeSibling;
	}
	return collResult;
}

HRESULT CSkinnedMesh::SetResourceFile(char* szFileName)
{
	FLOG( "CINFSelect::SetResourceFile(char* szFileName)" );
	SAFE_DELETE(m_pGameDataToDecal);
	m_pGameDataToDecal = new CGameData;
	if(!m_pGameDataToDecal->SetFile(szFileName, FALSE, NULL, 0))
	{
		SAFE_DELETE(m_pGameDataToDecal);
		return E_FAIL;
	}

	return S_OK;
}

DataHeader * CSkinnedMesh::FindResource(char* szRcName)
{
	FLOG( "CINFSelect::FindResource(char* szRcName)" );
	DataHeader* pHeader = NULL;
	if(m_pGameDataToDecal)
	{
		pHeader = m_pGameDataToDecal->Find(szRcName);
	}
	return pHeader;
}
// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
// �׸��� ������ �ؽ�ó
LPDIRECT3DTEXTURE9	CSkinnedMesh::GetShadowTex()
{
	if (m_bTotalTextureNum > 0)	
	return m_pTexture[m_bTotalTextureNum - 1];

	return NULL;
	
	
}

BOOL CSkinnedMesh::IsLowQuality()
{
	return g_pSOption->sLowQuality;
}

BOOL CSkinnedMesh::IsLoadTexture(int nTexture)
{	
	if(NULL == m_pLoadMem)
	{
		return FALSE;
	}
	return m_pLoadMem[nTexture];	
}
// 2007-11-08 by bhsohn �κ� ����Ʈ ���� ó��
void CSkinnedMesh::UpdateMeshObjName(char* pDstPath)
{	
	if(NULL == m_pGameData)
	{
		return;
	}
	char strObjPath[MAX_PATH];
	char buf[16];


	memset(strObjPath, 0x00, MAX_PATH);
	memset(buf, 0x00, 16);
	
	g_pD3dApp->LoadPath(strObjPath, IDS_DIRECTORY_EFFECT, buf);

	char* pMeshFileName = m_pGameData->GetZipFilePath();
	char* pFind = strstr(pMeshFileName, strObjPath);

	if(NULL == pFind)
	{
		return;
	}
	else if(strlen(pFind) < strlen(strObjPath))
	{
		return;
	}
	char* pObjFile = pFind + strlen(strObjPath);

	strncpy(m_szMeshObjFileName, pObjFile, strlen(pObjFile)+1);
	
}

char* CSkinnedMesh::GetMeshObjFileName()
{
	return m_szMeshObjFileName;
}

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStart(structLoadingGameData* GameData, int nMeshIndex)
{
	HRESULT	hr;
	switch(GameData->Step)
	{
	case _RESOURCE_LOADING_START :	
		if( GetIsLoadingStartTime() )
		{
			SetIsLoadingStartTime();
		}
		
		GameData->Step++;

		if(TimeOut())
		{	
			return E_FAIL;
		}

	case _RESOURCE_LOADING_STEP1 :
		if( GetIsLoadingStartTime() )
		{
			SetIsLoadingStartTime();
		}
		hr = LoadMeshHierarchyFromMemLoadStep1(GameData,nMeshIndex);
		GameData->Step++;

		if(TimeOut())
		{
			return E_FAIL;
		}
		
	case _RESOURCE_LOADING_STEP2 :
		if( GetIsLoadingStartTime() )
		{
			SetIsLoadingStartTime();
		}
		hr = LoadMeshHierarchyFromMemLoadStep2();
		GameData->Step++;

		if(TimeOut())
		{
			return E_FAIL;
		}
		
	case _RESOURCE_LOADING_STEP3 :
		if( GetIsLoadingStartTime() )
		{
			SetIsLoadingStartTime();
		}
		hr = LoadMeshHierarchyFromMemLoadStep3();
		GameData->Step++;

		if(TimeOut())
		{
			return E_FAIL;
		}
		
	case _RESOURCE_LOADING_STEP4 :		
		if( GetIsLoadingStartTime() )
		{
			SetIsLoadingStartTime();
		}
		hr = LoadMeshHierarchyFromMemLoadStep4(GameData);

		if(hr == E_FAIL)
		{
			return E_FAIL;
		}
		GameData->Step++;		

	case _RESOURCE_LOADING_STEP5 :
		if( GetIsLoadingStartTime() )
		{
			SetIsLoadingStartTime();
		}
		hr = LoadMeshHierarchyFromMemLoadStep5();

		if(hr == E_FAIL)
		{
			return E_FAIL;
		}
		GameData->Step++;

	case _RESOURCE_LOADING_STEP6 :
		if( GetIsLoadingStartTime() )
		{
			SetIsLoadingStartTime();
		}
		hr = LoadMeshHierarchyFromMemLoadStep6();

		GameData->Step = _RESOURCE_LOADING_END;
		break;
	}	
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep1(structLoadingGameData* pGameData, int nMeshIndex)
{
	FLOG( "CSkinnedMesh::LoadMeshHierarchyFromMemStep1" );
	char Stepbuf[32];
	memset(Stepbuf, 0x00, 32);
	pdeMesh = new SDrawElement();
	pMemory = new DXFILELOADMEMORY;    
	HRESULT hr;
	
	delete pdeMesh->pframeRoot;
    pdeMesh->pframeAnimHead = NULL;
    pdeMesh->pframeRoot = new SFrame();

	// object file loading
	char* pPointer = NULL;
	m_pGameData = pGameData->pGameData;

	// 2007-11-08 by bhsohn �κ� ����Ʈ ���� ó��
	UpdateMeshObjName(m_szMeshObjFileName);

	int totalNum = m_pGameData->GetTotalNumber();
	m_pDataHeader = new DataHeader[totalNum];
	
	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����			
	{
		SAFE_DELETE_ARRAY(m_pLoadMem);		
		m_pLoadMem = new BOOL[totalNum];
		int nCnt = 0;
		for(nCnt = 0;nCnt <totalNum;nCnt++)
		{
			m_pLoadMem[nCnt] = TRUE;
		}
		// 2007-10-18 by bhsohn �� �ƸӴ� ������ �� �ε�
		// �� �ƸӴ� ������ �� �ε�
		BOOL bMyArmor = FALSE;
		if(IsArmorMesh(nMeshIndex))
		{
			if(_SELECT == g_pD3dApp->GetGameState())				
			{
				bMyArmor = TRUE;
			}
			else if(NULL != g_pShuttleChild && nMeshIndex == g_pShuttleChild->GetUnitNum())
			{
				bMyArmor = TRUE;
			}
		}
		// end 2007-10-18 by bhsohn �� �ƸӴ� ������ �� �ε�

		// 2009-03-09 by bhsohn �Ϻ��� �⺻ �ƸӸ� �ε�
		BOOL bDefaultArmor = (IsArmorMesh(nMeshIndex) && IsLowQuality() && (FALSE == bMyArmor));
		if(IsArmorMesh(nMeshIndex))
			bDefaultArmor = TRUE;

		// end 2009. 07. 27 by ckPark ���絵 �⺻ �Ƹ� �ؽ��ĸ� �ε��ϵ��� ����
		
		// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		// �κ��Ƹ� �ؽ��� 2�� ���
		if( IsArmorMesh( nMeshIndex ) )
		{
			char szArmorNum[32];
			itoa( nMeshIndex, szArmorNum, 10 );
			szArmorNum[0] = '0';
			szArmorNum[1] = '8';
			
			int  nArmorItemNum = atoi( szArmorNum );
			
			ITEM* pItem = g_pDatabase->GetServerItemInfo( nArmorItemNum );
			if( pItem && COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) )
				bDefaultArmor = FALSE;
		}
		// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		

		if(bDefaultArmor)
		{			
			for(nCnt = 1;nCnt <totalNum-2;nCnt++)
			{
				// ù���� �ƸӸ� ���� ��� Disable
				m_pLoadMem[nCnt] = FALSE;
			}
		}
		// end 2009-03-09 by bhsohn �Ϻ��� �⺻ �ƸӸ� �ε�
	}
	// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	pHeader = m_pGameData->GetStartPosition();
	int i=0;
	while(pHeader)
	{
		m_pDataHeader[i] = *pHeader;
		m_pDataHeader[i++].m_pData = pHeader->m_pData;
		pHeader = m_pGameData->GetNext();
	}
	
    if (pdeMesh->pframeRoot == NULL) 
    {
        hr = E_OUTOFMEMORY;
		ReleaseData(hr);
        return E_FAIL;
    }    
	pMemory->lpMemory = m_pDataHeader[0].m_pData;
	pMemory->dSize = m_pDataHeader[0].m_DataSize;
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep2()
{
	FLOG( "CSkinnedMesh::LoadMeshHierarchyFromMemStep2" );	

	dwOptions = D3DXMESH_MANAGED;
	HRESULT hr;
    hr = DirectXFileCreate(&pxofapi);
    if (FAILED(hr))
	{
		ReleaseData(hr);
        return E_FAIL;    
	}  
    
    // Register templates for d3drm.
    hr = pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES,
        D3DRM_XTEMPLATE_BYTES);
    if (FAILED(hr))
	{
		ReleaseData(hr);
        return E_FAIL;    
	}  
	
    hr = pxofapi->RegisterTemplates((LPVOID)szTemplates,
        strlen(szTemplates));
	if (FAILED(hr))
	{
		ReleaseData(hr);
        return E_FAIL;    
	} 
	 return S_OK;
}
// 
HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep3()
{
	FLOG( "CSkinnedMesh::LoadMeshHierarchyFromMemStep3" );	
	
	HRESULT hr;
	hr = pxofapi->CreateEnumObject((LPVOID)pMemory,
        DXFILELOAD_FROMMEMORY,
        &pxofenum);
    if (FAILED(hr))
	{
		ReleaseData(hr);
        return E_FAIL;    
	}
	return S_OK;
 }

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep4(structLoadingGameData* pGameData)
{
 	FLOG( "CSkinnedMesh::LoadMeshHierarchyFromMemStep4" );
 	int totalNum = pGameData->pGameData->GetTotalNumber();
	int cnt=0;
	if(pGameData->Text_Cnt == 0)
	{
		cnt = 1;
		pGameData->Text_Cnt = 1;
	}
	else
	{
		cnt = pGameData->Text_Cnt;
 	}
 	
	if(totalNum > 1)
	{
		if(!m_pTexture)
		{		
			m_pTexture = new LPDIRECT3DTEXTURE9[ pGameData->pGameData->GetTotalNumber() - 1];
		}
		m_bTotalTextureNum = totalNum - 1;
		for(int i = cnt; i < totalNum; i++)
		{
			// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
			int nLoadIdx = i - 1;
			if(FALSE == IsLoadTexture(nLoadIdx))
			{
				continue;
			}
			// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����			
			
			if( FAILED( D3DXCreateTextureFromFileInMemory(m_pd3dDevice, (LPCVOID)m_pDataHeader[i].m_pData, 
				m_pDataHeader[i].m_DataSize, &(m_pTexture[i-1])))) 
			{
				if(g_pD3dApp && g_pD3dApp->m_pChat)
				{
					char errMsg[512];
					sprintf_s(errMsg, 512, STRERR_C_RESOURCE_0012, m_pDataHeader[i].m_FileName);
					g_pD3dApp->m_pChat->CreateChatChild(errMsg, COLOR_ERROR);// "���ҽ� ���� �ε��� ������ �־ ������Ʈ ������ �ߴܵ�."
					pGameData->Step = _RESOURCE_LOADING_END;					
				}
				DBGOUT("\n ERROR : Loading Resource File .\n\n");
				return E_FAIL;
			}
			if(TimeOut())
			{	
				pGameData->Text_Cnt = ++cnt;
				return E_FAIL; 
			}
		}
	}
	else
	{
		m_pTexture = new LPDIRECT3DTEXTURE9[1];
		m_pTexture[0] = NULL;
	}
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep5()
{
	FLOG( "CSkinnedMesh::LoadMeshHierarchyFromMemStep5" );		
	HRESULT hr;
	 while (SUCCEEDED(pxofenum->GetNextDataObject(&pxofobjCur)))
    {
		hr = LoadFrames(pxofobjCur, pdeMesh, dwOptions, m_dwFVF,
				m_pd3dDevice,pdeMesh->pframeRoot);		
		GXRELEASE(pxofobjCur);
	
		if(FAILED(hr))
		{
			ReleaseData(hr);
			return E_FAIL;    
		}		
 		if(TimeOut())
 		{	
 			return E_FAIL;
 		}        
	}    
	return S_OK;
}

HRESULT CSkinnedMesh::LoadMeshHierarchyFromMemLoadStep6()
{
	FLOG( "CSkinnedMesh::LoadMeshHierarchyFromMemStep6" );		
	
	HRESULT hr;
	hr = FindBones(pdeMesh->pframeRoot, pdeMesh);

    if (FAILED(hr))
	{
		ReleaseData(hr);
        return E_FAIL;    
	}
    
    delete []pdeMesh->szName;
	pdeMesh->szName = NULL;
	
    DeleteSelectedMesh();
	
    // link into the draw list
    pdeMesh->pdeNext = m_pdeHead;
    m_pdeHead = pdeMesh;
    
    m_pdeSelected = pdeMesh;
    m_pmcSelectedMesh = pdeMesh->pframeRoot->pmcMesh;
    
    m_pframeSelected = pdeMesh->pframeRoot;
    
	hr = CalculateBoundingSphere(pdeMesh);
	m_fRadius = pdeMesh->fRadius;
	m_vCenter = pdeMesh->vCenter;
	
	if (FAILED(hr))
	{
		ReleaseData(hr);
        return E_FAIL;    
	}
    
    SetProjectionMatrix();
    
    m_pdeSelected->fCurTime = 0.0f;
    m_pdeSelected->fMaxTime = 2000.0f;
    
    D3DXMatrixTranslation(&m_pdeSelected->pframeRoot->matRot,
        -pdeMesh->vCenter.x, -pdeMesh->vCenter.y, -pdeMesh->vCenter.z);
    m_pdeSelected->pframeRoot->matRotOrig = m_pdeSelected->pframeRoot->matRot;

	ReleaseData(hr);
 	return S_OK;
}
void CSkinnedMesh::ReleaseData(HRESULT hr)
{
	//	SAFE_DELETE(m_pGameData);// m_pDataHeader�� m_pData�� ��ȿ�ϰ�, �������� ��ȿ�ϴ�.
	SAFE_DELETE(pMemory);
	int totalNum = m_pGameData->GetTotalNumber();
	for(int i=0; i<totalNum; i++)
		m_pDataHeader[i].m_pData = NULL;
    GXRELEASE(pxofobjCur);
    GXRELEASE(pxofenum);
    GXRELEASE(pxofapi);
    
    if (FAILED(hr))
    {
		// ���� �߻�
        delete pdeMesh;
		pdeMesh = NULL;
    }
	
    SDrawElement *pdeCur;
    pdeCur = m_pdeHead;
    while (pdeCur != NULL)
    {
        D3DXMatrixIdentity(&pdeCur->pframeRoot->matRot);
        D3DXMatrixIdentity(&pdeCur->pframeRoot->matTrans);
        pdeCur = pdeCur->pdeNext;
    }
}

void CSkinnedMesh::DeleteLoadingGameData()
{
	if(this->GetIsLoadingFlag())
	{
		g_pD3dApp->DeleteLoadingGameData(this);
	}
}

BOOL CSkinnedMesh::TimeOut()
{
	if( timeGetTime() - m_StartTime > LOADING_TICK )
	{
		m_StartTime = NULL;
		return TRUE;
	}
	return FALSE;
}

void CSkinnedMesh::SetIsLoadingStartTime()
{
	m_StartTime = timeGetTime();
}

int CSkinnedMesh::GetIsLoadingStartTime()
{
// 2011-04-12 by hsson �ܰ躰 �ε� ���� ����
	if( NULL == m_StartTime )
		return 1;
//end 2011-04-12 by hsson �ܰ躰 �ε� ���� ����

	return m_StartTime;
}
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
