/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2005 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/
 
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    sdk_tst.c
Abstract:
	Samples codes of IVT Bluetooth API
Revision History:
2007-5-30   Yang Songhua  Created
<<<<<<< HEAD
=======
2016-3-8	Nick Memme	Edited
>>>>>>> refs/remotes/origin/master

---------------------------------------------------------------------------*/

#include "sdk_tst.h"
#include "profiles_tst.h"
<<<<<<< HEAD
=======
#include "sdk_gatt.h"

typedef struct MenuInputStru
{
	HANDLE ev_hdl;
	UCHAR *out_str;
	int str_size;
	struct MenuInputStru *next;
}NODESTRU;

typedef struct {/* New List Structure by hiw */
	void *head;
	void *tail;
}LISTSTRU;

static DWORD s_input_lock;
static LISTSTRU s_menus;

>>>>>>> refs/remotes/origin/master

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function shows the main menu
Arguments:
Return:
	void 
---------------------------------------------------------------------------*/
void SdkTestShowMenu()
{
	printf("\n\n");
<<<<<<< HEAD
	printf("  BlueSoleil SDK Sample App Ver 2.0.5    \n");
=======
	printf("  BlueSoleil SDK Sample App Ver 2.1.6    \n");
>>>>>>> refs/remotes/origin/master
	printf("*****************************************\n");
	printf("*         BTSDK Testing Menu            *\n");
	printf("* <1> Local Device Manager              *\n");
	printf("* <2> Remote Device Manager             *\n");
	printf("* <3> Profile Manager                   *\n");
	printf("* <m> Return to This Menu Again         *\n");
	printf("* <q> Quit                              *\n");
	printf("*****************************************\n");
	printf(">");
}

<<<<<<< HEAD
=======
void List_AddTail(LISTSTRU *list, NODESTRU *node)
{

	NODESTRU *temp = list->head;

	if (list->tail == NULL) 
	{
		list->head = node;
	} 
	else 
	{
		while(temp->next)
		{
			temp = temp->next;
		}
		temp->next = node;
	}
	list->tail = node;
	node->next = NULL;

}

NODESTRU *List_RemoveTail(LISTSTRU *list)
{	
	NODESTRU *node = list->head;
	NODESTRU *temp = list->head;
	
	if(node != NULL)
	{
		if (node->next != NULL) 
		{
			while(temp->next)
			{
				node = temp;
				temp = temp->next;
			}
			list->tail = node;  
			node->next = NULL;
		}
		else
		{
			list->tail = NULL;
			list->head = NULL;
		}
	}

	return temp;
}

int AppWaitForInput(char *out_str, int str_size)
{
	NODESTRU *input;
	NODESTRU *output;
	char *str_input = NULL;
	long r;
	
	input = malloc(sizeof(struct MenuInputStru));
	memset(input,0,sizeof(struct MenuInputStru));
	input->ev_hdl = CreateEvent(NULL,FALSE,FALSE,NULL);
	input->out_str = out_str;
	input->str_size = str_size;
	memset(out_str, 0, str_size);
	WaitForSingleObject((HANDLE)s_input_lock,INFINITE);
	List_AddTail(&s_menus, input);
	ReleaseSemaphore(s_input_lock, 1, &r);
	
	do
	{
		if (input == s_menus.head)
		{
			if (str_input == NULL)
			{
				str_input = malloc(512);
			}
			scanf("%s", str_input);
			WaitForSingleObject((HANDLE)s_input_lock,INFINITE);
			output = List_RemoveTail(&s_menus);
			ReleaseSemaphore(s_input_lock, 1, &r);
			if (output->str_size > (int)strlen(str_input))
			{
				strcpy(output->out_str, str_input);
			}
			else
			{
				memcpy(output->out_str, str_input, output->str_size);
			}
			if (input != output)
			{
				SetEvent(output->ev_hdl);
			}
			else
			{
				break;
			}
		}
		else
		{
			WaitForSingleObject(input->ev_hdl,INFINITE);
			break;
		}
	} while (s_menus.head != NULL);
	
	CloseHandle(input->ev_hdl);
	free(input);
	input = NULL;
	if (str_input != NULL)
	{
		free(str_input);
	}
	return strlen(out_str);
}

BTUINT32 AppWaitForInputInt(BTUINT32 *outp)
{
	char int_str[24];
	BTUINT32 ret;
	
	AppWaitForInput(int_str, 24);
	ret = atol(int_str);
	if (outp != NULL)
	{
		*outp = ret;
	}
	return ret;
}

