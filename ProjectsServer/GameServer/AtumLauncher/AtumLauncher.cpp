// AtumLauncher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AtumLauncher.h"
#include "AtumLauncherDlg.h"
#include "MGameDecryption.h"
#include "AtumError.h"				// 2006-10-02 by cmkwon
#include "RegistryControl.h"	// 2008-10-16 by cmkwon, Gameforge4D(Eng,Deu) Launcher Registry event ���� - 

// 2012-10-21 by mspark, ���� ���� �� ���μ����� �����ִ� ACEonline.atm ����
#include <windows.h>
#include <stdlib.h>
#include <Tlhelp32.h>
// end 2012-10-21 by mspark, ���� ���� �� ���μ����� �����ִ� ACEonline.atm ����


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// check: MSG�� print�ϴ� level�� �����ϱ� ����(AtumMonitor�� ���� ����), by kelovon
//BYTE		g_exchangeMsgPrintLevel = PRINTLEVEL_NO_MSG;
//-- CMKWON 2003-07-12 Global.h�� Global.cpp���� ������

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherApp

BEGIN_MESSAGE_MAP(CAtumLauncherApp, CWinApp)
	//{{AFX_MSG_MAP(CAtumLauncherApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherApp construction

CAtumLauncherApp::CAtumLauncherApp()
:CWinApp(REGISTRY_BASE_PATH)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_hMutexMonoInstance	= NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAtumLauncherApp object

CAtumLauncherApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherApp initialization
#ifdef _WARRIOR_ANTICHEAT
char* sModule[] = { "SbieDll.dll", "dbghelp.dll" };   

bool IsInSandbox()
{
    for( int i = 0; i < ( sizeof( sModule ) / sizeof( char* ) ); i++ )
    {
        if( GetModuleHandle( sModule[ i ] ) )
        {
            return true;
        }
    }
    return false;
}

