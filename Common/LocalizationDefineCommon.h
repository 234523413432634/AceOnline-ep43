// 2006-09-05 by cmkwon
#pragma once
#include "Contents.h"

///////////////////////////////////////////////////////////////////////////////
// 2015-05-17 Future, Service Type Definitions

///////////////////////////////////////////////////////////////////////////////
//  SERVICE_TYPE_ENGLISH_SERVER_1		==>	ĳ����	Wikigames		- Eng	2000
//  SERVICE_TYPE_VIETNAMESE_SERVER_1	==>	��Ʈ��	VTC-Intecom		- Viet	4000
//  SERVICE_TYPE_DREAMACE				==>	DreamAce
//  SERVICE_TYPE_ETERNALSKIES			==> Eternal Skies



#ifdef S_DA_SERVER_SETTING_HSSON
#define SERVICE_TYPE_DREAMACE
#define SERVICE_UID_FOR_WORLD_RANKING		15000			// 2015-05-18 Future, random value, wrk not used atm
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON
#define SERVICE_TYPE_ETERNAL_SKIES
#define SERVICE_UID_FOR_WORLD_RANKING		15000			// 2015-05-18 Future, random value, wrk not used atm
#endif

///////////////////////////////////////////////////////////////////////////////
// Code Page
#define CODE_PAGE							1252

///////////////////////////////////////////////////////////////////////////////
// Capacities & Max Level

#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define SIZE_MAX_INITIAL_GUILD_CAPACITY				30		// �ʱ� ��� ���� �� ���� ���� ��, // 2006-09-06 by cmkwon, �ѱ� ������ ���� (40-->30)
#define SIZE_MAX_GUILD_CAPACITY						300		// 2008-05-28 by dhjin, EP3 ���� ���� ���� - �ִ� ���� ��
#define SIZE_MAX_ITEM_GENERAL						111		// ĳ������ �κ��丮�� ������ �� �ִ� �������� �ִ� ����(1���� SPI �������� ī��Ʈ�̴�, Ŭ���̾�Ʈ�� 60�� ����Ѵ�.), // 2006-09-06 by cmkwon, �ѱ� ������ ����(61-->41)
#define SIZE_MAX_ITEM_GENERAL_IN_STORE				151		// â�� ������ �� �ִ� �������� �ִ� ����, // 2006-09-06 by cmkwon, �ѱ� ������ ����(101-->51)
#define CHARACTER_MAX_LEVEL							120
#define COUNT_IN_MEMBERSHIP_ADDED_INVENTORY			40
#define COUNT_IN_MEMBERSHIP_ADDED_STORE				50
#define COUNT_IN_MEMBERSHIP_ADDED_GUILD_CAPACITY	30
#endif

////////////////////////////////////////////////////
// Default Stats
#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define STAT_BGEAR_ATTACK_PART						3
#define STAT_BGEAR_DEFENSE_PART						3
#define STAT_BGEAR_FUEL_PART						3
#define STAT_BGEAR_SOUL_PART						3
#define STAT_BGEAR_SHIELD_PART						3
#define STAT_BGEAR_DODGE_PART						3

#define STAT_MGEAR_ATTACK_PART						2
#define STAT_MGEAR_DEFENSE_PART						4
#define STAT_MGEAR_FUEL_PART						3
#define STAT_MGEAR_SOUL_PART						4
#define STAT_MGEAR_SHIELD_PART						3
#define STAT_MGEAR_DODGE_PART						2

#define STAT_AGEAR_ATTACK_PART						4
#define STAT_AGEAR_DEFENSE_PART						3
#define STAT_AGEAR_FUEL_PART						3
#define STAT_AGEAR_SOUL_PART						3
#define STAT_AGEAR_SHIELD_PART						4
#define STAT_AGEAR_DODGE_PART						1

#define STAT_IGEAR_ATTACK_PART						4
#define STAT_IGEAR_DEFENSE_PART						2
#define STAT_IGEAR_FUEL_PART						3
#define STAT_IGEAR_SOUL_PART						3
#define STAT_IGEAR_SHIELD_PART						2
#define STAT_IGEAR_DODGE_PART						4
#endif

///////////////////////////////////////////////////////////////////////////////
// 2006-09-15 by cmkwon, 
// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 05

#if defined (S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define MSBILLING_DB_SERVER_IP						"127.0.0.1"
#endif


#define MSBILLING_DB_SERVER_PORT					9979

// Kor_Masang51		==> 1
// Kor_ETRI			==> 1
// Eng_Gala-Net		==> ��� ����
// Viet_VTC-Intecom	==> ��� ����
#define MSBILLING_GAMEUID							1

