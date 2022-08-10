#pragma once

//----------------------------------------------------------------------------------------------------------//
// 2015-05-17 Future, New Configuration Style                                                               //
//----------------------------------------------------------------------------------------------------------//
// _DEBUG
// WIKIGAMES_ENG
// ETERNAL_SKIES
// DREAM_ACE
//

#ifdef _DEBUG
	#define CUSTOM_OPTIMIZER_HSSON					// ini ���� �о ���ӿ� ����
	#define MULTI_LOADER_HSSON						// Ŭ���̾�Ʈ�� ������ ��� �� �ֵ��� ����
	#define GAMEGUARD_NOT_EXECUTE_HSSON				// ���� ���� ���� ����
	#define _ATUM_DEVELOP							// ������) �����ҽ��� ������� �� Ȱ��ȭ ����� ��
	//#define _WARRIOR_ANTICHEAT
	//#define _WARRIORSW_PROCESS_LIST_SENDER //2015-04-01 implement to send process list for dream
#endif // _DEBUG

#define S_ARARIO_HSSON							// �ƶ󸮿� ä�θ�

#if defined(_JPN)
#define LANGUAGE_JAPAN								// ??? ??? ???? ???
#include "Str_JPN/StringDefineCommon.h"
#include "Str_JPN/StringDefineServer.h"
#include "Str_JPN/StringDefineClient.h"
#endif

#if defined(_KOR)
#define YEDANG_RELEASE								// ???, ??? ?? ??? ??
#define KOR_INPUT_LANGUAGE_HSSON					// ?? ?? ??
#define KOR_HANGEUL_START_HSSON						// ?? ??? ??
#define KOR_CASHSHOP_INTERFACE_HSSON				// ??? ?????
#define KOR_GAME_RATINGS_HSSON						// ?? ??? ??? ??? ??
#define KOR_CHARACTER_INTERFACE_POS_HSSON			// ??? ?? ?? ??
#include "Str_KOR/StringDefineCommon.h"
#include "Str_KOR/StringDefineServer.h"
#include "Str_KOR/StringDefineClient.h"
#endif

#if defined(_VIE)
#include "Str_VIE/StringDefineCommon.h"
#include "Str_VIE/StringDefineServer.h"
#include "Str_VIE/StringDefineClient.h"
#endif

#if defined(_ENG)
#define S_DA_SERVER_SETTING_HSSON				// 2015-05-18 Future, Dream Ace Configuration
#include "Str_DA/StringDefineCommon.h"
#include "Str_DA/StringDefineServer.h"
#include "Str_DA/StringDefineClient.h"
#endif

#if defined(_RUS)
#include "Str_RUS/StringDefineCommon.h"
#include "Str_RUS/StringDefineServer.h"
#include "Str_RUS/StringDefineClient.h"
#endif

#if defined(_ETRS)
#define S_ETRS_SERVER_SETTING_HSSON				// 2015-05-16 Future, Eternal Skies Configuration
#include "Str_ETRS/StringDefineCommon.h"
#include "Str_ETRS/StringDefineServer.h"
#include "Str_ETRS/StringDefineClient.h"
#endif

//----------------------------------------------------------------------------------------------------------//
// �������� ����ȭ�� ������ ������ ����(����) �Ǵ� �䱸(Ư��)�� ���� ��ȭ �� �۾�                         //
//----------------------------------------------------------------------------------------------------------//