BTUINT16 AppWaitForInputSFloat(BTUINT16 *outp)
{
	char int_str[24] = {0};
	BTUINT16 ret = 0;
	char integer_str[24] = {0};
	char *point;
	char decimal_str[24] = {0};
	BTUINT8 len;
	int decimal;
	int i;

	AppWaitForInput(int_str, 24);

	strcpy(integer_str, int_str);
	point = strchr(integer_str,'.'); //find the '.'
	
	if(point == NULL)
	{
		ret = atoi(integer_str);
		if(ret > 4095)
		{
			printf("Your input is beyond the scope of percision!\nPlease input again:");
			ret = AppWaitForInputSFloat(outp);
			return ret;
		}
	}
	else
	{
		*point = 0;
		strcpy(decimal_str, point+1);
		len = strlen(decimal_str);
		for(i=0; i<len; i++)	
		{
			if(decimal_str[i] != '0') //if the decimal is .0....
			{
				break;
			}
		}
		decimal = atoi(decimal_str);
		itoa(decimal, decimal_str, 10);	
		len = strlen(decimal_str);
		len = len + i;
		for(; i>0; i--)
		{
			strcat(integer_str, "0");
		}
		strcat(integer_str, decimal_str);
		ret = atoi(integer_str);
		len = 0xF - len + 1;
		if(ret > 4095)
		{
			printf("Your input is beyond the scope of percision!\nPlease input again:");
			ret = AppWaitForInputSFloat(outp);
			return ret;
		}
		ret = len * 4096 + ret;
	}
	
	if (outp != NULL)
	{
		*outp = ret;
	}

	return ret;
}

BTUCHAR *SFloatToString(BTUINT16 data,BTUCHAR *out_str)
{
	BTUCHAR ret_str[24] = {0};
	BTUCHAR integer_str[24] = {0};
	BTUCHAR decimal_str[24] = {0};
	int decimal = 0;
	int integer = 0;
	int exponent = 0;
	int len = 0;
	int i;
	
	integer = data % 4096;
	itoa(integer, integer_str, 10);
	len = strlen(integer_str);
	if(data/4096 == 0)
	{
		exponent = 0;
		strcpy(out_str, integer_str);
		return out_str;
	}
	else
	{
		exponent = 16 - data / 4096; 
	}

	if(exponent >= len)	//if integer is 0
	{
		strcpy(decimal_str, "0.");
		for(i=0; i<(exponent-len); i++)
		{
			strcat(decimal_str, "0");
		}
		strcat(decimal_str, integer_str);
		strcpy(out_str, decimal_str);
		return out_str;
	}
	else
	{
		for(i=0; i<exponent; i++)
		{
			integer_str[len-i] = integer_str[len-i-1];
		}
		integer_str[len-exponent] = '.';
		strcpy(out_str, integer_str);
	}
	return out_str;
}

>>>>>>> refs/remotes/origin/master
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is a callback function to get status from COM Server
Arguments:
	usMsgType: [in] message type
	pucData:   [in] message base on message type
	param:     [in] device or service handle
	arg:       [in] not used now
Return:
	void 