// 2006-09-22 by dhjin
// Kor_Masang51		==> 201			// 2006-10-23 by cmkwon, ����(101-->201)
// Kor_ETRI			==> 201			// 2006-10-23 by cmkwon, ����(101-->201)
// Eng_Gala-Net		==> 201			// 2006-10-23 by cmkwon, ����(101-->201)
// Viet_VTC-Intecom	==> 201			// 2006-10-23 by cmkwon, ����(101-->201)
#define COUNT_IN_MEMBERSHIP_GUILDSTORE				201		// 2006-09-22 by dhjin, ����� ���񽺽� ���� â�� ī��Ʈ


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
// Homepage Domain

#ifdef S_DA_SERVER_SETTING_HSSON
#define STRMSG_S_GAMEHOMEPAGE_DOMAIN			"http://vn.dreamace.org"
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON
#define STRMSG_S_GAMEHOMEPAGE_DOMAIN			"http://COMMING_SOON"		// 2015-05-18 Future, Eternal Sky Homepage Domain
#endif


///////////////////////////////////////////////////////////////////////////////
// 2006-05-22 by cmkwon, ��� MD5�� ���ڵ��ÿ� ���� ��� �տ� �߰��� ��Ʈ�� - 
#define MD5_PASSWORD_ADDITIONAL_STRING			""
#define EXT_AUTH_GAME_NAME						"SCO"		// 2006-05-22 by cmkwon

// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 07

#ifdef S_DA_SERVER_SETTING_HSSON	// Wikigames
// 2008-06-05 by cmkwon, AdminTool, Monitor ���� ���� IP�� server config file �� �����ϱ� - MS140�� ������ �׽�Ʈ�� ���� IP �� Ʋ����(121.134.11.) ���� ��
// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP1		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP2		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP3		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng ���� ���� ���� - ���� Private IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP4		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng ���� ���� ���� - ���� Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP5		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng ���� ���� ���� - �׼� Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP6		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP7		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP8		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP9		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP10		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define IS_SCADMINTOOL_CONNECTABLE_IP(ip)		( 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP1,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP1)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP2,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP2)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP3,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP3)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP4,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP4)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP5,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP5)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP6,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP6)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP7,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP7)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP8,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP8)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP9,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP9)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP10,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP10)) )

///////////////////////////////////////////////////////////////////////////////
// 2006-05-02 by cmkwon, Launcher URL
#define LAUNCHER_WEB_URL						"http://dreamace.org/launcher/"
#define TESTSERVER_LAUNCHER_WEB_URL				"http://dreamace.org/launcher/"


///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
#define BILLING_DBSERVER_DATABASE_NAME			"MS_Billing"
#define BILLING_DBSERVER_USER_ID				"atum"
// Kor_Masang51		==> callweb
// Kor_Yedang		==> 2006-12-25 by cmkwon, ������
#define BILLING_DBSERVER_USER_PWD				"callweb"
#endif // S_DA_SERVER_SETTING_HSSON

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Wikigames
// 2008-06-05 by cmkwon, AdminTool, Monitor ���� ���� IP�� server config file �� �����ϱ� - MS140�� ������ �׽�Ʈ�� ���� IP �� Ʋ����(121.134.11.) ���� ��
// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP1		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP2		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP3		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng ���� ���� ���� - ���� Private IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP4		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng ���� ���� ���� - ���� Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP5		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng ���� ���� ���� - �׼� Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP6		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP7		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP8		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP9		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP10		"127.0.0.1"		// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define IS_SCADMINTOOL_CONNECTABLE_IP(ip)		( 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP1,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP1)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP2,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP2)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP3,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP3)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP4,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP4)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP5,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP5)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP6,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP6)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP7,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP7)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP8,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP8)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP9,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP9)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP10,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP10)) )

///////////////////////////////////////////////////////////////////////////////
// 2006-05-02 by cmkwon, Launcher URL
#define LAUNCHER_WEB_URL						"http://COMMING_SOON/launcher/"
#define TESTSERVER_LAUNCHER_WEB_URL				"http://COMMING_SOON/launcher/"


///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
#define BILLING_DBSERVER_DATABASE_NAME			"MS_Billing"
#define BILLING_DBSERVER_USER_ID				"atum"
// Kor_Masang51		==> callweb
// Kor_Yedang		==> 2006-12-25 by cmkwon, ������
#define BILLING_DBSERVER_USER_PWD				"callweb"
#endif // S_ETRS_SERVER_SETTING_HSSON

///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 08

#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define	SG_BOX_FONT_FACENAME						"Tahoma"					// 2007-02-12 by cmkwon, �۾�ü
#define	SG_BOX_FONT_CHARSET							ANSI_CHARSET			// 2007-02-12 by cmkwon, ĳ���ͼ�
#define	SG_BOX_FONT_WEIGHT							FW_BOLD					// 2007-02-12 by cmkwon, �۾�ü �α�
#endif



// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 09