//#define S_EP4_TEST_SERVER_HSKIM				// EP4 ���޿� �׽�Ʈ ����
//#define SC_DARK_CRASH_HSSON
//#define SC_DARK_CRASH_FILE_OUTPUT_HSSON
//#define S_GLOG_HSSON
//#define S_AUTO_UPDATE_VERSION_BY_PRESVR_HSSON
//#define S_ARENA_NOT_INFO						// �Ʒ��������� �� �� ���� ��
//#define S_MANAGER_ADMIN_HSSON					// ���� �����͵� ���� ��/��� ��ų �� �ֵ��� ����
// 2013-07-08 �������� �߰�
//#define S_HACK_SHIELD_AUTO_UPDATE_HSKIM		// �ٽ��� �ڵ� ������Ʈ ���
//#define S_ADMIN_CHARACTER_LOGIN_IP_CHECK		// 2012-06-07 by hskim, ������ ���� ���� ��ȭ - Ư�������� ��ϵ� �����ǰ� �ƴϸ� ���� ����
//#define C_OBJECT_RESOURCE_ORGANIZE_MSPARK		// 2012-04-18 by mspark, ������Ʈ ���ҽ� ����
//#define C_INTERFACE_RESOURCE_ORGANIZE_ISSHIN	// 2012-04-23 by isshin, �������̽� ���ҽ� ����				   
//#define C_USER_CTRL_KEY_LOCK
//#define C_USER_COPYPAST_JHSHN					// not exist in client and in server
//#define C_USER_EFFECT_CONTROL
//#define S_GAMEFORGE_KEY_CHANGE_JHSEOL
//#define S_SELECTIVE_SHUTDOWN_HSKIM
//#define S_BPSOFTBILLING_JHSEOL
//#define S_FULL_LOG_JHSEOL
//#define S_DISABLE_CHECKSUM_RESOURCE
//#define S_OPTION_PARTNER__JHAHN
// ep4-2��
//#define S_ARENA_EX_GAMEFORGE_VERSION_JHSEOL	// 2012-09-21 by jhseol, �Ʒ��� �߰����� GF���� - �Ʒ��� ���� ĳ���͸� ����ϵ� �⺻.���.�ƸӸ� �����ϰ� ��� ���� �ɸ����� ��� ���������� ����.
//#define SC_SHUT_DOWNMIN_JHAHN
//#define S_NPLAY_PCBANG_MODULE_JHSEOL
//#define S_ITEM_VIEW_UNCONDITIONAL_ON			// 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
//#define C_ITEM_VIEW_UNCONDITIONAL_ON			// 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
//#define S_TEMP_PRESERVER_MAKE_HSKIM			// PreServer ����(������ ����) �ÿ� �ӽ� PreServer ���ۿ� ��ũ��
//#define TEMP_PRESERVER_MAKE_IP				"182.162.137.2"		// ���� ��� ������
//#define S_GLOG_3ND_KHK
//#define S_KOR_BUSAN_MONITORTOOL_MAKE
//#define S_DELETE_BUY_CASH_GIVE				// 2012-11-12 by hskim, ĳ���� �����ϱ� ��� ����
//#define C_CASHSHOP_IMAGE_REMOVE				// 2012-11-16 by mspark, ĳ���� ����, ���� ��� ����
//#define S_KOR_TEST_GLOG_CHARDATA
//#define C_WEB_CASHSHOP
//#define S_WEB_CASHSHOP_JHSEOL
//#define S_MONTHL_CASH_BUY_GIFT_JHSEOL			// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
//#define S_ITEM_EVENT_SUB_TYPE_JHSEOL			// 2013-03-29 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰������� ����
//#define S_ITEM_EVENT_RETURN_USER_BCKIM		// 2013-02-28 by bckim, �������� �����߰�
//#define C_ITEM_EVENT_RETURN_USER_BHSOHN		// 2013-03-06 by bhsohn ���� ���� �ý���
//#define C_FALL_OF_BOOSTER_USE_STOP_MSPARK
//#define SC_SECURITY_COMMAND_HSKIM				// ���Ű ���� (Ŭ�� ����) [�������� (���� ���� ����)]
//#define C_SECURITY_COMMAND_JHAHN				// ���Ű Ŭ���̾�Ʈ Ȯ�� - ���� �ּ� Ǯ�� ������
//#define _ADVANCED_CONFIG
///////////////////////////////////////////////////////////////////
// ������ �ݿ� ������
#define MULTI_LOADER_HSSON						// allow multiple client instances
#define GUILD_WAREHOUSE_ACCESS_HSSON			// ������� �ƴϴ��� ����â�� �̿밡��
#define NEW_CASHSHOP_INTERFACE_HSSON			// �ű� ĳ���� �۾�
#define KOR_CASHSHOP_REFILL_WEB_HSSON			// ĳ���� ���� �������� �߰�
#define WAREHOUSE_SHARES_HSSON					// ĳ���� ���� â�� ���� �� �� ����
#define SC_GROWING_PARTNER_HSKIM_JHAHN			// ĳ���� ���� â���� ��Ʈ��UI Ȱ��ȭ
#define S_INFINITY3_HSKIM						// ���Ǵ�Ƽ 3�� ���� (����)
#define INFI_QUEST_JSKIM					    // ���Ǵ�Ƽ ����Ʈ
#define MULTI_TARGET_JSKIM						// ��ƼŸ��
#define S_AUTHENTICATION_SERVER_HSKIM			// ���� ���� ���� (����)
#define S_LOGIN_ITEM_EVENT_SHCHO			    // �α��ν� ������ ����
#define S_EP4_HSKIM								// EP4 1�� (����)
#define C_EPSODE4_UI_CHANGE_JSKIM				// 2011. 11. 17 by jskim EP4 UI ����
#define C_EPSODE4_SETTING_JHAHN					// 2011. 11. 17 by jhahn EP4 Ʈ���� �ý��� 
#define S_AUTHENTICATION_SERVER_2ND_HSKIM		// ���� ���� 2�� ���� (����) - ���� ���� �Ұ��� ���� ���� �Ұ�
#define S_STATISTICS_HSKIM						// ȭ�� ���
#define S_DELETE_DROP_ITEM_HSKIM				// ����� ������ 5�еڿ� ����
#define S_ARENA_EX_1ST_JHSEOL					// 2012-05-29 by jhseol, �Ʒ��� �߰����� - ���߰�, �ڵ���Ƽ, ����� ��ȭ, ������� ��ȭ, ����Ƚ�� ����, ti_arenainfo �ʵ��߰�
#define C_ARENA_EX_1ST_MSPARK					// 2012-05-29 by mspark, �Ʒ��� �߰����� - �� �̹��� �߰�, �̹��� ��ǥ ����, ���� �߰�
#define C_UI_REPAIR_ISSHIN						// 2012-06-21 by isshin UI���� ���� - Ep4�� ����
#define S_ENEMY_INFO_JHSEOL						// 2012-07-19 by jhseol, ���۹̸�����
#define C_ENEMY_INFO_ISSHIN						// 2012-06-14 by isshin ���۹̸�����
#define S_TIMEITEM_LOG_HIDE_JHSEOL				// 2012-08-28 by jhseol, �ð��� ������ ���� �α� ���� ��û
#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL		// 2013-02-26 by jhseol, ���� �ý��� ������ �ΰ��ɼ� - ������ �ִ� ������ ���� ������ ����
//#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUMMON_TIME_VARIANCE_JHSEOL		// 2013-03-18 by jhseol, ���� �����ð� ���� - ���� �����ð��� ������ ���۽ð��� ������ ������ �����Ͽ� �յ��ϰ� �л�ǵ��� ��.
#define S_SEARCHEYE_SHUTTLE_POSITION_JHSEOL		// 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
#define C_SEARCHEYE_SHUTTLE_POSITION_MSPARK		// 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
#define S_KOR_TEST_MSWAR_BACK_TO_CITY_HSKIM		// 2012-12-05 by hskim, ���� ���۽ÿ� ��� ���� ���ø����� ���� - �ѱ� ���� �׽�Ʈ �� ������ �ݿ��� ����
//#define _EXTENDED_CHATCOLOR
//#define C_GAME_SINGLE_THREAD_YMJOO
#define _STAFF_WARP_PERMISSIONS
#define _SHOW_INFLUENCE_ON_GMNATION
#define _SHOW_SPEAKER_IN_CHAT
#define _CROSS_NATION_CHAT						// Cross Nation Chat
#define _CROSS_NATION_CHAT_SPI_COST	100000
#define _REWORKED_COLORSHOP
#define _NCP_BASED_MSWAR
#define _ARENA_WP_HH
//#define _DBG_INFO
//#define _SHOW_LATENCY
#define _OUTPOST_BUFFS							//gives special buffs to outpost winners
#define _ARENA_ADDITIONAL_SUPPLY_ITEMS
#define S_MINI_DUMP_HSKIM
#define S_ACCESS_INTERNAL_SERVER_HSSON
#define	S_TRIGGER_OUTPOST_JHSEOL				// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
//#define	S_SKILL_NUMBERING_RENEWAL_JHSEOL	// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ���� - ������
//#define	C_SKILL_NUMBERING_RENEWAL_BHSOHN	// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ���� - Ŭ���
//#define SC_SHUT_DOWNMIN_SHCHO
//#define S_GM_COMMAND_USE_SHCHO
#define S_TRIGGER_SYSTEM_EX						// 2013-07-08 by jhseol, Ʈ���� �ý��� Ȯ�� - ���� Ʈ���ſ� �������� Ʈ������ ����� ����� �߰�
#define S_LAUNCHER_USE_ID_PASSWORD_HSKIM
#define C_USER_SCREENSHOTMODE_ISSHIN			//���� ��ũ���� ��� ����
#define S_MS_WAR_FIN_SUPPLY_ITEM_ADD
#define CHECK_SUM_ON
#define C_CANADA_HACKSHEILD_JHAHN				// 2012-09-17 by jhahn	ĳ���� �ٽ��� ���� ����
#define S_SERVER_CRASH_MSGBOX_REMOVE_MSPARK		// 2012-10-18 by mspark, �������� ���� �浹 �� �޽��� �ڽ� ������ �ʴ� �۾�
#define S_DISABLE_PREPARE_SHUTDOWN				// 2012-11-21 by hskim, PrepareShutdown ��� ����
#define C_ADMINISTRATOR_COPYPASTE_PERMISSION	// 2012-11-21 by mspark, ������ ����, �ٿ��ֱ� ���
#define S_AUTO_BLOCK_SYSTEM_HSKIM				// 2012-12-14 by hskim, �޸��� �ڵ� �� ��� ����
#define S_ADDITIONAL_EXPERIENCE					// 2012-11-12 by hskim, �޽� ����ġ
#define C_WARNING_APPEARANCE_CHANGE				// 2012-10-30 by mspark, ���� ���� ��� �޽��� �߰�
#define C_ABUSE_FILTER_RELAX					// 2013-04-03 by ssjung ĳ���� �弳 ���� ��ȭ
#define S_DB_FULL_LOG_JHSEOL					// 2013-01-04 by jhseol, DB Ǯ�α� �����
#define S_REMANING_PROCESS_REMOVE_MSPARK
#define S_DISABLE_ALONE_RUN_MODE_HSKIM			// 2013-06-12 by hskim, �ϳ��� OS ���� ���� ���� ����
#define S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM	// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
#define SC_PARTNER_SHAPE_CHANGE_HSKIM			// 2012-12-03 by hskim, ��Ʈ�� �Ⱓ�� ���� ����
#define GAMEFORGE4D_CHAT_MACRO_OUTPUT_TIME		// 2013-07-30 by ssjung ���� ���� �� ��� ��ũ�� ������ 15�� ���� 5������ ����

#ifdef _ATUM_CLIENT
	#define C_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL	// 2012-11-22 by jhseol, ���� �ý��� ������ - ����������(OutPost)�� ���� ���� ����
	#define _WIREFRAME
	#define _NOCLIP
	#define _DRAW_EVENTS
	#define _CRASH_HANDLER
	#define _ENHANCED_EXCEPTIONS
	#define _HIDE_PERCENTAGE_ON_MAXLEVEL
	#define _DISABLE_BSTOP_AUTOHORIZONT
	#define _ENHANCED_MIXING_DISPLAY
	#define _SHOW_GUILD_NAME
	#define _SHOW_LEADER_INFO					//Need _SHOW_GUILD_NAME defined to work
	//#define _WARRIOR_CLASS_KILL_BAR			//additional bar with showing current class and percent for next class
	#define _WARRIOR_SHOW_SPHP_4ALL
	#define _WARRIOR_ADDITIONAL_INFO			//show additional info near aim (hp and ammo status)
	#define S_SERVER_CRASH_FILENAME_ADD_INFO	// 2013-05-28 by hskim, �̴� ���� ���� �̸��� �ð� �߰�
	//#define CUSTOM_OPTIMIZER_HSSON
	//#define C_CLIENT_FILE_LOG
	//#define _ADVANCED_CONFIG
	#define _INSTANT_LAUNCH
	#define _NO_FADE