/*DWORD __forceinline IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
  PCONTEXT ctx = ep->ContextRecord;

  ctx->Ebx = -1; // Not running VPC
  ctx->Eip += 4; // skip past the "call VPC" opcodes
  return EXCEPTION_CONTINUE_EXECUTION;
  // we can safely resume execution since we skipped faulty instruction
}

// High level language friendly version of IsInsideVPC()
bool IsInsideVPC()
{
  bool rc = false;

  __try
  {
    _asm push ebx
    _asm mov  ebx, 0 // It will stay ZERO if VPC is running
    _asm mov  eax, 1 // VPC function number

    // call VPC 
    _asm __emit 0Fh
    _asm __emit 3Fh
    _asm __emit 07h
    _asm __emit 0Bh

    _asm test ebx, ebx
    _asm setz [rc]
    _asm pop ebx
  }
  // The except block shouldn't get triggered if VPC is running!!
  __except(IsInsideVPC_exceptionFilter(GetExceptionInformation()))
  {
  }

  return rc;
}

bool IsInsideVMWare()
{
  bool rc = true;

  __try
  {
    __asm
    {
      push   edx
      push   ecx
      push   ebx

      mov    eax, 'VMXh'
      mov    ebx, 0 // any value but not the MAGIC VALUE
      mov    ecx, 10 // get VMWare version
      mov    edx, 'VX' // port number

      in     eax, dx // read port
                     // on return EAX returns the VERSION
      cmp    ebx, 'VMXh' // is it a reply from VMWare?
      setz   [rc] // set return value

      pop    ebx
      pop    ecx
      pop    edx
    }
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
    rc = false;
  }

  return rc;
}
*/
int CheckHardwareBreakpoints()
{
    unsigned int NumBps = 0;

    // This structure is key to the function and is the 
    // medium for detection and removal
    CONTEXT ctx;
    ZeroMemory(&ctx, sizeof(CONTEXT)); 
    
    // The CONTEXT structure is an in/out parameter therefore we have
    // to set the flags so Get/SetThreadContext knows what to set or get.
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS; 
    
    // Get a handle to our thread
    HANDLE hThread = GetCurrentThread();

    // Get the registers
    if(GetThreadContext(hThread, &ctx) == 0)
        return -1;

    // Now we can check for hardware breakpoints, its not 
    // necessary to check Dr6 and Dr7, however feel free to
    if(ctx.Dr0 != 0)
        ++NumBps; 
    if(ctx.Dr1 != 0)
           ++NumBps; 
    if(ctx.Dr2 != 0)
           ++NumBps; 
    if(ctx.Dr3 != 0)
        ++NumBps;
        
    return NumBps;
}
bool MemoryBreakpointDebuggerCheck()
{
     unsigned char *pMem = NULL;
     SYSTEM_INFO sysinfo = {0}; 
     DWORD OldProtect = 0;
     void *pAllocation = NULL; // Get the page size for the system 
 
    GetSystemInfo(&sysinfo); // Allocate memory 
 
    pAllocation = VirtualAlloc(NULL, sysinfo.dwPageSize, 
                        MEM_COMMIT | MEM_RESERVE, 
                         PAGE_EXECUTE_READWRITE); 
        
    if (pAllocation == NULL)
        return false; 
    
    // Write a ret to the buffer (opcode 0xc3)
    pMem = (unsigned char*)pAllocation;
    *pMem = 0xc3; 
    
    // Make the page a guard page         
    if (VirtualProtect(pAllocation, sysinfo.dwPageSize, 
                    PAGE_EXECUTE_READWRITE | PAGE_GUARD, 
                    &OldProtect) == 0)
    {
        return false;
    } 
    
    __try
    {
        __asm
        {
            mov eax, pAllocation
            // This is the address we'll return to if we're under a debugger
            push MemBpBeingDebugged
            jmp eax // Exception or execution, which shall it be :D?
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        // The exception occured and no debugger was detected
        VirtualFree(pAllocation, NULL, MEM_RELEASE);
        return false;
    }     
    
    __asm{MemBpBeingDebugged:}
    VirtualFree(pAllocation, NULL, MEM_RELEASE);
    return true;
}

bool CheckForCCBreakpointXor55(void* pMemory,  size_t SizeToCheck)
 {
     unsigned char *pTmp = (unsigned char*)pMemory;
    unsigned char tmpchar = 0;
        
    for (size_t i = 0; i < SizeToCheck; i++)
     {
        tmpchar = pTmp[i];
        if( 0x99 == (tmpchar ^ 0x55) ) // 0xCC xor 0x55 = 0x99
            return true;
     } 

    return false;
 }
#endif
BOOL CAtumLauncherApp::InitInstance()
{
#ifdef _WARRIOR_ANTICHEAT
		//2015-03-03 for anti cheat
		if(IsInSandbox())
		{
			MessageBox(NULL, "Using Sandboxie is not allowed! \nPlease run DreamACE outside of Sandboxie.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
/*		if(IsInsideVPC())
		{
			MessageBox(NULL, "Using VirtualPC is not allowed! \n Please run DreamACE outside of VirtualPC.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		if(IsInsideVMWare())
		{
			MessageBox(NULL, "Using VMWare is not allowed! \n Please run DreamACE outside of VMWare", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
*/		if(MemoryBreakpointDebuggerCheck())
		{
			MessageBox(NULL, "Cheat Engine not allowed! \n Remove Cheat Engine, Restart Computer and Login again.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		if(CheckHardwareBreakpoints() != 0)
		{
			MessageBox(NULL, "Debug software detected! \n Close your debug software and Login Again.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		HKEY hSubKey = NULL;
		if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Cheat Engine", 0L, KEY_ALL_ACCESS, &hSubKey))
		{// warriorsw checking cheat engine
		MessageBox(NULL, "Cheat Engine not allowed! \n Remove Cheat Engine, Restart Computer and Login again.", "DreamACE Anti-Cheat", MB_OK);
		exit(1);
		return E_FAIL;
		}
		//endof 2015-03-03
#endif
	// 2012-10-21 by mspark, ���� ���� �� ���μ����� �����ִ� ACEonline.atm ����
#ifdef S_REMANING_PROCESS_REMOVE_MSPARK
	//
#else
	RemainingProcessRemove();
#endif
	// end 2012-10-21 by mspark, ���� ���� �� ���μ����� �����ִ� ACEonline.atm ����

	m_hMutexMonoInstance = CreateMutex(NULL, TRUE, "AtumLauncher");
	if(NULL == m_hMutexMonoInstance)
	{
		MessageBox(NULL, "CreateMutex Error", "AtumLauncher", MB_OK);
		return FALSE;
	}

	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, "ERROR : \n  Application is running already...", "AtumLauncher", MB_OK);
		return FALSE;
	}

	srand(timeGetTime());		// 2007-09-11 by cmkwon, ��Ʈ�� ȭ��Ű���� ���� - ��ġ�� ������ ���ؼ� �߰�

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	DisableXPServicePack2();

	SetRegistryKey(STRMSG_REG_STRING_REGISTRYKEY_NAME);
// 2007-03-06 by cmkwon, ������ ���� �ڵ�� �ʿ� ����
//#if !defined(_ATUM_DEVELOP) && !defined(_MASANG15_SERVER) && !defined(_MASANG51_SERVER) && !defined(_GLOBAL_ENG_SERVER) && !defined(_VTC_VIET_SERVER) && !defined(_KOREA_SERVER_2)
//	///////////////////////////////////////////////////////////////////////////////
//	// 2004-06-21 cmkwon ������ ������ �Ķ���� �ޱ�<�� 7��>
//	//	param1 : INET			==> ���ͳ��� ���� ���� �Ǿ������� ����, SpaceCowboy�� �׻� INET
//	//	param2 : 3850			==> ��ũ���� �õ尪
//	//	param3 : ������ ����	==> �����ӿ� �α����� ����
//	//	param4 : ���� ��ũ���ǵ� ���ڿ�	==> ������ ������ param2�� ��ũ������ ���ڿ�
//	//	param5 : ��� ��ũ���ǵ� ���ڿ�	==> ������ ����� ��ũ������ ���ڿ�, �õ尪�� param2�� �ٸ���
//	//	param6 : ����			==> ���� = 1, ���� = 2
//	//	param7 : ����⵵		==> ����⵵
//	if(__argc != 8)
//	{
//		DbgOut(STRERR_S_ATUMLAUNCHER_0000, __argc);
//		ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
//		return FALSE;
//	}
//
//	int nEnSeed=0;
//	STRNCPY_MEMSET(g_szMGameExcuteType, __argv[1], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameSeed, __argv[2], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameID, __argv[3], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameEncryptedID, __argv[4], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameEncryptedPWD, __argv[5], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameSEX, __argv[6], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameYear, __argv[7], MGAME_MAX_PARAM_STRING_SIZE);
//
//
//	///////////////////////////////////////////////////////////////////////////////
//	// Mutex Check
//	HANDLE hMutex = NULL;	
//	hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, TRUE, (LPTSTR)g_szMGameEncryptedPWD);  // ��ȣȭ���н����尪�� �ִ´�
//	if(hMutex == NULL)
//	{
//		DBGOUT(STRERR_S_ATUMLAUNCHER_0001);
//		return FALSE;
//	}	
//	ReleaseMutex(hMutex);
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// ExcuteType Check
//	if(strcmp(g_szMGameExcuteType, "INET"))
//	{
//		DBGOUT(STRERR_S_ATUMLAUNCHER_0002, g_szMGameExcuteType);
//		return FALSE;
//	}
//
//	nEnSeed = atoi(g_szMGameSeed);
//	char szDecryptedID[MGAME_MAX_PARAM_STRING_SIZE];	
//	STRNCPY_MEMSET(szDecryptedID, (char *)decrypt((unsigned char*)g_szMGameEncryptedID, nEnSeed), MGAME_MAX_PARAM_STRING_SIZE);
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// ID Check
//	if(strcmp(g_szMGameID, szDecryptedID))
//	{
//		DBGOUT(STRERR_S_ATUMLAUNCHER_0003, szDecryptedID);
//		return FALSE;
//	}
//
//	DBGOUT("%s %s %s %s %s %s %s\n"
//		, __argv[1], __argv[2], __argv[3], __argv[4]
//		, __argv[5], __argv[6], __argv[7]);
//	//
//	///////////////////////////////////////////////////////////////////////////////
//	//*/
//#endif // _ATUM_DEVELOP
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-02 by cmkwon
	Err_t errCode = GSetExcuteParameterList(__argc, __argv);
	if(ERR_NO_ERROR != errCode)
	{
		DbgOut("Set Excute Parameter error !! Error = %s(0x%X)\r\n", GetErrorString(errCode), errCode);
		return FALSE;
	}

	SEXCUTE_PARAMETER exeParam;		MEMSET_ZERO(&exeParam, sizeof(exeParam));
	exeParam.i_nExcuteFileType		= EXCUTE_FILE_TYPE_SC_LAUNCHER_ATM;
	errCode = GCheckExcuteParameterList(&exeParam);
	if(ERR_NO_ERROR != errCode)
	{
		DbgOut("Check Excute Parameter error !! Error = %s(0x%X)\r\n", GetErrorString(errCode), errCode);
		return FALSE;
	}
	//
	///////////////////////////////////////////////////////////////////////////////
	
	
	CAtumLauncherDlg dlg;
	m_pMainWnd = &dlg;
	ReadProfile();
	ReadCrocessProfile();
	if(0 != strcmp(exeParam.o_szPreServerIP0, ""))
	{
		dlg.m_szPreServerIPReg	= exeParam.o_szPreServerIP0;
	}
	dlg.m_nBirthYear			= exeParam.o_nBirthYear;	// 2007-06-05 by cmkwon, ����⵵

	dlg.m_ctlbWindowMode		= (dlg.m_nWindowModeReg == GAME_MODE_WINDOW) ? TRUE : FALSE;	// 2008-01-08 by cmkwon, â ��� ���� ����

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CAtumLauncherApp::DisableXPServicePack2()
{
// 2005-02-07 by cmkwon, Launcher�� �̹� ��ϵǾ�����, �ʿ���� �ڵ尰��
//	HKEY hKey;
//
//	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
//				"SYSTEM\\ControlSet001\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile\\AuthorizedApplications\\List", 
//				0L, 
//				KEY_ALL_ACCESS, 
//				&hKey) == ERROR_SUCCESS )
//	{
//		// ������Ʈ���� ���� �Ѵٸ�
//
//
//		// 1. ���� ��ġ ���丮�� ��´�.
//		HKEY hSubKey = NULL;
//		
//		BYTE byBuffer[_MAX_PATH];
//		DWORD dwType, dwSize = _MAX_PATH;
//		char szExecDir[_MAX_PATH] = {0}, szExecPath[_MAX_PATH] = {0};
//		
//		// ���� ���� ������Ʈ����  ���丮 ������ Ʋ�� ���� �ִ�.
//		if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceCowboy", 0L, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS )
//		{
//			if (RegQueryValueEx(hSubKey, "InstallPath", NULL, &dwType, byBuffer, &dwSize) == ERROR_SUCCESS )
//			{
//				RegCloseKey(hSubKey);
//
//
//				sprintf( szExecPath, "%s", (char*)byBuffer);
//
//				// ó�� ����� ������ ���� �ִ� ��� �̹� �˰� �ִ� �̸��̹Ƿ� 
//				char *pdest;
//				int nCount;
//				pdest = strstr(szExecPath, "SpaceCowboy.exe" );
//				nCount = pdest - szExecPath;
//				strncpy( szExecDir, szExecPath, nCount);
//				TRACE("\n%s\n", szExecDir );
//			}
//		}
//		else
//		{
//			return;
//		}
//
//		// 2. ���� ��Ͽ� ������ ���ڿ��� �����.
//		// �����̽�ī�캸�� ����ؾ��Ұ� 1��( Launcher.atm )  �����Ƿ�
//
//		char szName1[_MAX_PATH] = {0};
//
//		sprintf(szName1, "%s%s", szExecDir, "Launcher.atm");
//		
//		TRACE("\n%s\n", szName1);
//
//		// 3. ������ ���ڿ��� ���� �����.
//		// �� ���� �̹� xp �� ��ǻ�Ϳ��� �׽�Ʈ �ϰ� ������Ʈ���� ��ϵ�  ���� �״�� ����.
//		// :*:Enabled:SpaceCowboyLauncher
//		char szValue1[_MAX_PATH] = {0};
//
//		sprintf(szValue1, "%s%s", szName1, ":*:Enabled:SpaceCowboyLauncher" );
//		
//		TRACE("\n%s\n", szValue1);
//
//		// 4. ���ڿ��� ������Ƽ�� �̹� �ִ°� Ȯ���Ѵ�
//
//		BYTE byBuffer1[_MAX_PATH];
//		DWORD dwType1, dwSize1 = _MAX_PATH;
//		char szValue_old1[_MAX_PATH] = {0};
//		if (RegQueryValueEx(hKey, szName1, NULL, &dwType1, byBuffer1, &dwSize1) == ERROR_SUCCESS )
//		{
//			// �̹� �ִٸ�
//			sprintf( szValue_old1, "%s", (char*)byBuffer1);
//
//			// ���� �ִٸ� ���� �о Enabled ���� Disabled���� Ȯ�� �Ѵ�
//
//			char *pdest;
//			pdest = strstr(szValue_old1, "Disabled" );
//
//			if( pdest != NULL )
//			{
//				//���� Disabled �̶�� Enabled�� �����ؼ� ���� �ٲ��ش�
//				RegSetValueEx(hKey, szName1, 0L, REG_SZ, (BYTE *)szValue1, strlen(szValue1)+1);
//				TRACE("Disabled ����\n%s\n", szValue1);
//			}
//			// Enabled �̶�� �׳� ���
//		}
//		else
//		{
//			// ������
//			RegSetValueEx(hKey, szName1, 0L, REG_SZ, (BYTE *)szValue1, strlen(szValue1)+1);
//			TRACE("���� ���� \n%s\n", szValue1);
//		}
//	}
}

void CAtumLauncherApp::WriteProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\Atum Launcher\Configuration
	CAtumLauncherDlg *pDlg = (CAtumLauncherDlg*)AfxGetMainWnd();

	if(pDlg)
	{
		WriteProfileString(_T("Configuration"), STRMSG_REG_STRING_CLIENT_VERSION, pDlg->m_CurrentVersion.GetVersionString());
// 2007-12-27 by cmkwon, �������� ��� ��� �߰� -
//		WriteProfileInt(_T("Configuration"), _T("WindowDegree"), pDlg->m_nWindowDegreeReg);
		WriteProfileString(_T("Configuration"), STRMSG_REG_KEY_NAME_WINDOWDEGREE_NEW, pDlg->m_csWindowsResolutionReg);	// 2007-12-27 by cmkwon, �������� ��� ��� �߰� - 
		pDlg->m_szAccountNameReg.MakeLower();

#if defined(_DEBUG)
		///////////////////////////////////////////////////////////////////////////////
		// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher���� ���� ���� ���� ����ϱ�(K0000243) - 
		if(pDlg->m_ctlBtnRememberID.GetCheck())
		{
			WriteProfileString( _T("Configuration"), _T("AccountName"), pDlg->m_szAccountNameReg);
		}
		else
		{
			WriteProfileString( _T("Configuration"), _T("AccountName"), "");
		}
#else
		WriteProfileString( _T("Configuration"), _T("AccountName"), "");
#endif

		WriteProfileString( _T("Configuration"), _T("ServerGroupName"), pDlg->m_strServerGroupName);
#ifdef _ATUM_DEVELOP
		WriteProfileString(_T("Configuration"), _T("ExecutableBinary"), pDlg->m_szExecutableBinaryReg);
		WriteProfileString(_T("Configuration"), _T("ExecutePath"), pDlg->m_szExecutePathReg);
		WriteProfileString(_T("Configuration"), _T("PreServerIPAddress"), pDlg->m_szPreServerIPReg);
		WriteProfileString( _T("Configuration"), _T("Password"), pDlg->m_szPasswordReg);
		WriteProfileString(_T("Configuration"), _T("PreServerIPHistory"), pDlg->m_szPreServerIPHistoryReg);
		WriteProfileString(_T("Configuration"), _T("ExecuteBinHistory"), pDlg->m_szExecuteBinHistoryReg);
		WriteProfileString(_T("Configuration"), _T("ExecutePathHistory"), pDlg->m_szExecutePathHistoryReg);
#endif

		SREG_DATA_EXE_2 regDataExe2;
		regDataExe2.resetREG_DATA_EXE_2();
		regDataExe2.ClientVersion		= pDlg->m_CurrentVersion;
// 2007-12-27 by cmkwon, �������� ��� ��� �߰� -
//		regDataExe2.WindowDegree		= pDlg->m_nWindowDegreeReg;
		STRNCPY_MEMSET(regDataExe2.WindowDegree, (LPSTR)(LPCSTR)(pDlg->m_csWindowsResolutionReg), SIZE_MAX_WINDOW_DEGREE_NAME);	// 2007-12-27 by cmkwon, �������� ��� ��� �߰� -

// 2007-05-09 by cmkwon, ������ ������ �������� �ʴ´�
//		sprintf(regDataExe2.BeforeAccountName, "%s", pDlg->m_szAccountNameReg);

#if defined(_DEBUG)
		///////////////////////////////////////////////////////////////////////////////
		// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher���� ���� ���� ���� ����ϱ�(K0000243) - 
		if(pDlg->m_ctlBtnRememberID.GetCheck())
		{
			sprintf(regDataExe2.BeforeAccountName, "%s", pDlg->m_szAccountNameReg);
		}
#endif

		sprintf(regDataExe2.SelectedServerGroupName, "%s", pDlg->m_strServerGroupName);

		regDataExe2.IsWindowMode		= pDlg->m_ctlbWindowMode;			// 2008-01-03 by cmkwon, �������� ���� �����ϱ� - 	

		Err_t errCode = GSaveExe2VersionInfo(&regDataExe2, STRMSG_VERSION_INFO_FILE_NAME);

		///////////////////////////////////////////////////////////////////////////////
		// 2009-08-31 by cmkwon, Gameforge4D ���Ӱ��� ����â ���� - 
		WriteProfileInt(_T("Configuration"), "GameGuardAgreement", pDlg->m_bGuardAgreementReg);
	}
}

void CAtumLauncherApp::ReadProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\Atum Launcher\Configuration
	CAtumLauncherDlg *pDlg = (CAtumLauncherDlg*)AfxGetMainWnd();

	if(pDlg)
	{
		SREG_DATA_EXE_2 regDataExe2;
		regDataExe2.resetREG_DATA_EXE_2();

		Err_t errCode = GLoadExe2VersionInfo(&regDataExe2, STRMSG_VERSION_INFO_FILE_NAME);
		if(ERR_NO_ERROR != errCode
			|| FALSE == regDataExe2.ClientVersion.IsValidVersionInfo())
		{
			regDataExe2.ClientVersion.SetVersion((char*)(LPCSTR)GetProfileString( _T("Configuration"), STRMSG_REG_STRING_CLIENT_VERSION, "0.0.0.0"));
		}

// 2007-12-27 by cmkwon, �������� ��� ��� �߰� -
//		if(ERR_NO_ERROR != errCode
//			|| -1 == regDataExe2.WindowDegree)
		if(ERR_NO_ERROR != errCode
			|| 0 == strncmp(regDataExe2.WindowDegree, "", SIZE_MAX_WINDOW_DEGREE_NAME))
		{
			// 2007-12-27 by cmkwon, �������� ��� ��� �߰� -
			//regDataExe2.WindowDegree = GetProfileInt(_T("Configuration"), _T("WindowDegree"), WINDOW_DEGREE_1024x768_MEDIUM);
			CString csWDegree = GetProfileString(_T("Configuration"), STRMSG_REG_KEY_NAME_WINDOWDEGREE_NEW, STRMSG_WINDOW_DEGREE_1024x768_MEDIUM);
			STRNCPY_MEMSET(regDataExe2.WindowDegree, (char*)(LPCSTR)csWDegree, SIZE_MAX_WINDOW_DEGREE_NAME);
		}

		///////////////////////////////////////////////////////////////////////////////		
		// 2008-01-03 by cmkwon, �������� ���� �����ϱ� - ������Ʈ������ �������� �ʴ´�. �������ϸ��� �̿��Ѵ�.

		
		if(ERR_NO_ERROR != errCode
			|| 0 == strncmp(regDataExe2.BeforeAccountName, "", SIZE_MAX_ACCOUNT_NAME))
		{
			CString strTm;
			strTm = GetProfileString( _T("Configuration"), _T("AccountName"), NULL);
			strTm.MakeLower();
			sprintf(regDataExe2.BeforeAccountName, "%s", strTm);
		}

		if(ERR_NO_ERROR != errCode
			|| 0 == strncmp(regDataExe2.SelectedServerGroupName, "", SIZE_MAX_SERVER_NAME))
		{
			CString strTm;
			strTm = GetProfileString( _T("Configuration"), _T("ServerGroupName"), "");
			sprintf(regDataExe2.SelectedServerGroupName, "%s", strTm);
		}

		pDlg->m_CurrentVersion				= regDataExe2.ClientVersion;
// 2007-12-27 by cmkwon, �������� ��� ��� �߰� -
//		pDlg->m_nWindowDegreeReg			= regDataExe2.WindowDegree;
		pDlg->m_csWindowsResolutionReg		= regDataExe2.WindowDegree;		// 2007-12-27 by cmkwon, �������� ��� ��� �߰� -
		pDlg->m_szAccountNameReg			= regDataExe2.BeforeAccountName;
		pDlg->m_strServerGroupName			= regDataExe2.SelectedServerGroupName;
		pDlg->m_CurrentDelFileListVersion.SetVersion("0.0.0.0");		// 2007-05-09 by cmkwon, DeleteFileList Version�� �׻� 0.0.0.0
		pDlg->m_CurrentNoticeVersion.SetVersion("0.0.0.0");				// 2007-05-09 by cmkwon, NoticeVersion�� ������� ����

		///////////////////////////////////////////////////////////////////////////////
		// 2008-01-03 by cmkwon, �������� ���� �����ϱ� - 
		if(regDataExe2.IsWindowMode)
		{
			pDlg->m_nWindowModeReg = GAME_MODE_WINDOW;					// 2008-01-03 by cmkwon, �������� ���� �����ϱ� - 
		}
		else
		{
			pDlg->m_nWindowModeReg = GAME_MODE_FULLSCREEN;					// 2007-05-09 by cmkwon, �׻� FullScreenMode
		}

// 2008-04-23 by cmkwon, PreServer �ּҸ� IP�� ������ �Ѵ� ���� - �Ʒ��� ���� ����
// 		char szTemp[16];
// 		sprintf(szTemp, "%d.%d.%d.%d"
// 			, MAIN_PRE_SERVER_IP_0, MAIN_PRE_SERVER_IP_1
// 			, MAIN_PRE_SERVER_IP_2, MAIN_PRE_SERVER_IP_3);
// 		pDlg->m_szPreServerIPReg = szTemp;
		// 2008-04-23 by cmkwon, PreServer �ּҸ� IP�� ������ �Ѵ� ���� - 
		char szPreServer[1024];
		MEMSET_ZERO(szPreServer, 1024);
		XOR::XORDecrypt(szPreServer, CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR, STR_XOR_KEY_STRING_PRE_SERVER_ADDRESS);
		pDlg->m_szPreServerIPReg	= szPreServer;

		char szExecute[512];
		sprintf(szExecute, ".\\%s", CLIENT_EXEUTE_FILE_NAME);
		

#ifdef _ATUM_DEVELOP
		pDlg->m_szPreServerIPHistoryReg = GetProfileString(_T("Configuration"), _T("PreServerIPHistory"), "");
		pDlg->m_szExecuteBinHistoryReg = GetProfileString(_T("Configuration"), _T("ExecuteBinHistory"), "");
		pDlg->m_szExecutePathHistoryReg = GetProfileString(_T("Configuration"), _T("ExecutePathHistory"), "");
		pDlg->m_szExecutableBinaryReg = GetProfileString( _T("Configuration"), _T("ExecutableBinary"), szExecute);
		pDlg->m_szExecutePathReg = GetProfileString( _T("Configuration"), _T("ExecutePath"), ".\\ACEonline\\");
		pDlg->m_szPasswordReg = GetProfileString( _T("Configuration"), _T("Password"), NULL);
		if (pDlg->m_szExecutePathReg.Right(1) != "\\")
		{
			pDlg->m_szExecutePathReg += "\\";
		}
#endif

#if defined(_DEBUG)
		///////////////////////////////////////////////////////////////////////////////
		// 2008-10-16 by cmkwon, Gameforge4D(Eng,Deu) Launcher Registry event ���� - 		
		pDlg->m_strEventURL.Empty();

		CRegistryControl regCtrl;		
		char szRegKeyPath[MAX_PATH]	= {0x00,};	
		sprintf(szRegKeyPath, "SOFTWARE\\%s", "Gameforge4d\\AirRivals");	// "HKEY_LOCAL_MACHINE\SOFTWARE\Gameforge4d\AirRivals\launcherurl"
		if(regCtrl.RegistryControlInit(HKEY_LOCAL_MACHINE, szRegKeyPath))
		{
			char szEventURL[1024];
			MEMSET_ZERO(szEventURL, 1024);
			if(regCtrl.ReadString("launcherurl", szEventURL, 1024))
			{
				pDlg->m_strEventURL = szEventURL;
			}
		}
#endif


		///////////////////////////////////////////////////////////////////////////////
		// 2009-08-31 by cmkwon, Gameforge4D ���Ӱ��� ����â ���� - 
		pDlg->m_bGuardAgreementReg	= GetProfileInt(_T("Configuration"), "GameGuardAgreement", FALSE);
	}
}

void CAtumLauncherApp::ReadCrocessProfile()
{
	CAtumLauncherDlg *pDlg = (CAtumLauncherDlg*)AfxGetMainWnd();

	if(pDlg)
	{
		pDlg->m_szCrocessSuffix = "";
	}
	else
	{
		return;
	}

	HKEY hKey = NULL;
    TCHAR szSuffix[16];
    DWORD dwBufLen = 16;
    LONG lRet;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("software\\Gameinfinity\\Install game info.\\c34"), 0, KEY_QUERY_VALUE,
		&hKey) == ERROR_SUCCESS)
	{
		lRet = RegQueryValueEx(hKey,
							TEXT("identifier"),
							NULL,
							NULL,
							(LPBYTE)szSuffix,
							&dwBufLen);

		if (lRet == ERROR_SUCCESS)
		{
			pDlg->m_szCrocessSuffix = szSuffix;
		}
		else
		{
			pDlg->m_szCrocessSuffix = "";
		}

		RegCloseKey(hKey);
	}
	else
	{
		pDlg->m_szCrocessSuffix = "";
		return;
	}
}

int CAtumLauncherApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hMutexMonoInstance)
	{
		::CloseHandle(m_hMutexMonoInstance);
		m_hMutexMonoInstance = NULL;
	}
	
	return CWinApp::ExitInstance();
}

