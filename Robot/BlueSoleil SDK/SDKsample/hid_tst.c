/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2009 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/
 
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    hid_tst.c
Abstract:
    this module is to test HID profiles relative functionality. 
Revision History:
	2009-11-26   Chu Enlai  Created

---------------------------------------------------------------------------*/

#include "sdk_tst.h"
#include "profiles_tst.h"
#include "Btsdk_Stru.h"
#include "WTypes.h"
#include "SetupAPI.h"
#include "devguid.h"
#include "shlwapi.h"

#include "LMaccess.h"
#include "hidusage.h"
#include "hidpi.h"
#include "hidsdi.h"

static BTCONNHDL s_currHidConnHdl = BTSDK_INVALID_HANDLE;
static BTDEVHDL s_currRmtHidDevHdl = BTSDK_INVALID_HANDLE;
static HANDLE	s_HIDDrvDev = INVALID_HANDLE_VALUE;
OVERLAPPED     s_HID_overlapped = {0};
static HANDLE s_hTheadListen = NULL;
static BOOL   s_bIsListen = FALSE;
static HIDP_CAPS  s_HID_CAP = {0};
void TestAddOneRemoteHIDDevice();
void TestDeleteOneRemoteHIDDevice(BTDEVHDL dev_hdl);
void TestConnectTheRemoteHIDDevice(BTDEVHDL dev_hdl);
void RestoreLinkKeyForDevice(BTDEVHDL dev_hdl, char *linkkeyStr);
void StartListenDataFromRemoteHID(BTDEVHDL dev_hdl);
void StopListenDataFromRemoteHID();

#ifdef _WIN32
#pragma comment(lib, "hid.lib")
#endif
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is to select device's handle.
Arguments:
Return:
	void 