#endif

#ifdef _ATUM_SERVER
	#define _AUTO_DGA
	#define _NATION_BUFFS
#endif

#if defined(_REWORKED_COLORSHOP)
#define DEFAULT_COLOR_PRICE	50000				// 2015-06-25 by St0rmy, in case that the coloritem is missing in SSQL Table, the user won't get DC'd when SHOP_PRICES_PER_BUILDING_NPC is defined
#define COLOR_RESET_ITEMNUM 8000000				//This item resets the colorcode to 0 when bought in colorshop (needs to have kind 60)
#endif

#if defined(DREAM_ACE) || defined(ETERNAL_SKIES)
#define S_BONUS_KILL_SYSTEM_RENEWAL				// 2015-06-11 by Future, Bonus System by Kills
#else
#define S_BONUSEXPSYSTEM_RENEWAL				// Counters by Mob Kills (3600 & 300)
#endif

#if defined(DEACTICATION_ALL)					// ��籹�� ������
#define S_ARENA_EX_1ST_RESTORE_JHSEOL			// 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#define C_ARENA_EX_1ST_RESTORE_MSPARK			// 2012-09-14 by mspark, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#endif

#ifdef INTECOM_VIET
#define C_CLIENT_LIVE_TIME						// 2012-11-07 by bhsohn ��Ʈ�� ��Ŷ ó��
#define C_ARENA_PASSWORD_ALPHABET				// 2012-12-10 by jhjang �Ʒ��� ��ȣ ������ �����ϰ� �Է� ���� 
#define S_B_GEAR_DAMAGE_PROBABILITY_INFO		// 2013-05-06 by bckim, B��� Ÿ�� ������ Ȯ��

#endif

#ifndef INTECOM_VIET
#define C_MOUSEFOCUS_BACKGROUND_NOTRESET		// 2012-12-17 by jhjang ������ ��׶����϶�, ���콺 ��Ŀ���� �ʱ�ȭ ���� �ʵ��� ����
#define C_JOIN_CHATROOM_MESSAGE					// 2013-01-08 by mspark, ä�ù濡 ���ο� ������ �������� ��� �ý��� �޽��� �߰�
#define C_LONGTIME_WINDOW_ON					// 2012-12-06 by bhsohn ��Ⱓ window Ų��, �������ӽ� ���� ���߾� �ִ� ���� ó��
#define C_BAZZER_NOT_EXIT						// 2013-01-08 by jhjang ���λ����� �� ���¿��� Ŭ���̾�Ʈ ���������� ���� ���� ���� �߰�
#define C_AGEAR_REFINERYSHOP_CLOSE_MSPARK		// 2013-02-04 by mspark, A��� ���޻����� ���� �� �̵� ��ư�� �������, ���޻��� â�� �������� �۾�
#define S_BUGFIX_DUPLICATE_ITEM_HSKIM			// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����) - ���� �ֱ⸦ 1�ð� (3600000) ���� 26.5�ð����� ���� (90000000)
#define C_INGAME_MIX_ITEM						// 2013-02-20 by bhsohn �ΰ��� ����â ó��
#define S_ADMINTOOL_IP_SEARCH_JHSEOL			// 2013-06-28 by jhseol, ĳ���� ������ IP��ȸ ����
#define C_SHOP_ITEMCNT_INIT_BHSOHN				// 2013-06-20 by bhsohn �������� ������ ������ ���� �ʱ�ȭ �ڵ� �߰�
#define C_DECA_TOOLTIP	
#define S_CANNOT_DO_GIFT_UNDER_LEVEL_BCKIM		// 2013-07-08 by bckim, ĳ���ٿ�û ����70 �̸��� �����ϱ� �Ұ�
#define C_FOCUS_MISS_CLIENT_NO_CLOSE			// 2013-06-28 by ssjung ��Ŀ���� �Ҿ��� �� 1���� Ŭ���̾�Ʈ�� ����Ǵ� ���� ĳ���� ����ó��
#define C_LEAST_LV_GIFT							// 2013-07-09 by bhsohn 70 ���� �̻� �����ϱ� �����ϰ� ����
#define SC_COLLECTION_ARMOR_JHSEOL_BCKIM		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ������ ����
#define C_LABORATORY_UI_CHANGE_JWLEE