---------------------------------------------------------------------------*/
void BsStatusCBKFuc(ULONG usMsgType, ULONG pucData, ULONG param, BTUINT8 *arg)
{
	/* message received */
	switch(usMsgType)
	{
	case BTSDK_BLUETOOTH_STATUS_FLAG:
		{
			switch(pucData)
			{
			case BTSDK_BTSTATUS_TURNON:
				{
					//printf("MSG: Bluetooth is turned on.\n");
					break;
				}		
			case BTSDK_BTSTATUS_TURNOFF:
				{
					//printf("MSG: Bluetooth is turned off.\n");
					break;
				}
			case BTSDK_BTSTATUS_HWPLUGGED:
				{
					//printf("MSG: Bluetooth hardware is plugged.\n");
					break;
				}
				
			case BTSDK_BTSTATUS_HWPULLED:
				{
					//printf("MSG: Bluetooth hardware is pulled out.\n");
					break;
				}
			default:
				{
					//printf("MSG: Others.\n");
					break;
				}			
			}		
			break;
		}
	default:
		{
			//printf("MSG Received. Type: OTHER MESSAGES.\n");
			break;
		}		
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is to execute user's choice.
Arguments:
	BTUINT8 choice: [in] user's choice
Return:
	void 
---------------------------------------------------------------------------*/
void ExecInputCmd(BTUINT8 choice)
{
	switch (choice) 
	{
		case '1':
			TestLocDevMgr();
			break;
		case '2':
			TestRmtDevMgr();
			break;
		case '3':
			TestProfiles();			
			break;
		case 'm':
			system("cls");
			SdkTestShowMenu();
			break;
		case 'q':
			break;
		default:
			printf("Invalid command.\n");
			break;
	}
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function registers callback function to get status change of BlueSoleil.
Arguments:
	void
Return:
	void 
---------------------------------------------------------------------------*/
void Test_RegisterGetStatusCBK(void)
{
	/* register callback function to get the status change of BlueSoleil. */
	Btsdk_RegisterGetStatusInfoCB4ThirdParty(BsStatusCBKFuc);
	Btsdk_SetStatusInfoFlag(BTSDK_BLUETOOTH_STATUS_FLAG);		
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is to conduct some initialization operations.
Arguments:
Return:
	BOOL 
---------------------------------------------------------------------------*/
BOOL InitBlueSoleilForSample()
{
	if (BTSDK_TRUE != Btsdk_IsServerConnected()) /* not connected with BlueSoleil */
	{
		if (BTSDK_OK == Btsdk_Init())
		{
			printf("Connected to BlueSoleil Server successfully.\n\n");		
		}
		else
		{
			printf("Fail to connect to BlueSoleil Server.\n\n");
			return FALSE;
		}			
	}
	return TRUE;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Description:
	This function is the main function
Arguments:
	void
Return:
	always return 0 
---------------------------------------------------------------------------*/
int main(void)
{
	BTUINT8 chInputCmd = 0;
	BTUINT8 chEnterChoice = 0;
	
<<<<<<< HEAD
=======
	s_input_lock = (DWORD)CreateSemaphore(NULL, 1, 1, NULL);

>>>>>>> refs/remotes/origin/master
	printf("IVT BlueSoleil SDK is being Initialized....\n");
	if (FALSE == InitBlueSoleilForSample())
	{
		printf("Fail to initialize BlueSoleil, assure BlueSoleil is installed!\n");
		printf("Press any key to exit this application please.\n");
<<<<<<< HEAD
		scanf(" %c", &chEnterChoice);
		getchar();
=======
		AppWaitForInput(&chEnterChoice, 1);

		CloseHandle((HANDLE)s_input_lock);
>>>>>>> refs/remotes/origin/master
		return 1;
	}
	else
	{		
		RegAppIndCallback();
		Test_RegisterGetStatusCBK();
		
		if (BTSDK_TRUE != Btsdk_IsBluetoothHardwareExisted())
		{
			printf("There isn't any Bluetooth hardware detected.\n");
	        printf("1. Enter 'N' to exit this application.\n");
			printf("2. Plug a Bluetooth hardware and enter 'Y' to continue.\n");
			
			while (TRUE)
			{
<<<<<<< HEAD
				scanf(" %c",&chEnterChoice);
				getchar();
=======
				AppWaitForInput(&chEnterChoice, 1);
>>>>>>> refs/remotes/origin/master

				if (('y'==chEnterChoice)||('Y'==chEnterChoice))
				{
					if (BTSDK_TRUE == Btsdk_IsBluetoothHardwareExisted())
					{
						printf("Bluetooth hardware is detected.\n");
						break;
					}
					else
					{
						printf("Bluetooth hardware isn't detected and plug it again please.\n");
						printf("Enter 'Y' to try again, Enter 'N' to exit this application.\n");
						printf(">");
					}
				}
				else if(('n'==chEnterChoice)||('N'== chEnterChoice))
				{
<<<<<<< HEAD
=======
					CloseHandle((HANDLE)s_input_lock);
>>>>>>> refs/remotes/origin/master
					return 1;
				}
				else
				{
					printf("You have entered into an invalid character.\n");
				}				
			}
		}		
		
		if (BTSDK_FALSE == Btsdk_IsBluetoothReady())
		{
			Btsdk_StartBluetooth();
		}			
		
		if (BTSDK_TRUE == Btsdk_IsBluetoothReady())
		{
		/*we default expect this application runs on desktop platform. 
			of course, you can set another device class according to your need. */
<<<<<<< HEAD
			Btsdk_SetLocalDeviceClass(BTSDK_COMPCLS_DESKTOP);				
			
			SdkTestShowMenu();
			while (chInputCmd != 'q')
			{
				scanf(" %c", &chInputCmd);
				getchar();
=======
			Btsdk_SetLocalDeviceClass(BTSDK_COMPCLS_DESKTOP);
			GATTSvrSetLocalName();
			GATTSvrEnableAdvertising();
			GATTInitLocalAttribute();
		}
			/*
			SdkTestShowMenu();
			while (chInputCmd != 'q')
			{
				AppWaitForInput(&chInputCmd, 1);

>>>>>>> refs/remotes/origin/master
				if ('\n' == chInputCmd)
				{
					printf(">>");
				}
				else
				{
					ExecInputCmd(chInputCmd);
					printf("\n");
					if (chInputCmd != 'q')
					{
						SdkTestShowMenu();
					}
				}
			}
			
		}
		else
		{
			printf("BlueSoleil fail to reset hardware...\n");
		}
		
		printf("IVT BlueSoleil SDK is being quitted....\n");
		Btsdk_RegisterGetStatusInfoCB4ThirdParty(NULL);
		UnRegAppIndCallback();	
		Btsdk_Done();
<<<<<<< HEAD
=======
		CloseHandle((HANDLE)s_input_lock);*/
>>>>>>> refs/remotes/origin/master
		return 0;
		
	}	
}