// Dream Ace
#if defined(S_DA_SERVER_SETTING_HSSON) && defined(S_ACCESS_INTERNAL_SERVER_HSSON)
#ifdef S_EP4_TEST_SERVER_HSKIM
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"121848507940481966557E4A1F"	 //	IP 61.39.170. 238 - // 2013-03-18 by hskim, EP4 ���޿� ���� �߰�
#else
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A14544F7F455E066F4869"	// IP 66.207.198.252 - ĳ���� ���� �׼�
//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A1C534F7F4752066D537E4A1D01"	// IP 127.0.0.1142 - ���� ���� ĳ���� �׼�
#endif

#define REGISTRY_BASE_PATH						"ACEonline"
#define EXE_1_FILE_NAME							"ACEonline.exe"
#define LAUNCHER_FILE_NAME						"Launcher.exe"

// Win XP Executable name. Used for Server only
#define CLIENT_EXEUTE_FILE_NAME_XP				"ACEonlin3.atm"

// Determination of Executable name by Client Configuration
#ifdef _WIN_XP
#define CLIENT_EXEUTE_FILE_NAME					CLIENT_EXEUTE_FILE_NAME_XP
#else
#define CLIENT_EXEUTE_FILE_NAME					"ACEonline.exe"
#endif

#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, ������

#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#endif // Dream Ace

// Eternal Skies
#if defined(S_ETRS_SERVER_SETTING_HSSON) && defined(S_ACCESS_INTERNAL_SERVER_HSSON)
#ifdef S_EP4_TEST_SERVER_HSKIM
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"121848507940481966557E4A1F"	 //	IP 61.39.170. 238 - // 2013-03-18 by hskim, EP4 ���޿� ���� �߰�
#else
//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A14544F7F455E066F4869"		// IP 66.207.198.252 - ĳ���� ���� �׼�
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR "1A14544F7F455E066E48614A1E"		//192.168.6.132
//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A1C534F7F4752066D537E4A1D01"	// IP 127.0.0.1142 - ���� ���� ĳ���� �׼�
//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR "191848507F4348196E527E4A100A"  // IP 25.110.104.199 - St0rmy hamachi IP
#endif

#define REGISTRY_BASE_PATH						"EternalSkies"
#define EXE_1_FILE_NAME							"ACEonline.exe"
#define LAUNCHER_FILE_NAME						"Launcher.atm"

// Win XP Executable name. Used for Server only
#define CLIENT_EXEUTE_FILE_NAME_XP				"ACEonlin3.atm"

// Determination of Executable name by Client Configuration
#ifdef _WIN_XP
#define CLIENT_EXEUTE_FILE_NAME					CLIENT_EXEUTE_FILE_NAME_XP
#else
#define CLIENT_EXEUTE_FILE_NAME					"ACEonline.exe"
#endif

#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, ������

#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#endif // Eternal Skies


// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 10

#ifdef S_DA_SERVER_SETTING_HSSON	// Dream Ace
#define STRMSG_WINDOW_TEXT							"ACEonline"
#define STRMSG_REG_STRING_CLIENT_VERSION			"ACEonlineVersion"
#define STRMSG_REG_STRING_REGISTRYKEY_NAME			"killburne"				// 2008-07-31 by cmkwon, Yedang-Global_Eng �� Wikigames_Eng �� ���� �� - 
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Eternal Skies
#define STRMSG_WINDOW_TEXT							"EternalSkies"
#define STRMSG_REG_STRING_CLIENT_VERSION			"EternalSkiesVersion"
#define STRMSG_REG_STRING_REGISTRYKEY_NAME			"EternalSkies"				// 2008-07-31 by cmkwon, Yedang-Global_Eng �� Wikigames_Eng �� ���� �� - 
#endif


///////////////////////////////////////////////////////////////////////////////
// 2007-06-27 by cmkwon, �߱� ����� �ý��� ���� - �̼����� ����
// Kor		- �� 20��
// China	- �� 18��
#define ADULT_YEARS									20			// 2007-06-29 by cmkwon,

///////////////////////////////////////////////////////////////////////////////
// 2007-07-06 by cmkwon, SCAdminTool���� OnlyServerAdmin���� ���� - ���� ����
#define SCADMINTOOL_ONLY_SERVER_ADMIN_ACCOUNT_NAME		"SC_moniter"
#define SCADMINTOOL_ONLY_SERVER_ADMIN_PASSWORD			"cowboyWkd"

///////////////////////////////////////////////////////////////////////////////
// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - ���� URL, ���� �ϴ� URL
#define EXE1_URL_1										"http://notice.aceonline.com.cn/ace2.htm"
#define EXE1_URL_2										"http://notice.aceonline.com.cn/ace1.htm"