#if defined(TEST140) || defined(YEDANG_KOR) || defined(WIKIGAMES_ENG)
// 2013-06-18 �ѱ�����
#define S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - ������ ����
#define C_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM
#endif

#endif

//	#endif
//#define S_WEB_CASHSHOP_SERVER_MODULE_HSKIM		// 2013-03-13 by hskim, �� ĳ�� ����
//#define S_DIRECT_DB_ITEM_INSERT_SECURITY_HSKIM	// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
// 2013-06-10 �ѱ� �߰�
//#define S_UPGRADE_INFLUENCE_POINT_HSKIM		// 2013-05-09 by hskim, ���� ����Ʈ ����
//#define C_UPGRADE_INFLUENCE_POINT_BHSOHN		// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
// 2013-07-09 ĳ����, �Ƹ���, �������� �߰�		// 2013-07-11 �Ϻ� �߰� 
//#define S_ADMINTOOL_CASHSHOP_REALIGNMENT_BCKIM	// 2013-02-05 by bckim, ĳ�ü� ��õ�� ���ļ����ο�
//#define C_ITEM_EVENT_NOTIFY_MSG_CHANGE_JHSEOL		// 2013-04-08 by jhseol, �̺�Ʈ ������ ���� �˸���� ����
// 2013-04-19 �ѱ�����
// 2013-05-23 �ѱ�����
//#define SC_BUFF_PENALTY_JHSEOL_BCKIM			// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
// 2013-06-27 �ѱ�����
//#define S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL		// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
//#define C_SERVER_DOWN_ALARM
//#define S_MULTI_RESOLUTION_JHSEOL 			// 2013-07-30 by jhseol, ��Ƽ�ػ� ����


/////////////////////////////////////////////////////////////
// 2015-05-25 Future, new Shop System (Prices per shop, not per Item)
#if defined(ETERNAL_SKIES)|| defined(DREAM_ACE) // add your servers here
	#define SHOP_PRICES_PER_BUILDING_NPC
	#define SCREENSHOT_FILE_FORMAT_JPG			// 2015-06-21 Future, ScreenShots as JPG
#endif

// Kill Rewards
#define _WP_REWARD_PER_KILL
#define WP_REWARD_KILL_MIN_COUNT	50
#define WP_REWARD_KILL_MAX_COUNT	100

#define _SPI_REWARD_PER_KILL
#define SPI_REWARD_KILL_MIN_COUNT	500000
#define SPI_REWARD_KILL_MAX_COUNT	1000000

#define _EXTENDED_KILL_MESSAGE	// 2015-07-22 Future, extended message like: Future was shot down. Reward ... SPI ... WP

////////////////////////////////////////////////////////////
// 2015-09-25 Future, Inactivity Checks
//#define C_DISCONNECT_INACTIVE_PLAYERS
//#define DISCONNECT_INACTIVE_PLAYERS_AFTER_SECONDS	1800	// 30 mins * 60 seconds
//#define DISCONNECT_INACTIVE_PLAYERS_WARNING_TIME	300		// 5 mins * 60 seconds
//#define EP1_CHARACTER