---------------------------------------------------------------------------*/
void TestSelectRmtHidDev()
{   
    s_currRmtHidDevHdl = SelectRemoteDevice(0);
	if (BTSDK_INVALID_HANDLE == s_currRmtHidDevHdl)
	{
		printf("Please make sure that the expected device \
				is in discoverable state and search again.\n");
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is to connect HID device.
Arguments:
Return:
	void 
---------------------------------------------------------------------------*/
void TestConnectHidSvc()
{
	ULONG ulSvcHdlCnt = 0;
	BTUINT32 ulRet = BTSDK_FALSE;
	BTSDKHANDLE hEnum = BTSDK_INVALID_HANDLE;
	BTSVCHDL hSvcHdl = BTSDK_INVALID_HANDLE;

	BtSdk_SDAP_PNPINFO PnpInfo = {0};
	BtSdkRemoteServiceAttrStru struRmtSvcAttr = {0};

	Btsdk_BrowseRemoteServices(s_currRmtHidDevHdl, NULL, &ulSvcHdlCnt);	

	hEnum = Btsdk_StartEnumRemoteService(s_currRmtHidDevHdl);
	if (BTSDK_INVALID_HANDLE == hEnum)
	{
		return;
	}

	struRmtSvcAttr.mask = BTSDK_RSAM_EXTATTRIBUTES;

	while (BTSDK_INVALID_HANDLE != (hSvcHdl = Btsdk_EnumRemoteService(hEnum, &struRmtSvcAttr)))
	{
		if (NULL != struRmtSvcAttr.ext_attributes)
		{
			if (BTSDK_CLS_PNP_INFO == struRmtSvcAttr.svc_class)
			{			
				PnpInfo.size = sizeof(BtSdk_SDAP_PNPINFO);
				PnpInfo.product_id = ((BtSdkRmtDISvcExtAttrStru*)struRmtSvcAttr.ext_attributes)->product_id;
				PnpInfo.mask = ((BtSdkRmtDISvcExtAttrStru*)struRmtSvcAttr.ext_attributes)->mask;
				PnpInfo.spec_id = ((BtSdkRmtDISvcExtAttrStru*)struRmtSvcAttr.ext_attributes)->spec_id;
				PnpInfo.svc_hdl = 0;
				PnpInfo.vendor_id = ((BtSdkRmtDISvcExtAttrStru*)struRmtSvcAttr.ext_attributes)->vendor_id;
				PnpInfo.vendor_id_src = ((BtSdkRmtDISvcExtAttrStru*)struRmtSvcAttr.ext_attributes)->vendor_id_source;
				PnpInfo.version_value = ((BtSdkRmtDISvcExtAttrStru*)struRmtSvcAttr.ext_attributes)->version;
				Btsdk_FreeMemory(struRmtSvcAttr.ext_attributes);
				struRmtSvcAttr.ext_attributes = NULL;
				break;
			}
			else
			{
				Btsdk_FreeMemory(struRmtSvcAttr.ext_attributes);
				struRmtSvcAttr.ext_attributes = NULL;
			}
		}
	}
	
	Btsdk_EndEnumRemoteService(hEnum);
	hEnum = BTSDK_INVALID_HANDLE;
	
	ulRet = Btsdk_ConnectEx(s_currRmtHidDevHdl, BTSDK_CLS_HID, (BTUINT32)&PnpInfo, &s_currHidConnHdl);
	
	if (BTSDK_OK != ulRet)
	{
		printf("Please make sure that the expected device is powered on and connectable.\n");
		return;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is to show HID Services user interface.
Arguments:
    void
Return:
	void 
---------------------------------------------------------------------------*/
void HidShowMenu()
{
	printf("*****************************************\n");
	printf("*           HID Testing Menu            *\n");
	printf("* <1> Select Remote HID Device          *\n");
	printf("* <2> Unplug Device                     *\n");
	printf("* <3> Add one remote HID Device         *\n");
	printf("* <4> Restore linkkey for remote device *\n");
	printf("* <5> Connect the plugged device        *\n");
	printf("* <6> Start Listening data from driver  *\n");
	printf("* <7> Stop Listening data from driver   *\n");
	printf("* <8> Remove one remote HID Device      *\n");
	printf("* <r> Return to upper menu              *\n");
	printf("*****************************************\n");
	printf(">");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is the execution for HID sample.
Arguments:
	BTUINT8 choice
Return:
	void 
---------------------------------------------------------------------------*/
void HidExecCmd(BTUINT8 choice)
{
	BTINT8 ch = 0;
	BTUINT8 comNum = 0;
	BTUINT8 bd_addr[6] = {0};

	if (choice == '1')
	{
		TestSelectRmtHidDev();
		//TestConnectHidSvc();
		HidShowMenu();
		while (ch != 'r')
		{
			AppWaitForInput(&ch, 1);
		
			if (ch == '\n')
			{
				printf(">>");
			}
			else if('r' == ch)
			{
				break;
			}
			else
			{
				HidExecCmd(ch);
				printf("\n");
				HidShowMenu();
			}
		}
	}
	else if (choice == '2')
	{
		if (s_currHidConnHdl)
		{
			Btsdk_GetRemoteDeviceAddress(s_currRmtHidDevHdl, bd_addr);
			Btsdk_Hid_ClntUnPluggedDev(bd_addr);
			s_currHidConnHdl = BTSDK_INVALID_HANDLE;
		}
		else
		{
			printf("make sure exist connection now!");
		}
	}
	else if (choice == '3')
	{
		TestAddOneRemoteHIDDevice();
		HidShowMenu();
		while (ch != 'r')
		{
			AppWaitForInput(&ch, 1);
		
			if (ch == '\n')
			{
				printf(">>");
			}
			else if('r' == ch)
			{
				break;
			}
			else
			{
				HidExecCmd(ch);
				printf("\n");
				HidShowMenu();
			}
		}
	}
	else if (choice == '4')
	{
		if (s_currRmtHidDevHdl != BTSDK_INVALID_HANDLE)
		{
			char szLinkKey[BTSDK_LINK_KEY_LENGTH*2 +1] = {0};
			printf("Please input the Linkkey for remote HID device:\n");
			scanf(" %s", szLinkKey);
			RestoreLinkKeyForDevice(s_currRmtHidDevHdl, szLinkKey);
		}
		HidShowMenu();
		while (ch != 'r')
		{
			AppWaitForInput(&ch, 1);
		
			if (ch == '\n')
			{
				printf(">>");
			}
			else if('r' == ch)
			{
				break;
			}
			else
			{
				HidExecCmd(ch);
				printf("\n");
				HidShowMenu();
			}
		}
	}
	else if (choice == '5')
	{
		if (s_currRmtHidDevHdl != BTSDK_INVALID_HANDLE)
		{
			TestConnectTheRemoteHIDDevice(s_currRmtHidDevHdl);
		}
		HidShowMenu();
		while (ch != 'r')
		{
			AppWaitForInput(&ch, 1);
		
			if (ch == '\n')
			{
				printf(">>");
			}
			else if('r' == ch)
			{
				break;
			}
			else
			{
				HidExecCmd(ch);
				printf("\n");
				HidShowMenu();
			}
		}
	}
	else if (choice == '6')
	{
		if (s_currRmtHidDevHdl != BTSDK_INVALID_HANDLE)
		{
			StartListenDataFromRemoteHID(s_currRmtHidDevHdl);
		}
		HidShowMenu();
		while (ch != 'r')
		{
			AppWaitForInput(&ch, 1);
		
			if (ch == '\n')
			{
				printf(">>");
			}
			else if('r' == ch)
			{
				break;
			}
			else
			{
				HidExecCmd(ch);
				printf("\n");
				HidShowMenu();
			}
		}
	}
	else if (choice == '7')
	{
		StopListenDataFromRemoteHID();
		HidShowMenu();
		while (ch != 'r')
		{
			AppWaitForInput(&ch, 1);
	
			if (ch == '\n')
			{
				printf(">>");
			}
			else if('r' == ch)
			{
				break;
			}
			else
			{
				HidExecCmd(ch);
				printf("\n");
				HidShowMenu();
			}
		}
	}
	else if (choice == '8')
	{
		if (s_currRmtHidDevHdl != BTSDK_INVALID_HANDLE)
		{
			TestDeleteOneRemoteHIDDevice(s_currRmtHidDevHdl);
		}
		HidShowMenu();
		while (ch != 'r')
		{
			AppWaitForInput(&ch, 1);
		
			if (ch == '\n')
			{
				printf(">>");
			}
			else if('r' == ch)
			{
				break;
			}
			else
			{
				HidExecCmd(ch);
				printf("\n");
				HidShowMenu();
			}
		}
	}
	else
	{
		printf("Invalid command.\n");
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is the entry function for HID sample.
Arguments:
	void
Return:
	void 
---------------------------------------------------------------------------*/
void TestHidFunc(void)
{
	BTUINT8 ch = 0;
	BTUINT8 comNum = 0;
	BTUINT8 bd_addr[6] = {0};

	HidShowMenu();	
	while (ch != 'r')
	{
		AppWaitForInput(&ch, 1);
		
		if (ch == '\n')
		{
			printf(">>");
		}
		else if('r' == ch)
		{
			break;
		}
		else
		{
			HidExecCmd(ch);
			printf("\n");
			HidShowMenu();
		}
	}
	if (BTSDK_INVALID_HANDLE != s_currHidConnHdl)
	{
		Btsdk_GetRemoteDeviceAddress(s_currRmtHidDevHdl, bd_addr);
		Btsdk_Hid_ClntUnPluggedDev(bd_addr);
		s_currHidConnHdl = BTSDK_INVALID_HANDLE;
	}
}

unsigned char* StringNumber2HexEx(char* str_num, unsigned int str_len)
{
	unsigned int i = 0;
	unsigned int iByte = 0;
	char szByte[3] = {0, 0, 0};
	unsigned char* hex = NULL;
	
	if (!str_num || strlen(str_num) < str_len)
		return NULL;
	
	hex = (unsigned char*) malloc(str_len/2);
	memset(hex, 0, str_len/2);
	
	for(i=0; i<str_len/2; i++)
	{
		if ( ! ((*str_num>='A'&& *str_num<='F') || (*str_num>='0'&& *str_num<='9') || (*str_num>='a'&& *str_num<='f'))
			|| ! ((*(str_num+1)>='A'&& *(str_num+1)<='F') || (*(str_num+1)>='0'&& *(str_num+1)<='9') || (*(str_num+1)>='a'&& *(str_num+1)<='f')))
		{
			free(hex);
			return NULL;
		}
		
		szByte[0] = *str_num;
		szByte[1] = *(str_num+1);
		
		sscanf(szByte, "%02X", &iByte);
		
		hex[i] = (unsigned char) iByte;
		
		str_num += 2;
	}
	return hex;
}

void TransPnpInfoFromSdk2Sdap(BtSdkRmtDISvcExtAttrStru *Src,
							  BTP_SDAP_PNPINFO *Dest)
{
	/*the src->size is more than sizeof(struct BTP_SDAP_PNPINFO),use sizeof(struct BTP_SDAP_PNPINFO)
	to avoid access invalid memory when access BTP_SDAP_PNPINFO structure.*/
	Dest->size = sizeof(BTP_SDAP_PNPINFO);
	Dest->product_id = Src->product_id;
	Dest->mask = Src->mask;
	Dest->spec_id = Src->spec_id;
	Dest->svc_hdl = 0;
	Dest->vendor_id = Src->vendor_id;
	Dest->vendor_id_src = Src->vendor_id_source;
	Dest->version_value = Src->version;
};
#define FMTBD2STR(bd) TEXT("%02X:%02X:%02X:%02X:%02X:%02X"), bd[5],bd[4],bd[3],bd[2],bd[1],bd[0]
#define FMTBD2STRCONVER(bd) TEXT("%02X:%02X:%02X:%02X:%02X:%02X"), bd[0],bd[1],bd[2],bd[3],bd[4],bd[5]
BTINT32 CreateShortCut(BTDEVHDL hDevHdl, BTSVCHDL hSvcHdl, BTSHCHDL *phShcHdl)
{
	BTINT32 lRet = 0;
	BtSdkRemoteServiceAttrStru struRmtSvcAttr = {0};
	BTUINT8 pucDevAddr[BTSDK_BDADDR_LEN] = {0};
	BtSdkShortCutPropertyStru struShcProp = {0};
	char Temp[10] = {0};
	if (BTSDK_INVALID_HANDLE == hDevHdl
		||	BTSDK_INVALID_HANDLE == hSvcHdl)
	{
		return BTSDK_ER_INVALID_PARAMETER;
	}
	
	if (BTSDK_TRUE != Btsdk_IsBluetoothReady())
	{
		return BTSDK_ER_SDK_UNINIT;
	}
	
	// Start to create.
	struRmtSvcAttr.mask = BTSDK_RSAM_SERVICENAME;
	lRet = Btsdk_GetRemoteServiceAttributes(hSvcHdl, &struRmtSvcAttr);
	if (BTSDK_OK != lRet)
	{
		return lRet;
	}

	// If we can not get the service name, assign one.
	if (0 == struRmtSvcAttr.svc_name[0])
	{
		// Set the service name to the string associated with the service class.
		if (struRmtSvcAttr.service_class = BTSDK_CLS_HID)
		{
			strcpy(struRmtSvcAttr.svc_name, "Bluetooth Human Interface Device");
		}
	}
	
	// Get the device address.
	lRet = Btsdk_GetRemoteDeviceAddress(hDevHdl, pucDevAddr);
	if (BTSDK_OK != lRet)
	{
		return lRet;
	}
	
	// The device address, service class and service name are gotten,
	// we can create the shortcut now.
	struShcProp.dev_hdl = hDevHdl;
	struShcProp.by_dev_hdl = BTSDK_TRUE;
	struShcProp.svc_hdl = hSvcHdl;
	struShcProp.by_svc_hdl = BTSDK_TRUE;
	struShcProp.svc_class = struRmtSvcAttr.service_class;
	strcpy((CHAR*)struShcProp.svc_name, (const CHAR*)struRmtSvcAttr.svc_name);
	struShcProp.mask = BTSDK_SCPM_SHCNAME | BTSDK_SCPM_SVCNAME 
		| BTSDK_SCPM_DEVCLASS;
	Btsdk_GetRemoteDeviceClass(hDevHdl, &(struShcProp.dev_class));
	// Make the shortcut name. The shortcut name should be unique.
	// It is dev_addr + svc_cls + svc_name.
	sprintf((CHAR*)struShcProp.shc_name,FMTBD2STR(pucDevAddr));
	sprintf( Temp," %x ", struRmtSvcAttr.service_class);
	strcat((CHAR*)struShcProp.shc_name, Temp);
	strncat((CHAR*)struShcProp.shc_name, (CHAR*)struRmtSvcAttr.svc_name, 
		BTSDK_SHORTCUT_NAME_LEN - 30);
	
	lRet = Btsdk_CreateShortCutEx(&struShcProp);
	if (BTSDK_OK == lRet)
	{
		if (NULL != phShcHdl)
		{
			*phShcHdl = struShcProp.shc_hdl;
		}
	}
	return lRet;
}

BTSHCHDL CreateHIDShortcutByDevHdl(BTDEVHDL dev_hdl)
{
	BTSHCHDL shc_hdl = BTSDK_INVALID_HANDLE;
	BTUINT32 svc_count = 0;
	BTINT32 ret = Btsdk_BrowseRemoteServices(dev_hdl, NULL, &svc_count);
	if (BTSDK_OK == ret)
	{
		BTSVCHDL svc_hdl = BTSDK_INVALID_HANDLE;
		BTP_HID_HOSTINFO *HostInfo = NULL;
		BtSdkRmtHIDSvcExtAttrStru *HidAttr = NULL;
		BTP_SDAP_PNPINFO PnpAttr = {0};
		BtSdkRemoteServiceAttrStru SvcAttr = {0};
		BTSVCHDL hHIDSvcHdl = BTSDK_INVALID_HANDLE;
		BTSDKHANDLE hEnumHDL = Btsdk_StartEnumRemoteService(dev_hdl);
		if (hEnumHDL != BTSDK_INVALID_HANDLE)
		{
			SvcAttr.mask = BTSDK_RSAM_EXTATTRIBUTES;
			while (BTSDK_INVALID_HANDLE != (svc_hdl = Btsdk_EnumRemoteService(hEnumHDL, &SvcAttr)))
			{
				if (NULL != SvcAttr.ext_attributes)
				{
					if (BTSDK_CLS_PNP_INFO == SvcAttr.svc_class)
					{
						TransPnpInfoFromSdk2Sdap((BtSdkRmtDISvcExtAttrStru*)SvcAttr.ext_attributes, &PnpAttr);
						Btsdk_FreeMemory(SvcAttr.ext_attributes);
						SvcAttr.ext_attributes = NULL;
					}
					else if (BTSDK_CLS_HID == SvcAttr.svc_class)
					{
						HidAttr = (BtSdkRmtHIDSvcExtAttrStru*)SvcAttr.ext_attributes;
						hHIDSvcHdl = svc_hdl;
					}	
					else// free other service's attributes
					{
						Btsdk_FreeMemory(SvcAttr.ext_attributes);
						SvcAttr.ext_attributes = NULL;
					}
				}
			}
			Btsdk_EndEnumRemoteService(hEnumHDL);			
			if (hHIDSvcHdl)
			{
				CreateShortCut(dev_hdl, hHIDSvcHdl, &shc_hdl);
			}
		}
		if (HidAttr) 
		{
			//Create Shortcut
			BtSdkShortCutPropertyStru ShcAttr = {0};
			if (shc_hdl != BTSDK_INVALID_HANDLE)
			{
				ShcAttr.shc_hdl = shc_hdl;
				ShcAttr.mask |= BTSDK_SCPM_SHCATTR;
				Btsdk_GetShortCutProperty(&ShcAttr);
				
				ShcAttr.shc_attrib_len = sizeof(BTP_SDAP_PNPINFO) + HidAttr->size;
				HostInfo = (BTP_HID_HOSTINFO*)malloc(sizeof(BTUINT8)*ShcAttr.shc_attrib_len);
				if (NULL != HostInfo)
				{
					ZeroMemory(HostInfo, ShcAttr.shc_attrib_len);
					memcpy(&(HostInfo->pnpinfo), &PnpAttr, sizeof(BTP_SDAP_PNPINFO));
					memcpy(&(HostInfo->sdapInfo), HidAttr, HidAttr->size);
					ShcAttr.pshc_attrib = (BTUINT8*)HostInfo;
					ShcAttr.mask = BTSDK_SCPM_SHCATTR;
					Btsdk_SetShortCutProperty(&ShcAttr);
					free(HostInfo);
				}
			}
			if (NULL != HidAttr)
			{
				Btsdk_FreeMemory(HidAttr);
				HidAttr = NULL;
			}
			
		}
	}
	else
	{
		printf("Browse service failed, can not create shortcut, please try again.\n");
	}
	return shc_hdl;
}

BOOL PlugHIDDeviceByShcHdl(BTSHCHDL shc_hdl)
{
	BOOL bRet = FALSE;
	BtSdkShortCutPropertyStru shc_prop = {0};
	BTP_HID_HOSTINFO *pHid_hostinfo = NULL;
	BTINT32 ret = BTSDK_OK;
	shc_prop.shc_hdl = shc_hdl;	
	shc_prop.mask = BTSDK_SCPM_SHCATTR;
	ret = Btsdk_GetShortCutProperty(&shc_prop);
	if (BTSDK_OK == ret || BTSDK_ER_BUFFER_NOT_ENOUGH == ret)
	{
		if (0 == shc_prop.shc_attrib_len)
		{
			return FALSE;
		}
		shc_prop.pshc_attrib = (BTUINT8*)malloc(sizeof(BTUINT8)*shc_prop.shc_attrib_len);
		if (shc_prop.pshc_attrib != NULL)
		{
			memset(shc_prop.pshc_attrib, 0, sizeof(BTUINT8)*shc_prop.shc_attrib_len);
			Btsdk_GetShortCutProperty(&shc_prop);
			pHid_hostinfo = (BTP_HID_HOSTINFO *)shc_prop.pshc_attrib;			
			if (BTSDK_OK == Btsdk_Hid_ClntPluggedDev(shc_prop.bd_addr, 
							(BTUINT8 *)&(pHid_hostinfo->sdapInfo), 
							(BTUINT8 *)&(pHid_hostinfo->pnpinfo)))
			{
				bRet = TRUE;
			}

			free(shc_prop.pshc_attrib);
		}
	}
	return bRet;
}

void UnplugHIDDeviceByShcHdl(BTSHCHDL shc_hdl)
{
	if (shc_hdl != BTSDK_INVALID_HANDLE)
	{
		BtSdkShortCutPropertyStru shc_prop = {0};
		shc_prop.shc_hdl = shc_hdl;
		if (BTSDK_OK == Btsdk_GetShortCutProperty(&shc_prop))
		{
			Btsdk_Hid_ClntUnPluggedDev(shc_prop.bd_addr);
		}
	}
}

void TestAddOneRemoteHIDDevice()
{   
	char szBDAddress[MAX_PATH] = {0};
	BTUINT8 bd_addr[BTSDK_BDADDR_LEN] = {0};
	BTDEVHDL dev_hdl = BTSDK_INVALID_HANDLE;
	BTSHCHDL shc_hdl = BTSDK_INVALID_HANDLE;
	UCHAR *pKBD = NULL;
	if (s_currRmtHidDevHdl == BTSDK_INVALID_HANDLE)
	{
		printf("If the bluetooth address is (00:15:20:57:2F:39), input 392F57201F00\n");
		printf("Please input the remote device address:\n");
		AppWaitForInput(szBDAddress, MAX_PATH);

		pKBD =  StringNumber2HexEx(szBDAddress, strlen(szBDAddress));
		if (pKBD != NULL)
		{
			memcpy(bd_addr, pKBD, BTSDK_BDADDR_LEN);
			free(pKBD);
			dev_hdl = Btsdk_AddRemoteDevice(bd_addr);
			if (dev_hdl == BTSDK_INVALID_HANDLE)
			{
				return;
			}
			s_currRmtHidDevHdl = dev_hdl;
		}
	}
	else
	{
		dev_hdl = s_currRmtHidDevHdl;
	}
	if (dev_hdl != BTSDK_INVALID_HANDLE)
	{		
		Btsdk_GetShortCutByDeviceHandle(dev_hdl, BTSDK_CLS_HID, &shc_hdl, 1);
		if (shc_hdl == BTSDK_INVALID_HANDLE)
		{
			shc_hdl = CreateHIDShortcutByDevHdl(dev_hdl);
		}
		
		if (shc_hdl != BTSDK_INVALID_HANDLE)
		{
			PlugHIDDeviceByShcHdl(shc_hdl);
		}
	}
}


void TestDeleteOneRemoteHIDDevice(BTDEVHDL dev_hdl)
{
	BTUINT8 bd_addr[BTSDK_BDADDR_LEN] = {0};
	BTSHCHDL shc_hdl = BTSDK_INVALID_HANDLE;
	if (dev_hdl == BTSDK_INVALID_HANDLE)
	{
		return;	
	}

	if (BTSDK_OK != Btsdk_GetRemoteDeviceAddress(dev_hdl, bd_addr))
	{
		return;
	}
	Btsdk_GetShortCutByDeviceHandle(dev_hdl, BTSDK_CLS_HID, &shc_hdl, 1);
	if (shc_hdl != BTSDK_INVALID_HANDLE)
	{
		BtSdkShortCutPropertyStru shc_prop = {0};
		if (BTSDK_TRUE == Btsdk_Hid_ClntGetPlugFlag(bd_addr))
		{
			Btsdk_Hid_ClntUnPluggedDev(bd_addr);
		}
		shc_prop.shc_hdl = shc_hdl;
		Btsdk_GetShortCutProperty(&shc_prop);
		Btsdk_Disconnect(shc_prop.conn_hdl);
		Btsdk_DeleteShortCut(shc_hdl);
	}
	Btsdk_DeleteRemoteDeviceByHandle(dev_hdl);
}


void TestConnectTheRemoteHIDDevice(BTDEVHDL dev_hdl)
{
	BtSdkShortCutPropertyStru shc_prop = {0};
	BTSHCHDL shc_hdl = BTSDK_INVALID_HANDLE;
	Btsdk_GetShortCutByDeviceHandle(dev_hdl, BTSDK_CLS_HID, &shc_hdl, 1);
	if (shc_hdl != BTSDK_INVALID_HANDLE)
	{
		if (BTSDK_OK == Btsdk_ConnectShortCut(shc_hdl))
		{
			Btsdk_GetShortCutProperty(&shc_prop);
			s_currHidConnHdl = shc_prop.conn_hdl;
		}
		else
		{
			printf("Connect failed, please make sure the remote device is in connectable mode\n");
		}
	}
	else
	{
		printf("Connect failed, shortcut handle is NULL\n");
	}
}

BOOL IsWow64Proc()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE hProcess, PBOOL Wow64Process);
	
	BOOL bIsWow64 = FALSE;
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"),"IsWow64Process");
	
	if (fnIsWow64Process != NULL)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
        }
	}
	return bIsWow64;
}

BOOL IsX64Platform()
{
	BOOL bRet = FALSE;
#ifdef _WIN64
	bRet = TRUE;
#else
	if (IsWow64Proc())
	{
		bRet = TRUE;
	}
#endif
	return bRet;
}

void WriteLinkKeyToIni(BTDEVHDL dev_hdl, char* szLinkKey)
{
	char szIniPath[MAX_PATH] = {0};
	BTUINT8 bd_addr[BTSDK_BDADDR_LEN] = {0};
	char szBDAddr[MAX_PATH] = {0};
	if (dev_hdl == BTSDK_INVALID_HANDLE || szLinkKey == NULL)
	{
		return;
	}
	if (IsX64Platform())
	{
		GetWindowsDirectory(szIniPath, MAX_PATH);
		strcat(szIniPath, "\\");
		strcat(szIniPath, "SysWow64");
	}
	else
	{
		GetSystemDirectory(szIniPath, MAX_PATH);
	}
	strcat(szIniPath, "\\");
	strcat(szIniPath, "REMOTEDEVICE.ini");

	Btsdk_GetRemoteDeviceAddress(dev_hdl, bd_addr);
	sprintf(szBDAddr, FMTBD2STRCONVER(bd_addr));
	Btsdk_WritePrivateProfileString(szBDAddr, "link_key", szLinkKey, szIniPath);
}
void RestoreLinkKeyForDevice(BTDEVHDL dev_hdl, char *linkkeyStr)
{
	UCHAR *pKey = NULL;
	BTUINT8 lin_key[BTSDK_LINK_KEY_LENGTH] = {0};
	char cLinkKey[BTSDK_LINK_KEY_LENGTH*2+1] = {0};// This can be save the linkkey String
	if (dev_hdl == BTSDK_INVALID_HANDLE || linkkeyStr == NULL)
	{
		return;
	}
	strcpy(cLinkKey, linkkeyStr);
	//strcpy(cLinkKey, "D011767785741EF91A13568344347A0E"); //Sample for link-key
	{
		//Write linkkey to ini
		WriteLinkKeyToIni(dev_hdl, cLinkKey);
	}
	pKey = StringNumber2HexEx(cLinkKey, strlen(cLinkKey));
	if (pKey != NULL)
	{
		memcpy(lin_key, pKey, BTSDK_LINK_KEY_LENGTH);
		free(pKey);
		Btsdk_RecoverRemoteDeviceLinkKey(dev_hdl, lin_key);
	}
}

const GUID DECLSPEC_SELECTANY GUID_DEVINTERFACE_HID \
= { 0x4D1E55B2L, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } };
DEFINE_GUID(GUID_DEVINTERFACE_HID, 0x4D1E55B2L, 0xF16F, 0x11CF, 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 );

HANDLE GetRemoteDevHIDDrvHandle(char* vid, char* pid)
{
	HDEVINFO                            hardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA            deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA    deviceInterfaceDetailData = NULL;
    ULONG                               predictedLength = 0;
    ULONG                               requiredLength = 0;
    DWORD                               error;
    UINT                                i=0;
	GUID								InterfaceGuid = GUID_DEVINTERFACE_HID;
	SP_DEVINFO_DATA did = {sizeof(SP_DEVINFO_DATA)};
	HANDLE retHandle = INVALID_HANDLE_VALUE;
	char buf[MAX_PATH] = {0};
	char cHardwareID[MAX_PATH] = {0};
	char szDevID[MAX_PATH] = {0};
	//HID\VID_046D&PID_b008
	sprintf(szDevID, "HID\\VID_%s&PID_%s", vid, pid);
    // Create a Device Information Set with all present devices.
    hardwareDeviceInfo = SetupDiGetClassDevs((LPGUID)&InterfaceGuid,
		0,0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT ); // All devices present on system
    if (hardwareDeviceInfo == INVALID_HANDLE_VALUE)
	{
        return INVALID_HANDLE_VALUE;        
	}
	do
	{
		 DWORD DataT = 0;
		 char *p = NULL;
		 char buffer[MAX_PATH]={0};
		 DWORD buffersize = MAX_PATH;
		 deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		 for (i = 0; SetupDiEnumDeviceInterfaces(hardwareDeviceInfo,
			 0, // No care about specific PDOs
			 (LPGUID)&InterfaceGuid, 
			 i, 
			 &deviceInterfaceData); i++)
		 {
			 if (NULL != deviceInterfaceDetailData)
			 {
				 HeapFree (GetProcessHeap(), 0, deviceInterfaceDetailData);
			 }
			 if(!SetupDiGetDeviceInterfaceDetail (
				 hardwareDeviceInfo,
				 &deviceInterfaceData,
				 NULL, // probing so no output buffer yet
				 0, // probing so output buffer length of zero
				 &requiredLength,
				 &did) && (error = GetLastError()) != ERROR_INSUFFICIENT_BUFFER)
			 {
				 goto _exit;
			 }
			 
			 predictedLength = requiredLength;
			 deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
				 predictedLength);
			 if(deviceInterfaceDetailData == NULL)
			 {
				 goto _exit;
			 }
			 
			 deviceInterfaceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
			 
			 if (!SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfo, &deviceInterfaceData, deviceInterfaceDetailData,
				 predictedLength, &requiredLength, &did))
			 {
				 goto _exit;
			 }
			 //HID\VID_046D&PID_b008
			 memset(cHardwareID, 0, sizeof(cHardwareID));
			 SetupDiGetDeviceRegistryProperty(hardwareDeviceInfo, &did, SPDRP_HARDWAREID, NULL, (PBYTE) cHardwareID, sizeof(cHardwareID), NULL);
			 if (NULL != StrStrI(cHardwareID, szDevID))
			 {
				retHandle = CreateFile (
						 deviceInterfaceDetailData->DevicePath,
						 GENERIC_READ | GENERIC_WRITE,
						 FILE_SHARE_READ|FILE_SHARE_WRITE,
						 NULL, // no SECURITY_ATTRIBUTES structure
						 OPEN_EXISTING, // No special create flags
						 0, // No special attributes
						 NULL);
				 if (INVALID_HANDLE_VALUE == retHandle)
				 {
					 continue;
				 }
				 break;				
			 }
		 }
_exit:
		 if(deviceInterfaceDetailData)
		 {
			 HeapFree (GetProcessHeap(), 0, deviceInterfaceDetailData);
		 }
	}while(FALSE);
    SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
	return retHandle;
}