///////////////////////////////////////////////////////////////////////////////
// 2008-12-19 by cmkwon, �ѱ� Yedang �ٽ��� ����͸� ���� ���� �߰� - IP�� ""�� �����Ǹ� ����͸� ������ ������� �ʴ� ����, ����� Masang140�� Yedang�� ��� �� ����
// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 11

#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define GAME_GUARD_MONITORING_SERVER_IP					"127.0.0.1"
#endif



///////////////////////////////////////////////////////////////////////////////
// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - ���巩ŷ DB ���� ����
// 2009-06-01 by cmkwon, ���� ��ŷ �ý��� �׽�Ʈ ��� ����(for �׼�) - 
// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 12

#ifdef S_DA_SERVER_SETTING_HSSON	// Dream Ace
#define WRK_DBSERVER_IP							"127.0.0.1"				// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define WRK_DBSERVER_PORT						9911
#define WRK_DBSERVER_DATABASE_NAME				"atum2_db_1"
#define WRK_DBSERVER_ID							"atum"
#define WRK_DBSERVER_PWD						"efinos76/*"
#define WRK_DBSERVER_IP_FOR_TEST_SERVER			"127.0.0.1"				// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)// 2009-06-01 by cmkwon, ���� ��ŷ �ý��� �׽�Ʈ ��� ����(for �׼�) - 
#define WRK_DBSERVER_PORT_FOR_TEST_SERVER		1433					// 2009-06-01 by cmkwon, ���� ��ŷ �ý��� �׽�Ʈ ��� ����(for �׼�) - 
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Eternal Skies
#define WRK_DBSERVER_IP							"127.0.0.1"				// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)
#define WRK_DBSERVER_PORT						9911
#define WRK_DBSERVER_DATABASE_NAME				"atum2_db_1"
#define WRK_DBSERVER_ID							"atum"
#define WRK_DBSERVER_PWD						"callweb"
#define WRK_DBSERVER_IP_FOR_TEST_SERVER			"127.0.0.1"				// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(127.0.0.1)// 2009-06-01 by cmkwon, ���� ��ŷ �ý��� �׽�Ʈ ��� ����(for �׼�) - 
#define WRK_DBSERVER_PORT_FOR_TEST_SERVER		1433					// 2009-06-01 by cmkwon, ���� ��ŷ �ý��� �׽�Ʈ ��� ����(for �׼�) - 
#endif

///////////////////////////////////////////////////////////////////////////////
// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
#define MAX_INFLUENCE_PERCENT			53		// �ִ� 5% ���̱����� ���� ������ �����Ѵ�.


///////////////////////////////////////////////////////////////////////////////
// 2009-05-12 by cmkwon, (�Ϻ���û) �Ϻ��� ���� ������ �ֱ� 7�Ϸ� ���� - ���������� �ֱ� ���񽺺��� �ٸ��� ����
// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 13
#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define	OUTPOST_NEXTWARGAP				7
#endif


///////////////////////////////////////////////////////////////////////////////
// 2009-07-08 by cmkwon, ���� ���� ���� ��ġ �̵�(LocalizationDefineCommon.h) - 
// ���� �ҽ����� �� �߰� �Ǿ�� �� �κ� ���� 14

#ifdef S_DA_SERVER_SETTING_HSSON	// Dream Ace
#define	OUTPOST_WARTIME					60			// ���������� ���� �ð�			==> ���� 10��, �������� 120��
#define OUTPOST_WARTIME_FOR_TESTSERVER	60			// �׼��� ���������� ���� �ð�	==> ���� 10��, �������� 60��
#define PAY_MINIMUN_COUNT				10			// ����,����������,������ ���� ������ ���� �ּ� �ο��� ==> ���� 1��, �������� 10��
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Eternal Skies
#define	OUTPOST_WARTIME					60			// ���������� ���� �ð�			==> ���� 10��, �������� 120��
#define OUTPOST_WARTIME_FOR_TESTSERVER	60			// �׼��� ���������� ���� �ð�	==> ���� 10��, �������� 60��
#define PAY_MINIMUN_COUNT				10			// ����,����������,������ ���� ������ ���� �ּ� �ο��� ==> ���� 1��, �������� 10��
#endif

///////////////////////////////////////////////////////////////////////////////
// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
#define SIZE_MAX_ADDABLE_INVENTORY_COUNT		50		// �⺻�� �����̾��� ������ �߰��� ������ �ִ� �κ� �߰� ����
#define SIZE_MAX_ADDABLE_STORE_COUNT			50		// �⺻�� �����̾��� ������ �߰��� ������ �ִ� â�� �߰� ����


////////////////////////////////////////////////////////////////////////////////
// ������ ���� ������ ������. by hsLee. 
#define __CONTENTS_SHOW_INFINITY_DIFFICULTY_EDIT_WND__		// ���Ǵ�Ƽ ���̵� ���� UI ���̱�.	2010. 07. 27. by hsLee.
////////////////////////////////////////////////////////////////////////////////