// 2012-10-21 by mspark, ���� ���� �� ���μ����� �����ִ� ACEonline.atm ����
void CAtumLauncherApp::RemainingProcessRemove()
{
	DWORD dwSize = 250;
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	BOOL bCrrent = FALSE;
	
	
	hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);
	
	pEntry.dwSize =sizeof(PROCESSENTRY32);
	// �������� ���μ������� ù���� ������ �����´�.
	Process32First (hSnapShot,&pEntry);
	
	// Tool�� ���������� Ȯ��
	int nCnt = 0;
	BOOL bRes = TRUE;

	for(nCnt = 0; nCnt < 1000; nCnt++)
	{		
		if(bRes==FALSE)
			break;        
		if (IsWindowsVersionOrGreater(6, 0, 0))
		{
			if (!strncmp(pEntry.szExeFile, CLIENT_EXEUTE_FILE_NAME, 15))
			{
				bCrrent = TRUE;
			}
		}
		else
		{
			if (!strncmp(pEntry.szExeFile, CLIENT_EXEUTE_FILE_NAME_XP, 17))
			{
				bCrrent = TRUE;
			}
		}
		
		if(bCrrent)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pEntry.th32ProcessID);
			if(hProcess)
			{
				if(TerminateProcess(hProcess, 0))
				{
					unsigned long nCode; //���μ��� ���� ����
					GetExitCodeProcess(hProcess, &nCode);
				}
				CloseHandle(hProcess);
			}
			break;
		} 
		// ������ ���μ����� ������ �����´�.
		bRes=Process32Next (hSnapShot,&pEntry);
	}
}
// end 2012-10-21 by mspark, ���� ���� �� ���μ����� �����ִ� ACEonline.atm ����