DWORD WINAPI ReportListenerThread(LPVOID param)
{
	DWORD dwError = 0;
	DWORD result, bytes_read;
	unsigned char input_report[46];
	while (s_HIDDrvDev != INVALID_HANDLE_VALUE && s_bIsListen)
	{
		ZeroMemory(input_report, 46);		
		// Issue a read request
		result = ReadFile(s_HIDDrvDev, 
				input_report, 
				s_HID_CAP.InputReportByteLength, 
				&bytes_read, 
				(LPOVERLAPPED)&(s_HID_overlapped));			
		dwError = GetLastError();
			
		// Wait for read to finish
		result = WaitForSingleObject(s_HID_overlapped.hEvent, 1000);
		
		ResetEvent(s_HID_overlapped.hEvent);		
		// If the wait didn't result in a successful read, try again
		if (result != WAIT_OBJECT_0)
		{
			continue;
		}		
		printf("%d bytes data read.\n", bytes_read);
	}
	return 0;
}

void GetDeviceCapabilities(HANDLE hDevice)
{
	//Get the Capabilities structure for the device.
	PHIDP_PREPARSED_DATA PreparsedData;
#ifdef _WIN32	
	HidD_GetPreparsedData(hDevice, &PreparsedData);
	HidP_GetCaps(PreparsedData, &s_HID_CAP);
	HidD_FreePreparsedData(PreparsedData);
#endif
}
extern char* HexNumber2String(unsigned char* hex_num, int hex_len);
void StartListenDataFromRemoteHID(BTDEVHDL dev_hdl)
{
	BTSHCHDL shc_hdl = BTSDK_INVALID_HANDLE;
	BtSdkShortCutPropertyStru shc_prop = {0};
	BTINT32 ret = BTSDK_OK;
	BTP_HID_HOSTINFO *pHid_hostinfo = NULL;
	BOOL bIsDevIDSucceed = FALSE;
	char szPID[MAX_PATH] = {0};
	char szVID[MAX_PATH] = {0};
	Btsdk_GetShortCutByDeviceHandle(dev_hdl, BTSDK_CLS_HID, &shc_hdl, 1);
	if (shc_hdl != BTSDK_INVALID_HANDLE)
	{
		shc_prop.shc_hdl = shc_hdl;
		shc_prop.mask = BTSDK_SCPM_SHCATTR;
		ret = Btsdk_GetShortCutProperty(&shc_prop);
		if (BTSDK_OK == ret || BTSDK_ER_BUFFER_NOT_ENOUGH == ret)
		{
			if (0 == shc_prop.shc_attrib_len)
			{
				return;
			}
			shc_prop.pshc_attrib = (BTUINT8*)malloc(sizeof(BTUINT8)*shc_prop.shc_attrib_len);
			if (shc_prop.pshc_attrib != NULL)
			{
				memset(shc_prop.pshc_attrib, 0, sizeof(BTUINT8)*shc_prop.shc_attrib_len);
				Btsdk_GetShortCutProperty(&shc_prop);
				pHid_hostinfo = (BTP_HID_HOSTINFO *)shc_prop.pshc_attrib;
				bIsDevIDSucceed = TRUE;
				{
					char *pStr = NULL;
					BTUINT8 startByte = ((BTUINT8*)&(pHid_hostinfo->pnpinfo.product_id))[1];
					BTUINT8 endByte = ((BTUINT8*)&(pHid_hostinfo->pnpinfo.product_id))[0];
					pStr = HexNumber2String((unsigned char*)(&startByte), sizeof(BTUINT8));
					if (pStr != NULL)
					{
						strcpy(szPID, pStr);
						free(pStr);
						pStr = NULL;
					}
					pStr = HexNumber2String((unsigned char*)(&endByte), sizeof(BTUINT8));
					if (pStr != NULL)
					{
						strcat(szPID, pStr);
						free(pStr);
						pStr = NULL;
					}
					startByte = ((BTUINT8*)&(pHid_hostinfo->pnpinfo.vendor_id))[1];
					endByte = ((BTUINT8*)&(pHid_hostinfo->pnpinfo.vendor_id))[0];
					pStr = HexNumber2String((unsigned char*)(&startByte), sizeof(BTUINT8));
					if (pStr != NULL)
					{
						strcpy(szVID, pStr);
						free(pStr);
						pStr = NULL;
					}
					pStr = HexNumber2String((unsigned char*)(&endByte), sizeof(BTUINT8));
					if (pStr != NULL)
					{
						strcat(szVID, pStr);
						free(pStr);
						pStr = NULL;
					}
				}
				free(shc_prop.pshc_attrib);
			}
		}		
	}

	if (!bIsDevIDSucceed)
	{
		return;
	}
	
	{
		s_HIDDrvDev = GetRemoteDevHIDDrvHandle(szVID, szPID);
		if (s_HIDDrvDev != INVALID_HANDLE_VALUE)
		{
			GetDeviceCapabilities(s_HIDDrvDev);
			memset(&s_HID_overlapped, 0, sizeof(OVERLAPPED));
			s_HID_overlapped.hEvent = CreateEvent(NULL, TRUE, TRUE, "HID Driver Read");
			s_bIsListen = TRUE;
			s_hTheadListen = (HANDLE)CreateThread(0, 
									0, 
									ReportListenerThread, 
									NULL, 
									0, 
 									NULL);
			if (s_hTheadListen == NULL)
			{
				CloseHandle(s_HID_overlapped.hEvent);
				memset(&s_HID_overlapped, 0, sizeof(OVERLAPPED));
				CloseHandle(s_HIDDrvDev);
				s_HIDDrvDev = INVALID_HANDLE_VALUE;
				s_bIsListen = FALSE;
			}
		}
	}
}

void StopListenDataFromRemoteHID()
{
	DWORD dwWaitResult = 0;
	if (s_bIsListen)
	{
		s_bIsListen = FALSE;
	}
	dwWaitResult = WaitForSingleObject(s_hTheadListen, 5000); //wait 5s
	if (dwWaitResult == WAIT_TIMEOUT)
	{
		TerminateThread(s_hTheadListen, 0);
	}

	CloseHandle(s_hTheadListen);
	s_hTheadListen = NULL;

	if (s_HIDDrvDev != INVALID_HANDLE_VALUE)
	{
		CloseHandle(s_HIDDrvDev);
		s_HIDDrvDev = INVALID_HANDLE_VALUE;
	}

	if (s_HID_overlapped.hEvent != NULL)
	{
		CloseHandle(s_HID_overlapped.hEvent);
	}
	memset(&s_HID_overlapped, 0, sizeof(OVERLAPPED));
	memset(&s_HID_CAP, 0, sizeof(HIDP_CAPS));

}