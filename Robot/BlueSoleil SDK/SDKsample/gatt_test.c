#include "sdk_tst.h"
#include "sdk_gatt.h"
#include "..\include\Btsdk_ui.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>
//#include <sys/utime.h>
#include <time.h>
#define BUFF_NUM 20
BtsdkGATTServiceStru svc[BUFF_NUM] = {0};
BtsdkGATTCharacteristicStru crt[BUFF_NUM] = {0};
BtsdkGATTDescriptorStru dsc[BUFF_NUM] = {0};
BTDEVHDL rmt_ble_dev = BTSDK_INVALID_HANDLE;
BTSDKHANDLE ev_hdl = BTSDK_INVALID_HANDLE;
BTUINT16 svc_num = 0;
BTUINT16 crt_num = 0;
BTUINT16 dsc_num = 0;
BTUCHAR robotCMD[]="P1M00C00P2M00C00";
static BOOL s_LEScanAuto = FALSE;
	char devs[100][20];
#define dim(x)		(sizeof(x)/sizeof(x[0]))

BTDEVHDL s_rmt_le_dev_hdls[MAX_DEV_NUM] = {0};
BTINT32 s_rmt_le_dev_num = 0; 

typedef struct _GATT_UUIDNameStru
{
	BTUINT16 uuid;
	BTUINT8 *name;
}GATT_UUIDNameStru;

GATT_UUIDNameStru g_gatt_uuid_list[] =
{
	/* Services */
	{0x1811, "Alert Notification Service"	},
	{0x180F, "Battery Service"				},
	{0x1810, "Blood Pressure"				},
	{0x1805, "Current Time Service"			},
	{0x1816, "Cycling Speed and Cadence"	},
	{0x180A, "Device Information"			},
	{0x1800, "Generic Access"				},
	{0x1801, "Generic Attribute"			},
	{0x1808, "Glucose"						},
	{0x1809, "Health Thermometer"			},
	{0x180D, "Heart Rate"					},
	{0x1812, "Human Interface Device"		},
	{0x1802, "Immediate Alert"				},
	{0x1803, "Link Loss"					},
	{0x1807, "Next DST Change Service"		},
	{0x180E, "Phone Alert Status Service"	},
	{0x1806, "Reference Time Update Service"},
	{0x1814, "Running Speed and Cadence"	},
	{0x1813, "Scan Parameters"				},
	{0x1804, "Tx Power"						},

	/* Characteristics */
	{0x2A43, "AlertCategoryID"								 },
	{0x2A42, "AlertCategoryIDBitMask"                        },
	{0x2A06, "AlertLevel"                                    },
	{0x2A44, "AlertNotificationControlPoint"                 },
	{0x2A3F, "AlertStatus"                                   },
	{0x2A01, "Appearance"                                    },
	{0x2A19, "BatteryLevel"                                  },
	{0x2A49, "BloodPressureFeature"                          },
	{0x2A35, "BloodPressureMeasurement"                      },
	{0x2A38, "BodySensorLocation"                            },
	{0x2A22, "BootKeyboardInputReport"                       },
	{0x2A32, "BootKeyboardOutputReport"                      },
	{0x2A33, "BootMouseInputReport"                          },
	{0x2A5C, "CSCFeature"                                    },
	{0x2A5B, "CSCMeasurement"                                },
	{0x2A2B, "CurrentTime"                                   },
	{0x2A08, "DateTime"                                      },
	{0x2A0A, "DayDateTime"                                   },
	{0x2A09, "DayofWeek"                                     },
	{0x2A00, "DeviceName"                                    },
	{0x2A0D, "DSTOffset"                                     },
	{0x2A0C, "ExactTime256"                                  },
	{0x2A26, "FirmwareRevisionString"                        },
	{0x2A51, "GlucoseFeature"                                },
	{0x2A18, "GlucoseMeasurement"                            },
	{0x2A34, "GlucoseMeasurementContext"                     },
	{0x2A27, "HardwareRevisionString"                        },
	{0x2A39, "HeartRateControlPoint"                         },
	{0x2A37, "HeartRateMeasurement"                          },
	{0x2A4C, "HIDControlPoint"                               },
	{0x2A4A, "HIDInformation"                                },
	{0x2A2A, "IEEE11073-20601RegulatoryCertificationDataList"},
	{0x2A36, "IntermediateCuffPressure"                      },
	{0x2A1E, "IntermediateTemperature"                       },
	{0x2A0F, "LocalTimeInformation"                          },
	{0x2A29, "ManufacturerNameString"                        },
	{0x2A21, "MeasurementInterval"                           },
	{0x2A24, "ModelNumberString"                             },
	{0x2A46, "NewAlert"                                      },
	{0x2A04, "PeripheralPreferredConnectionParameters"       },
	{0x2A02, "PeripheralPrivacyFlag"                         },
	{0x2A50, "PnPID"                                         },
	{0x2A4E, "ProtocolMode"                                  },
	{0x2A03, "ReconnectionAddress"                           },
	{0x2A52, "RecordAccessControlPoint"                      },
	{0x2A14, "ReferenceTimeInformation"                      },
	{0x2A4D, "Report"                                        },
	{0x2A4B, "ReportMap"                                     },
	{0x2A40, "RingerControlPoint"                            },
	{0x2A41, "RingerSetting"                                 },
	{0x2A54, "RSCFeature"                                    },
	{0x2A53, "RSCMeasurement"                                },
	{0x2A55, "SCControlPoint"                                },
	{0x2A4F, "ScanIntervalWindow"                            },
	{0x2A31, "ScanRefresh"                                   },
	{0x2A5D, "SensorLocation"                                },
	{0x2A25, "SerialNumberString"                            },
	{0x2A05, "ServiceChanged"                                },
	{0x2A28, "SoftwareRevisionString"                        },
	{0x2A47, "SupportedNewAlertCategory"                     },
	{0x2A48, "SupportedUnreadAlertCategory"                  },
	{0x2A23, "SystemID"                                      },
	{0x2A1C, "TemperatureMeasurement"                        },
	{0x2A1D, "TemperatureType"                               },
	{0x2A12, "TimeAccuracy"                                  },
	{0x2A13, "TimeSource"                                    },
	{0x2A16, "TimeUpdateControlPoint"                        },
	{0x2A17, "TimeUpdateState"                               },
	{0x2A11, "TimewithDST"                                   },
	{0x2A0E, "TimeZone"                                      },
	{0x2A07, "TxPowerLevel"                                  },
	{0x2A45, "UnreadAlertStatus"                             },

	/* Descriptors */
	{0x2905, "CharacteristicAggregateFormat"		},
	{0x2900, "CharacteristicExtendedProperties"		},
	{0x2904, "CharacteristicPresentationFormat"		},
	{0x2901, "CharacteristicUserDescription"     	},
	{0x2902, "ClientCharacteristicConfiguration" 	},
	{0x2907, "ExternalReportReference"           	},
	{0x2908, "ReportReference"                   	},
	{0x2903, "ServerCharacteristicConfiguration" 	},
	{0x2906, "ValidRange"                        	},

	/* Declarations */
	{0x2803, "GATT Characteristic Declaration"   },
	{0x2802, "GATT Include Declaration"          },
	{0x2800, "GATT Primary Service Declaration"  },
	{0x2801, "GATT Secondary Service Declaration"},

	/* Units */
	/* Each unit has a reference to a table within the 8th edition of the SI brochure of the International Bureau of Weights and Measures (BIPM). */
	{0x2700,  "unitless"                                                },
	{0x2701,  "length (metre)"                                          },
	{0x2702,  "mass (kilogram)"                                         },
	{0x2703,  "time (second)"                                           },
	{0x2704,  "electric current (ampere)"                               },
	{0x2705,  "thermodynamic temperature (kelvin)"                      },
	{0x2706,  "amount of substance (mole)"                              },
	{0x2707,  "luminous intensity (candela)"                            },
	{0x2710,  "area (square metres)"                                    },
	{0x2711,  "volume (cubic metres)"                                   },
	{0x2712,  "velocity (metres per second)"                            },
	{0x2713,  "acceleration (metres per second squared)"                },
	{0x2714,  "wavenumber (reciprocal metre)"                           },
	{0x2715,  "density (kilogram per cubic metre)"                      },
	{0x2716,  "surface density (kilogram per square metre)"             },
	{0x2717,  "specific volume (cubic metre per kilogram)"              },
	{0x2718,  "current density (ampere per square metre)"               },
	{0x2719,  "magnetic field strength (ampere per metre)"              },
	{0x271A,  "amount concentration (mole per cubic metre)"             },
	{0x271B,  "mass concentration (kilogram per cubic metre)"           },
	{0x271C,  "luminance (candela per square metre)"                    },
	{0x271D,  "refractive index"                                        },
	{0x271E,  "relative permeability"                                   },
	{0x2720,  "plane angle (radian)"                                    },
	{0x2721,  "solid angle (steradian)"                                 },
	{0x2722,  "frequency (hertz)"                                       },
	{0x2723,  "force (newton)"                                          },
	{0x2724,  "pressure (pascal)"                                       },
	{0x2725,  "energy (joule)"                                          },
	{0x2726,  "power (watt)"                                            },
	{0x2727,  "electric charge (coulomb)"                               },
	{0x2728,  "electric potential difference (volt)"                    },
	{0x2729,  "capacitance (farad)"                                     },
	{0x272A,  "electric resistance (ohm)"                               },
	{0x272B,  "electric conductance (siemens)"                          },
	{0x272C,  "magnetic flex (weber)"                                   },
	{0x272D,  "magnetic flex density (tesla)"                           },
	{0x272E,  "inductance (henry)"                                      },
	{0x272F,  "Celsius temperature (degree Celsius)"                    },
	{0x2730,  "luminous flux (lumen)"                                   },
	{0x2731,  "illuminance (lux)"                                       },
	{0x2732,  "activity referred to a radionuclide (becquerel)"         },
	{0x2733,  "absorbed dose (gray)"                                    },
	{0x2734,  "dose equivalent (sievert)"                               },
	{0x2735,  "catalytic activity (katal)"                              },
	{0x2740,  "dynamic viscosity (pascal second)"                       },
	{0x2741,  "moment of force (newton metre)"                          },
	{0x2742,  "surface tension (newton per metre)"                      },
	{0x2743,  "angular velocity (radian per second)"                    },
	{0x2744,  "angular acceleration (radian per second squared)"        },
	{0x2745,  "heat flux density (watt per square metre)"               },
	{0x2746,  "heat capacity (joule per kelvin)"                        },
	{0x2747,  "specific heat capacity (joule per kilogram kelvin)"      },
	{0x2748,  "specific energy (joule per kilogram)"                    },
	{0x2749,  "thermal conductivity (watt per metre kelvin)"            },
	{0x274A,  "energy density (joule per cubic metre)"                  },
	{0x274B,  "electric field strength (volt per metre)"                },
	{0x274C,  "electric charge density (coulomb per cubic metre)"       },
	{0x274D,  "surface charge density (coulomb per square metre)"       },
	{0x274E,  "electric flux density (coulomb per square metre)"        },
	{0x274F,  "permittivity (farad per metre)"                          },
	{0x2750,  "permeability (henry per metre)"                          },
	{0x2751,  "molar energy (joule per mole)"                           },
	{0x2752,  "molar entropy (joule per mole kelvin)"                   },
	{0x2753,  "exposure (coulomb per kilogram)"                         },
	{0x2754,  "absorbed dose rate (gray per second)"                    },
	{0x2755,  "radiant intensity (watt per steradian)"                  },
	{0x2756,  "radiance (watt per square meter steradian)"              },
	{0x2757,  "catalytic activity concentration (katal per cubic metre)"},
	{0x2760,  "time (minute)"                                           },
	{0x2761,  "time (hour)"                                             },
	{0x2762,  "time (day)"                                              },
	{0x2763,  "plane angle (degree)"                                    },
	{0x2764,  "plane angle (minute)"                                    },
	{0x2765,  "plane angle (second)"                                    },
	{0x2766,  "area (hectare)"                                          },
	{0x2767,  "volume (litre)"                                          },
	{0x2768,  "mass (tonne)"                                            },
	{0x2780,  "pressure (bar)"                                          },
	{0x2781,  "pressure (millimetre of mercury)"                        },
	{0x2782,  "length (angstrom)"                                       },
	{0x2783,  "length (nautical mile)"                                  },
	{0x2784,  "area (barn)"                                             },
	{0x2785,  "velocity (knot)"                                         },
	{0x2786,  "logarithmic radio quantity (neper)"                      },
	{0x2787,  "logarithmic radio quantity (bel)"                        },
	{0x27A0,  "length (yard)"                                           },
	{0x27A1,  "length (parsec)"                                         },
	{0x27A2,  "length (inch)"                                           },
	{0x27A3,  "length (foot)"                                           },
	{0x27A4,  "length (mile)"                                           },
	{0x27A5,  "pressure (pound-force per square inch)"                  },
	{0x27A6,  "velocity (kilometre per hour)"                           },
	{0x27A7,  "velocity (mile per hour)"                                },
	{0x27A8,  "angular velocity (revolution per minute)"                },
	{0x27A9,  "energy (gram calorie)"                                   },
	{0x27AA,  "energy (kilogram calorie)"                               },
	{0x27AB,  "energy (kilowatt hour)"                                  },
	{0x27AC,  "thermodynamic temperature (degree Fahrenheit)"           },
	{0x27AD,  "percentage"                                              },
	{0x27AE,  "per mille"                                               },
	{0x27AF,  "period (beats per minute)"                               },
	{0x27B0,  "electric charge (ampere hours)"                          },
	{0x27B1,  "mass density (milligram per decilitre)"                  },
	{0x27B2,  "mass density (millimole per litre)"                      },
	{0x27B3,  "time (year)"                                             },
	{0x27B4,  "time (month)"                                            },
	
};


//GATT Server
BTSDKHANDLE local_svr_hdl = BTSDK_INVALID_HANDLE;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------------------------------------------*/

char *GetNameByUUID(BTUINT16 UUID)
{
	int i;
	for (i=0; i<dim(g_gatt_uuid_list); i++)
	{
		if (UUID == g_gatt_uuid_list[i].uuid)
		{
			return g_gatt_uuid_list[i].name;
		}
	}
	return "Unknown_UUID";
}

void ClearResources()
{
	memset(svc, 0, sizeof(BtsdkGATTServiceStru)*BUFF_NUM);
	memset(crt, 0, sizeof(BtsdkGATTCharacteristicStru)*BUFF_NUM);
	memset(dsc, 0, sizeof(BtsdkGATTDescriptorStru)*BUFF_NUM);

	rmt_ble_dev = BTSDK_INVALID_HANDLE;
	ev_hdl = BTSDK_INVALID_HANDLE;

	svc_num = 0;
	crt_num = 0;
	dsc_num = 0;
}

BTUINT16 FormateUUIDToBLEAssignedNum(BTUINT8* uuid, BTUINT16 uuid_len)
{
	BTUINT16 ret = 0;
	if (uuid_len != 16
		|| uuid == NULL)
	{
		return 0;
	}
 	ret = uuid[3];
 	ret = ret << 8;
 	ret = ret + uuid[2];
 	return ret;
}

void GATTShowMenu(void)
{
	printf("\n");
	printf("****************************************\n");
	printf("*         GATT Testing Menu            *\n");
	printf("* <1> Get Services                     *\n");
	printf("* <2> Get Characteristics              *\n");
	printf("* <3> Get Characteristics Value        *\n");
	printf("* <4> Set Characteristics Value        *\n");
	printf("* <5> Get Descriptors                  *\n");
	printf("* <6> Get Descriptor value             *\n");
	printf("* <7> Set Descriptor value             *\n");
	printf("* <8> Register Event                   *\n");
	printf("* <9> Unregister Event                 *\n");
	printf("* <a> End Session                      *\n");
	printf("* <b> Get Appearance                   *\n");
	//printf("* <c> Reg Tree                         *\n");
	printf("* <r> Return to upper menu             *\n");
	printf("****************************************\n");
	printf(">");
}

void TestSelectRmtBLEDev(int num)
{   
    rmt_ble_dev = SelectRemoteLEDevice(num);
	if (BTSDK_INVALID_HANDLE == rmt_ble_dev)
	{
		printf("Please make sure that the expected device \
				is in discoverable state and search again.\n");
	}
}

int ShowGATTServices()
{
	int i;
	if (svc_num <= 0)
	{
		printf("there is no services\n");
		return svc_num;
	}
	else
	{
		printf("servies assigned number:\n", svc_num);
		for (i=0; i<svc_num; i++)
		{
			BTUINT16 assigned_num = 0;
			if (svc[i].ServiceUuid.IsShortUuid)
			{
				assigned_num = svc[i].ServiceUuid.ShortUuid;
			}
			else
			{
				assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&(svc[i].ServiceUuid.LongUuid), 16);
			}
			printf("    %d    [UUID:0x%04X, NAME:%s]\n", i+1, assigned_num, GetNameByUUID(assigned_num));
		}
	}
	return svc_num;
}

int ShowGATTCharacteristics()
{
	int i;
	if (crt_num <= 0)
	{
		printf("there is no characteristics\n");
	}
	else
	{
		printf("characteristics assigned number:\n", crt_num);
		for (i=0; i<crt_num; i++)
		{
			BTUINT16 assigned_num = 0;
			if (crt[i].CharacteristicUuid.IsShortUuid)
			{
				assigned_num = crt[i].CharacteristicUuid.ShortUuid;
			}
			else
			{
				assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&(crt[i].CharacteristicUuid.LongUuid), 16);
			}
			printf("    %d    [UUID:0x%04X, NAME:%s]\n", i+1, assigned_num, GetNameByUUID(assigned_num));
		}
	}
	return crt_num;
}

int ShowGATTDescriptors()
{
	int i;
	if (dsc_num <= 0)
	{
		printf("there is no descriptors\n");
	}
	else
	{
		printf("descriptors assigned number:\n", dsc_num);
		for (i = 0; i<dsc_num; i++)
		{
 			BTUINT16 assigned_num = 0;
			if (dsc[i].DescriptorUuid.IsShortUuid)
			{
				assigned_num = dsc[i].DescriptorUuid.ShortUuid;
			} 
			else
 			{
				assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&dsc[i].DescriptorUuid.LongUuid, 16);
			}
			printf("    %d    [UUID:0x%04X, NAME:%s]\n", i+1, assigned_num, GetNameByUUID(assigned_num));
		}
	}
	return dsc_num;
}


void GATTGetServices()
{
	BTUINT16 num = 0;
	//rmt_ble_dev = SelectRemoteDevice(0);
	Btsdk_GATTGetServices(rmt_ble_dev, BUFF_NUM, svc, &num, BTSDK_GATT_FLAG_NONE);
	svc_num = num;
	//printf("get services number: %d\n", num);
	//ShowGATTServices();
}


void GATTGetCharacteristics()
{
	BTUINT16 num = 0;
	BTUINT32 ch = 0;
	int sel;
	/*if (//ShowGATTServices() <=0)
	/{
		return;
	}*/
SELECT_ID:
	//printf("please select a service by type the service's id\n");
	//AppWaitForInputInt(&ch);
	ch=5;
	sel = (int)(ch - 1);
	if (sel >= svc_num || sel < 0)
	{
		printf("%c is a wrong service's id\n", ch);
		goto SELECT_ID;
	}
	Btsdk_GATTGetCharacteristics(rmt_ble_dev, &svc[sel], BUFF_NUM, crt, &num, BTSDK_GATT_FLAG_NONE);
	crt_num = num;
	//printf("get characteristics number: %d\n", num);
	//ShowGATTCharacteristics();
}


int sel_crt;
static BTUINT8 s_Month[12][10] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
static BTUINT8 s_Week[7][10] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

void GATTGetCharacteristicsValue()
{
	BTUINT16 num = 0;
	BTUINT16 size = 0;
	BTUINT32 ch = 0;
	int i;
	int sel;
	PBtsdkGATTCharacteristicValueStru val = NULL;
	if (ShowGATTCharacteristics() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a character by type the character's id\n");
	AppWaitForInputInt(&ch);

	sel  = (int)(ch - 1);
	if (sel >= crt_num || sel < 0)
	{
		printf("%c is a wrong character's id\n", ch);
		goto SELECT_ID;
	}	
	Btsdk_GATTGetCharacteristicValue(rmt_ble_dev, &crt[sel], 0, NULL, &num, BTSDK_GATT_FLAG_NONE);	
	if (num <= 0)
	{
		printf("get characteristics value size is %d\n", num);
		return;
	}
	val = (PBtsdkGATTCharacteristicValueStru)malloc(num);
	memset(val, 0, num);
	Btsdk_GATTGetCharacteristicValue(rmt_ble_dev, &crt[sel], num, val, &size, BTSDK_GATT_FLAG_NONE);
	if (val != NULL)
	{
		if (val->DataSize > 0)
		{
			printf("get characteristics value data size is %d\n", val->DataSize);
			printf("get characteristics value data is ");
			for (i=0; i<val->DataSize; i++) 
			{
				printf("0x%02x ", val->Data[i]);
			}
			printf("\n");
			if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A2B) //Current Time
			{
				//Parse Current Time
				INT OffSet = 0;
				char *pTmp = val->Data;
				INT iType = 0;
				BTUINT16 year = 0;
				BTUINT8 month = 0;
				BTUINT8 day = 0;
				BTUINT16 hours = 0;
				BTUINT8 minutes = 0;
				BTUINT8 seconds = 0;
				BTUINT8 dayofWeek = 0;
				BTUINT8 Fraction256 = 0;
				BTUINT8 adjustReason = 0;
				while (OffSet < val->DataSize && iType < 9)
				{
					switch(iType)
					{
					case 0:// Year
						{
							year = *((BTUINT16*)pTmp);
							pTmp += 2;
							iType++;
							break;
						}
					case 1: //Month
						{
							month = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					case 2://Day
						{
							day = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					case 3://Hours
						{
							hours = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					case 4://Minutes
						{
							minutes = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					case 5://Seconds
						{
							seconds = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					case 6://Seconds
						{
							dayofWeek = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					case 7://Seconds
						{
							Fraction256 = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					case 8://Seconds
						{
							adjustReason = *((BTUINT8*)pTmp);
							pTmp++;
							iType++;
							break;
						}
					default:
						break;
					}
				}
				printf("Current time is: %s %d. %d %s %02d:%02d:%02d\n", s_Month[month-1], day, year, s_Week[dayofWeek-1], hours, minutes, seconds);
			}
			else if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A35)//BloodPressureMeasurement
			{
				GetBloodPressureMeasurement(val->Data);
			}
			else if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A18) //G
			{
				GetGlucoseMeasurement(val->Data);
			}
			printf("\n");
		}
		else
		{
			printf("get characteristics value size is %d\n", val->DataSize);
		}
	}
	free(val);
}

void GATTSetCharacteristicsValue()
{
	BTUINT32 ch = 0;
	PBtsdkGATTCharacteristicValueStru val;
	BTINT32 ret;
	int sel;
	int data_num =  0;
	BTUINT8 data[32] = {0};
	INT index = 0;
	INT characterValue = 0;
	FILE *stream;
	char line[100];
	val = (PBtsdkGATTCharacteristicValueStru)malloc(sizeof(BtsdkGATTCharacteristicValueStru)+100);
	memset(val, 0, sizeof(BtsdkGATTCharacteristicValueStru)+100);
	/*if (ShowGATTCharacteristics() <= 0)
	{
		return;
	}*/
SELECT_ID:
	//printf("please select a character by type the character's id\n");
	//AppWaitForInputInt(&ch);
	ch=2;
	sel  = (int)(ch - 1);
	if (sel >= crt_num || sel < 0)
	{
		printf("%c is a wrong character's id\n", ch);
		goto SELECT_ID;
	}
	//printf("please enter the characteristics value data length\n");
	//scanf("%x", &ch);
	data_num = ch;
	if (data_num >= 32)
	{
		goto SELECT_ID;
	}
	index = 0;
	//ect. 0x01 0x02...
	/*printf("please enter the characteristics value\n");
	while(index < data_num && 1 == scanf("%x", &characterValue))
	{
		data[index] = (BTUINT8)characterValue;
		index++;
	}*/
	

	if( fopen_s( &stream, "c:\\VirtualRealty\\BluetoothRead\\location.txt", "r" ) == 0 )
	{
		if( fgets( line, 100, stream ) == NULL)
			printf( "fgets error\n" );
		else
			strcpy(data,line);
		fclose( stream );
	}
	
	val->DataSize = strlen(data);
	memcpy(val->Data, data, val->DataSize);
	ret = Btsdk_GATTSetCharacteristicValue(rmt_ble_dev, &crt[sel], val, BTSDK_INVALID_HANDLE, BTSDK_GATT_FLAG_NONE);
	if (ret != BTSDK_OK)
	{
		printf("set characteristics value fail\n");
	}
	else
	{
		printf("set characteristics value ok\n");
	}
	free(val);
}

void GATTGetDescriptors()
{
	BTUINT16 num = 0;
	BTUINT32 ch = 0;
	int sel;
	if (ShowGATTCharacteristics() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a character by type the character's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= crt_num || sel < 0)
	{
		printf("%c is a wrong character's id\n", ch);
		goto SELECT_ID;
	}
	Btsdk_GATTGetDescriptors(rmt_ble_dev, &crt[sel], BUFF_NUM, dsc, &num, BTSDK_GATT_FLAG_NONE);
	dsc_num = num;
	printf("get descriptors number: %d\n", num);
	ShowGATTDescriptors();	
}

void GATTGetDescriptorValue()
{
	BTUINT32 ch = 0;
	BTUINT16 size_req;
	int sel;
	PBtsdkGATTDescriptorValueStru val = NULL;
	UCHAR str[256];
	BtsdkGATTDescriptorValueStru descriptorValue = {0};
	BTUINT16 descriptorValueSize = sizeof(BtsdkGATTDescriptorValueStru);
	BTINT32 ret = BTSDK_OK;
		
	memset(str, 0, 256);
	if (ShowGATTDescriptors() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a descriptor by type the descriptor's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= dsc_num || sel < 0)
	{
		printf("%c is a wrong descriptor's id\n", ch);
		goto SELECT_ID;
	}
	ret = Btsdk_GATTGetDescriptorValue(rmt_ble_dev, &dsc[sel], descriptorValueSize, (PBtsdkGATTDescriptorValueStru)&descriptorValue, &size_req, BTSDK_GATT_FLAG_NONE);
	if (BTSDK_ER_BUFFER_NOT_ENOUGH == ret)
	{
		val = (PBtsdkGATTDescriptorValueStru)malloc(size_req);
		if (val != NULL)
		{
			memset(val, 0, size_req);
			ret = Btsdk_GATTGetDescriptorValue(rmt_ble_dev, &dsc[sel], size_req, val, &size_req, BTSDK_GATT_FLAG_NONE);
		}
	}
	else if (BTSDK_OK == ret)
	{
		val = (PBtsdkGATTDescriptorValueStru)malloc(sizeof(BtsdkGATTDescriptorValueStru));
		if (val != NULL)
		{
			memset(val, 0, sizeof(BtsdkGATTDescriptorValueStru));
			memcpy(val, (&descriptorValue), sizeof(BtsdkGATTDescriptorValueStru));
		}
    }

	if (ret == BTSDK_OK)
	{
		switch (val->DescriptorType)
		{
		case CharacteristicExtendedProperties:
			printf("RelW = %d, AuxW = %d\n", val->CharacteristicExtendedProperties.IsReliableWriteEnabled, 
				val->CharacteristicExtendedProperties.IsAuxiliariesWritable);
			break;
		case CharacteristicFormat:
			sprintf(&str[strlen(str)],"For:0x2X, Exp:0x2X, NS:0x2X", val->CharacteristicFormat.Format, 
				val->CharacteristicFormat.Exponent, val->CharacteristicFormat.NameSpace);		
			printf("%s\n", str);
			break;
		case ClientCharacteristicConfiguration:
			printf("Noti:%d, Indi:%d\n", val->ClientCharacteristicConfiguration.IsSubscribeToNotification,
				val->ClientCharacteristicConfiguration.IsSubscribeToIndication);
			break;
		case ServerCharacteristicConfiguration:
			printf("Broad:%d\n", val->ServerCharacteristicConfiguration.IsBroadcast);
			break;		
		}
	}

	if (val != NULL)
	{
		free(val);
	}
}

void GATTSetDescriptorValue()
{
	BTUINT32 ch = 0;
	int sel;
	int cfg;
	BtsdkGATTDescriptorValueStru val;
	BTINT32 ret;
	
	memset(&val, 0, sizeof(BtsdkGATTDescriptorValueStru));
	if (ShowGATTDescriptors() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a descriptor by type the descriptor's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= dsc_num || sel < 0)
	{
		printf("%c is a wrong descriptor's id\n", ch);
		goto SELECT_ID;
	}
	printf("please select client characteristic configuration\n0  notification\n1  indication\n");
	cfg = AppWaitForInputInt(NULL);

	val.DescriptorType = ClientCharacteristicConfiguration;
	if (cfg == 0)
	{
		val.ClientCharacteristicConfiguration.IsSubscribeToNotification = 1;
	}
	else if (cfg == 1)
	{
		val.ClientCharacteristicConfiguration.IsSubscribeToIndication = 1;
	}
	else
	{
		printf("select wrong\n");
		return;
	}
	ret = Btsdk_GATTSetDescriptorValue(rmt_ble_dev, &dsc[sel], &val, BTSDK_GATT_FLAG_NONE);
	if (ret != BTSDK_OK)
	{
		ret = Btsdk_GATTSetDescriptorValue(rmt_ble_dev, &dsc[sel], &val, BTSDK_GATT_FLAG_NONE|BTSDK_GATT_FLAG_CONNECTION_ENCRYPTED);
		if (ret != BTSDK_OK)
		{
			printf("set descriptor value fail\n");
			return;
		}		
	}
	printf("set descriptor value ok\n");
}

void GATTEndSession()
{
	if (rmt_ble_dev == BTSDK_INVALID_HANDLE)
	{
		printf("Do not select a device before\n");
		return;
	}
	Btsdk_GATTCloseSession(rmt_ble_dev, BTSDK_GATT_FLAG_NONE);
	printf("OK!\n");
}

void GATTGetAppearance()
{
	BTUINT16 appearance = 0;
	if (rmt_ble_dev == BTSDK_INVALID_HANDLE)
	{
		printf("Do not select a device before\n");
		return;
	}
	//Get the appearace from GATT_APPEARANCE_CATEGORY_XXX
	if (BTSDK_OK == Btsdk_GetLEDeviceAppearance(rmt_ble_dev, &appearance))
	{
		printf("Get remote device appearace succeed, appearace is 0x%x\n", appearance);
	}
	else
	{
		printf("Get remote device appearace failed!\n");
	}
}

void GetBloodPressureMeasurement(BTUCHAR *p)
{
	BTUINT16 systolic = 0;
	BTUINT16 disatolic = 0;
	BTUINT16 mean_artrrial = 0;
	BTUCHAR systolic_str[24] = {0};
	BTUCHAR disatolic_str[24] = {0};
	BTUCHAR mean_artrrial_str[24] = {0};
	
	systolic = *(p + 1) * 256 + *(p + 2);
	disatolic = *(p + 3) * 256 + *(p + 4);
	mean_artrrial = *(p + 5) * 256 + *(p + 6);

	SFloatToString(systolic, systolic_str);
	SFloatToString(disatolic, disatolic_str);
	SFloatToString(mean_artrrial, mean_artrrial_str);

	if(*p & 1)
	{
		printf("Value-systolic:%skPa\nValue-disatolic:%skPa\nValue-mean_artrrial:%sKpa\n", systolic_str, disatolic_str, mean_artrrial_str);
	}
	else
	{
		printf("Value-systolic:%smmhg\nValue-disatolic:%smmhg\nValue-mean_artrrial:%smmhg\n", systolic_str, disatolic_str, mean_artrrial_str);
	}
}	

void GetGlucoseMeasurement(BTUCHAR *p)
{
	BTUINT32 year;
	BTUINT16 glucose;
	BTUCHAR glucose_str[24] = {0};
	int i =0;
	
	if(*p & 1)	
	{
		i+=2;//c1 sint16
	}
	if(*p & 2)
	{
		i+=2;//c2 sfloat 
	}
	year=*(p + 3) * 256 + *(p + 4);
	glucose=*(p + 10 + i) * 256 + *(p + 11 + i);
	SFloatToString(glucose, glucose_str);

	printf("%04d/%02d/%02d %02d:%02d:%02d \t", year, *(p + 5), *(p + 6), *(p + 7), *(p + 8), *(p + 9));
	if(*p & 4)
	{
		printf("Glucose is: %smol/L!", glucose_str);
	}
	else
	{
		printf("Glucose is: %skg/L!", glucose_str);
	}

}

void GATTValueCbk(BTUINT16 ChangedAttributeHandle, BTUINT32 CharacteristicValueDataSize, PBtsdkGATTCharacteristicValueStru CharacteristicValue, BTLPVOID Context)
{
	BTUCHAR *p;
	INT temp = 0;
	printf("\r\n[HdlValCbk][Hdl:0x%x][Len:%d][", ChangedAttributeHandle, CharacteristicValue->DataSize);

	p = CharacteristicValue->Data;	
	while (CharacteristicValue->DataSize--) {
			printf("0x%02x ", *p++);
	}
	printf("\b]\r\n");	

	if (crt[0].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[0].CharacteristicUuid.ShortUuid == 0x2A2B) //Current Time
	{
		//Parse Current Time
		INT OffSet = 0;
		char *pTmp = CharacteristicValue->Data;
		INT iType = 0;
		BTUINT16 year = 0;
		BTUINT8 month = 0;
		BTUINT8 day = 0;
		BTUINT16 hours = 0;
		BTUINT8 minutes = 0;
		BTUINT8 seconds = 0;
		BTUINT8 dayofWeek = 0;
		BTUINT8 Fraction256 = 0;
		BTUINT8 adjustReason = 0;
		while (OffSet < CharacteristicValue->DataSize && iType < 9)
		{
			switch(iType)
			{
			case 0:// Year
				{
					year = *((BTUINT16*)pTmp);
					pTmp += 2;
					iType++;
					break;
				}
			case 1: //Month
				{
					month = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			case 2://Day
				{
					day = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			case 3://Hours
				{
					hours = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			case 4://Minutes
				{
					minutes = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			case 5://Seconds
				{
					seconds = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			case 6://Seconds
				{
					dayofWeek = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			case 7://Seconds
				{
					Fraction256 = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			case 8://Seconds
				{
					adjustReason = *((BTUINT8*)pTmp);
					pTmp++;
					iType++;
					break;
				}
			default:
				break;
			}
		}
		printf("Current time is: %s %d. %d %s %02d:%02d:%02d\n", s_Month[month-1], day, year, s_Week[dayofWeek-1], hours, minutes, seconds);
	}
	else if (crt[0].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[0].CharacteristicUuid.ShortUuid == 0x2A35) //BloodPressureMeasurement
	{
		GetBloodPressureMeasurement(CharacteristicValue->Data);
	}
	else if (crt[0].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[0].CharacteristicUuid.ShortUuid == 0x2A18) //GlucoseMeasurement
	{
		GetGlucoseMeasurement(CharacteristicValue->Data);
	}
}

void GATTRegisterEvent()
{
	BTUINT32 ch = 0;
	int sel;
	BTINT32 ret;
	if (rmt_ble_dev == BTSDK_INVALID_HANDLE)
	{
		printf("Do not select a device before\n");
		return;
	}
	if (ShowGATTCharacteristics() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a character by type the character's id\n");
	AppWaitForInputInt(&ch);

	sel  = (int)(ch - 1);
	if (sel >= crt_num || sel < 0)
	{
		printf("%c is a wrong characteristic's id\n", ch);
		goto SELECT_ID;
	}
	
	if (ev_hdl == BTSDK_INVALID_HANDLE)
	{
		ret = Btsdk_GATTRegisterEvent(rmt_ble_dev, CharacteristicValueChangedEvent, &crt[sel], GATTValueCbk, NULL, &ev_hdl, BTSDK_GATT_FLAG_NONE);
		if (ret != BTSDK_OK)
		{
			printf("register event fail\n");
		}
		else
		{
			printf("register event ok\n");
		}
	}
	else
	{
		printf("have register event\n");
	}
}

 void GATTUnregisterEvent()
 {
	if (ev_hdl == BTSDK_INVALID_HANDLE)
	{
		printf("Do not register event before\n"); 
		return;
	}
 	Btsdk_GATTUnregisterEvent(ev_hdl, BTSDK_GATT_FLAG_NONE);
	ev_hdl = BTSDK_INVALID_HANDLE;
 	printf("OK!\n");
 }

 
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------------------------------------------*/
void GATTExecCmd(BTUINT8 choice)
{
	switch (choice) {
	case '1':
		GATTGetServices();
		break;
	case '2':
		 GATTGetCharacteristics();
		break;
	case '3':
		 GATTGetCharacteristicsValue();
		break;
	case '4':
		 GATTSetCharacteristicsValue();
		break;
	case '5':
		 GATTGetDescriptors();
		break;
	case '6':
		 GATTGetDescriptorValue();
		break;
	case '7':
		 GATTSetDescriptorValue();
		 break;
	case '8':
		GATTRegisterEvent();
		break;
	case '9':
		GATTUnregisterEvent();
		break;
	case 'a':
		 GATTEndSession();
		break;
	case 'b':
		GATTGetAppearance();
		break;
	case 'c':
		// GATTRegTree();
		break;
	case 'r':
		{
			if (ev_hdl != BTSDK_INVALID_HANDLE)
			{
				Btsdk_GATTUnregisterEvent(ev_hdl, BTSDK_GATT_FLAG_NONE);
				ev_hdl = BTSDK_INVALID_HANDLE;
			}
			ClearResources();
		}
		break;
	default:
		printf("*Invalid command!");
		break;
	}
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------------------------------------------*/
void TestGATT(void)
{
	BTUINT8 ch = 0;
	TestSelectRmtBLEDev(1);
	GATTShowMenu();
	while (ch != 'r')	
	{
		AppWaitForInput(&ch, 1);

		if (ch == '\n')
		{
			printf(">");
		} 
		else
		{
			GATTExecCmd(ch);
			printf("\n");
			if (ch != 'r')
			{
				GATTShowMenu();
			}
		}
	}
}

void GetAllRmtLEDevHdl()
{
	BTSDKHANDLE hEnumHdl = BTSDK_INVALID_HANDLE;
	INT i = 0;
	BTDEVHDL dev_hdl = BTSDK_INVALID_HANDLE;
	s_rmt_le_dev_num = 0;
	memset(s_rmt_le_dev_hdls, 0, sizeof(s_rmt_le_dev_hdls));
	
	hEnumHdl = Btsdk_StartEnumRemoteDevice(BTSDK_ERD_FLAG_NOLIMIT, 0);
	if (BTSDK_INVALID_HANDLE != hEnumHdl)
	{		
		while (BTSDK_INVALID_HANDLE != (dev_hdl = Btsdk_EnumRemoteDevice(hEnumHdl, NULL)))
		{
			BTINT32 dev_type = Btsdk_GetRemoteDeviceType(dev_hdl);
			if (dev_type == BTSDK_DEV_TYPE_LE_ONLY || dev_type == BTSDK_DEV_TYPE_BREDR_LE)
			{
				if (s_rmt_le_dev_num >= MAX_DEV_NUM)
				{
					break;
				}
				s_rmt_le_dev_hdls[s_rmt_le_dev_num] = dev_hdl;
				s_rmt_le_dev_num++;
			}
		}
		Btsdk_EndEnumRemoteDevice(hEnumHdl);
		hEnumHdl = BTSDK_INVALID_HANDLE;
	}
}

HANDLE s_hBrowseLEDevEventHdl = INVALID_HANDLE_VALUE;
void AppInquiryLEDevInd(BTDEVHDL dev_hdl)
{
	BTUINT32 devType = Btsdk_GetRemoteDeviceType(dev_hdl);
	if (devType == BTSDK_DEV_TYPE_LE_ONLY || devType == BTSDK_DEV_TYPE_BREDR_LE)
	{
		if (!IsDeviceHdlInList(dev_hdl) && s_rmt_le_dev_num < MAX_DEV_NUM)
		{
			s_rmt_le_dev_hdls[s_rmt_le_dev_num++] = dev_hdl;
		}
	}
}

void StartSearchLEDevice()
{
	BtSdkCallbackStru cb = {0};
	HANDLE hEvent = NULL;
	memset(s_rmt_le_dev_hdls, 0, sizeof(s_rmt_le_dev_hdls));
	s_rmt_le_dev_num = 0;
	
	s_hBrowseLEDevEventHdl = CreateEvent(NULL, FALSE, FALSE, "completeBrowseLEDevice");
	printf("Please wait for a while searching for remote devices......\n");

	cb.type = BTSDK_DEVICE_FOUND_IND;
	cb.func = (void*)AppInquiryLEDevInd;
	Btsdk_RegisterCallback4ThirdParty(&cb);

	Btsdk_LESetScanEnable(BTSDK_LE_GENERAL_DISCOVERABLE, BTSDK_LE_ADV_SCAN_FAST);
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 10000); //Change the wait time to scan more LE device here.
	CloseHandle(hEvent);
	Btsdk_LESetScanEnable(BTSDK_LE_NONE_DISCOVERABLE, BTSDK_LE_ADV_SCAN_FAST);
	if (s_rmt_le_dev_num < MAX_DEV_NUM)
	{
		BTSDKHANDLE enum_handle = BTSDK_INVALID_HANDLE;
		enum_handle = Btsdk_StartEnumRemoteDevice(BTSDK_ERD_FLAG_NOLIMIT, 0);
		if (enum_handle != BTSDK_INVALID_HANDLE)
		{
			BTDEVHDL dev_hdl = BTSDK_INVALID_HANDLE;
			while (BTSDK_INVALID_HANDLE != (dev_hdl = Btsdk_EnumRemoteDevice(enum_handle, NULL)))
			{
				BTUINT32 dev_type = Btsdk_GetRemoteDeviceType(dev_hdl);
				if (dev_type == BTSDK_DEV_TYPE_BREDR_LE || dev_type == BTSDK_DEV_TYPE_LE_ONLY)
				{
					if (!IsDeviceHdlInList(dev_hdl) && s_rmt_le_dev_num < MAX_DEV_NUM)
					{
						s_rmt_le_dev_hdls[s_rmt_le_dev_num] = dev_hdl;
						s_rmt_le_dev_num++;
					}
				}
			}

			Btsdk_EndEnumRemoteDevice(enum_handle);
		}
	}

	DisplayRemoteLEDevices();
	printf("Search for remote devices completely.\n");
	
	
	cb.type = BTSDK_DEVICE_FOUND_IND;
	cb.func = (void*)NULL;
	Btsdk_RegisterCallback4ThirdParty(&cb);
}

void DisplayRemoteLEDevices()
{
	int i = 0;
	int j = 0;
	BTUINT8 szDevName[BTSDK_DEVNAME_LEN] = { 0 };
	BTUINT8 szTmp[32] = { 0 };
	BTUINT16 usLen = 0;
	BTUINT32 ulDevClass = 0;
	BTUINT8 szBdAddr[BD_ADDR_LEN] = {0};
	char cQuote = ' ';
	char temp[20];

	//printf("Remote devices searched:\n");
	//printf("number  device name %16hc device address %4hc\n", cQuote, cQuote);

	for(i = 0;i<100;i++)
	{
		strcpy(devs[i],"");
	}
	for (i = 0; i < s_rmt_le_dev_num; i++)
	{
		/*In order to display neatly.*/
		if (i<9)
		{
			//printf("  %d%5hc", i + 1, cQuote);
		}
		else
		{
			//printf("  %d%4hc", i + 1, cQuote);
		}
		
		usLen = 32;
		memset(szDevName, 0, sizeof(szDevName));
		/*if (Btsdk_GetRemoteDeviceName(s_rmt_le_dev_hdls[i], szDevName, &usLen) != BTSDK_OK)
		{
			strcpy((char*)szDevName, "Unknown");
		}*/
		strcpy(szTmp, szDevName);
		MultibyteToMultibyte(CP_UTF8, szTmp, -1, CP_ACP, szDevName, BTSDK_DEVNAME_LEN);
		//printf("%-29hs", szDevName);		
		Btsdk_GetRemoteDeviceAddress(s_rmt_le_dev_hdls[i], szBdAddr);
		for(j = 5; j > 0; j --)
		{
			//printf("%02X:", szBdAddr[j]);
			sprintf(temp,"%02X:",szBdAddr[j]);
			strcat(devs[i],temp);
		}
		//printf("%02X%3hc\n", szBdAddr[0], cQuote);
		sprintf(temp,"%02X",szBdAddr[0]);
		strcat(devs[i],temp);
		//printf("%s\n",devs[i]);
	}	

}

BTDEVHDL SelectRemoteLEDevice(int num)
{
	BTDEVHDL retDevHdl = BTSDK_INVALID_HANDLE;
	int nIdx = 0;
	char szChoice[4] = {0};
	char i;
	char choice=0;
	GetAllRmtLEDevHdl();
	/*show remote devices. If there is no device shown, search for them at first*/
	if (0 == s_rmt_le_dev_num)
	{
		StartSearchLEDevice();
	}
	else
	{
		DisplayRemoteLEDevices();
	}
	
	//printf("Select the target device :\n"); 
	//printf("if there is no expected device, please press 'a' to search again!\n");
	//printf("if you want to exit this procedure, please press 'q' to quit.\n");
	
	do
	{
		//printf("Target device number = ");
		//AppWaitForInput(szChoice, 4);
		for(i = 0; i<100; i++)
		{
			if(num==1)
			{
				if(strcmp(devs[i],"D9:50:3E:58:AC:3C")==0)
				{
					choice=i;
					break;

				}
				
			}
			if(num==2)
			{
				if(strcmp(devs[i],"F0:7A:40:64:A0:C5")==0)
				{
					choice=i;
					break;

				}
				
			}

		}
		//printf("Num: %d Choice %d\n",num,choice);

		szChoice[0]='0'+(char)(choice+1);
		if ('a' == szChoice[0])
		{
			StartSearchLEDevice();		
			continue;
		}
		if(('q' == szChoice[0]) || ('Q' == szChoice[0]))
		{
			printf("\nUser abort the operation.\n");
			return BTSDK_INVALID_HANDLE;
		}
		nIdx = atoi(szChoice);
		if((nIdx <= 0) || (nIdx > s_rmt_le_dev_num))
		{
			printf("%d is not a valid datum, please select again.\n", nIdx);
			continue;
		}
		else
		{
			break;
		}
	} while (1);
	
	return (s_rmt_le_dev_hdls[nIdx - 1]);
}

#define FMTBD2STR(bd) TEXT("%02X:%02X:%02X:%02X:%02X:%02X"), bd[5],bd[4],bd[3],bd[2],bd[1],bd[0]
void AppConnectionCompleteCbk(BTDEVHDL dev_hdl)
{
	BTUINT8 szDevName[BTSDK_DEVNAME_LEN] = {0};
	BTUINT8 szTmp[BTSDK_DEVNAME_LEN] = {0};
	BTUINT16 usLen = BTSDK_DEVNAME_LEN;
	BTUINT8 szBdAddr[BTSDK_BDADDR_LEN] = {0};
	BTINT32 devType = 0;
	char szDeviceAddress[MAX_PATH] = {0};
	if (Btsdk_GetRemoteDeviceName(dev_hdl, szDevName, &usLen) != BTSDK_OK)
	{
		strcpy((char*)szDevName, "Unknown");
	}		
	strcpy(szTmp, szDevName);
	MultibyteToMultibyte(CP_UTF8, szTmp, -1, CP_ACP, szDevName, BTSDK_DEVNAME_LEN);
	Btsdk_GetRemoteDeviceAddress(dev_hdl, szBdAddr);
	sprintf(szDeviceAddress, FMTBD2STR(szBdAddr));
//	printf("\nLocal device has established the ACL connection with %s %s\n", szDevName, szDeviceAddress);
	devType = Btsdk_GetRemoteDeviceType(dev_hdl);
	if (devType == BTSDK_DEV_TYPE_LE_ONLY || devType == BTSDK_DEV_TYPE_BREDR_LE)
	{
		//When link lost we set scan to auto, after connection established, stop scan.
		if (s_LEScanAuto)	
		{
			Btsdk_LESetScanEnable(BTSDK_LE_GENERAL_DISCOVERABLE, BTSDK_LE_ADV_SCAN_AUTO);
			s_LEScanAuto = FALSE;
		}
	}
}

void AppDisconnectionCompleteCbk(BTDEVHDL dev_hdl, BTUINT32 reason)
{
	BTUINT8 szDevName[BTSDK_DEVNAME_LEN] = {0};
	BTUINT8 szTmp[BTSDK_DEVNAME_LEN] = {0};
	BTUINT16 usLen = BTSDK_DEVNAME_LEN;
	BTUINT8 szBdAddr[BTSDK_BDADDR_LEN] = {0};
	char szDeviceAddress[MAX_PATH] = {0};
	BTINT32 devType = 0;
	if (Btsdk_GetRemoteDeviceName(dev_hdl, szDevName, &usLen) != BTSDK_OK)
	{
		strcpy((char*)szDevName, "Unknown");
	}		
	strcpy(szTmp, szDevName);
	MultibyteToMultibyte(CP_UTF8, szTmp, -1, CP_ACP, szDevName, BTSDK_DEVNAME_LEN);
	Btsdk_GetRemoteDeviceAddress(dev_hdl, szBdAddr);
	sprintf(szDeviceAddress, FMTBD2STR(szBdAddr));
//	printf("\nLocal device has dropped the ACL connection with %s %s\n", szDevName, szDeviceAddress);
	if (reason == BTSDK_ER_CONNECTION_TIMEOUT)
	{
		//If the device is LinkLost and you wants to listen to the device's advertising. SetScan enable here.
		devType = Btsdk_GetRemoteDeviceType(dev_hdl);
		if (devType == BTSDK_DEV_TYPE_LE_ONLY || devType == BTSDK_DEV_TYPE_BREDR_LE)
		{
			Btsdk_LESetScanEnable(BTSDK_LE_GENERAL_DISCOVERABLE, BTSDK_LE_ADV_SCAN_AUTO);
			s_LEScanAuto = TRUE;
		}
	}
}

void AppDeviceFoundFuncCbk(BTDEVHDL dev_hdl)
{
	BTUINT32 devType = 0;
	BTUINT8 szDevName[BTSDK_DEVNAME_LEN] = {0};
	BTUINT8 szTmp[BTSDK_DEVNAME_LEN] = {0};
	BTUINT16 usLen = BTSDK_DEVNAME_LEN;
	BTUINT8 szBdAddr[BTSDK_BDADDR_LEN] = {0};
	char szDeviceAddress[MAX_PATH] = {0};
	devType = Btsdk_GetRemoteDeviceType(dev_hdl);
	if (devType == BTSDK_DEV_TYPE_LE_ONLY || devType == BTSDK_DEV_TYPE_BREDR_LE)
	{
		if (Btsdk_GetRemoteDeviceName(dev_hdl, szDevName, &usLen) != BTSDK_OK)
		{
			strcpy((char*)szDevName, "Unknown");
		}		
		strcpy(szTmp, szDevName);
		MultibyteToMultibyte(CP_UTF8, szTmp, -1, CP_ACP, szDevName, BTSDK_DEVNAME_LEN);
		Btsdk_GetRemoteDeviceAddress(dev_hdl, szBdAddr);
		sprintf(szDeviceAddress, FMTBD2STR(szBdAddr));
	//	printf("\nLE device %s %s is advertising.\n", szDevName, szDeviceAddress);
	}
}

BOOL IsDeviceHdlInList(BTDEVHDL dev_hdl)
{
	BOOL bRet = FALSE;
	INT i = 0;
	for (i=0; i<s_rmt_le_dev_num; i++)
	{
		if (dev_hdl == s_rmt_le_dev_hdls[i])
		{
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}


void GATTSvrSetLocalName()
{
	BTINT32 ret = BTSDK_ER_SDK_UNINIT;
	BTUINT8 device_name[BTSDK_DEVNAME_LEN] = {0};
	BTUINT16 appearance = 0;
	strcpy(device_name, "Virtual Realty Server");
	ret = Btsdk_GATTSetGapUIAttributes(device_name, GATT_APPEARANCE_CATEGORY_GENERIC_COMPUTER);
	if (ret != BTSDK_OK)
	{
		printf("GATT Server:Set local name and appearance failed.\r\n");
	}
}

void GATTSvrEnableAdvertising()
{
	BTINT32 ret = BTSDK_ER_SDK_UNINIT;
	ret = Btsdk_LESetAdvertisingEnable(BTSDK_LE_GENERAL_DISCOVERABLE, BTSDK_LE_ADV_SCAN_FAST);
	if (ret != BTSDK_OK)
	{
		printf("GATT Server:Enable advertising failed.\r\n");
	}
}

void GATTSvrDisableAdvertising()
{
	BTINT32 ret = BTSDK_ER_SDK_UNINIT;
	ret = Btsdk_LESetAdvertisingEnable(BTSDK_LE_NONE_DISCOVERABLE, BTSDK_LE_ADV_SCAN_FAST);
	if (ret != BTSDK_OK)
	{
		printf("GATT Server:Enable advertising failed.\r\n");
	}
	
}

BTBOOL HandleReadCharacteristicRequestEvent(BTLPVOID Context, BTSDK_GATT_EVENT_TYPE EventType, BTUINT32 RequestID, BTUINT32 EventParameterSize, PBtsdkGATTCharacteristicReadRequestStru characterReadReq)
{
	BTBOOL ret = BTSDK_TRUE;

	if (EventType == ReadCharacteristicRequestEvent && characterReadReq != NULL)
	{
		BTUINT16 assigned_num = 0;
		BTUINT8 bd_addr[BTSDK_BDADDR_LEN] = {0};
		char bd_address[MAX_PATH] = {0};
		Btsdk_GetRemoteDeviceAddress(characterReadReq->hDevice, bd_addr);
		sprintf(bd_address, FMTBD2STR(bd_addr));		

		if (characterReadReq->CharacteristicUuid.IsShortUuid)
		{
			assigned_num = characterReadReq->CharacteristicUuid.ShortUuid;
		}
		else
		{
			assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&(characterReadReq->CharacteristicUuid.LongUuid), 16);
		}
		if(strcmp(GetNameByUUID(assigned_num),"SystemID")==0)
		{
			BTUINT8 data[16] = {0};
			BTUINT32 data_size = 16;
			char* pTmp = data;
			int i;
			FILE *stream;
			char line[100];
			if( fopen_s( &stream, "c:\\Users\\nmemme\\Desktop\\commands.txt", "r" ) == 0 )
			{
				if( fgets( line, 100, stream ) == NULL)
					printf( "fgets error\n" );
				else
					printf( "%s\n", line);
				fclose( stream );
			}
			else
			{
				printf("Can't Open\n");
			}
			for(i=0;i<16;i++)
			{
				pTmp += sprintf(pTmp, "%c", line[i]);
			}
		   //system("dir c:\\Users\\nmemme\\Desktop");
		   Btsdk_GATTSendResponse(characterReadReq->hDevice, RequestID, BTSDK_OK, characterReadReq->Offset, data, data_size);
		   ret = BTSDK_FALSE;

		}
		printf("Device:%s is reading local characteristic [UUID:0x%04X, NAME:%s] \n", bd_address, assigned_num, GetNameByUUID(assigned_num));
			
		if (characterReadReq->CharacteristicUuid.IsShortUuid == BTSDK_TRUE && characterReadReq->CharacteristicUuid.ShortUuid == 0x2A2B)
		{
			BTUINT8 data[10] = {0};
			BTUINT32 data_size = 10;
			char* pTmp = data;
			SYSTEMTIME sysTime = {0};
			GetSystemTime(&sysTime);
			*((BTUINT16*)pTmp) = sysTime.wYear; //Year
			pTmp += 2;
			*((BTUINT8*)pTmp) = sysTime.wMonth; //Month
			pTmp++;
			*((BTUINT8*)pTmp) = sysTime.wDay; //Day
			pTmp++;
			*((BTUINT8*)pTmp) = sysTime.wHour + 8;//hours
			pTmp++;
			*((BTUINT8*)pTmp) = sysTime.wMinute; //Minute
			pTmp++;
			*((BTUINT8*)pTmp) = sysTime.wSecond;//Second
			pTmp++;
			*((BTUINT8*)pTmp) = sysTime.wDayOfWeek;
			Btsdk_GATTSendResponse(characterReadReq->hDevice, RequestID, BTSDK_OK, characterReadReq->Offset, data, data_size);
			ret = BTSDK_FALSE;
		}
	}
	return ret;
}

BTBOOL HandleWriteCharacteristicRequestEvent(BTLPVOID Context, BTSDK_GATT_EVENT_TYPE EventType, BTUINT32 RequestID, BTUINT32 EventParameterSize, PBtsdkGATTCharacteristicWriteRequestStru characterWriteReq)
{
	BTBOOL ret = BTSDK_TRUE;
	if (EventType == WriteCharacteristicRequestEvent && characterWriteReq != NULL)
	{
		BTUINT16 assigned_num = 0;
		BTUINT8 bd_addr[BTSDK_BDADDR_LEN] = {0};
		char bd_address[MAX_PATH] = {0};
		Btsdk_GetRemoteDeviceAddress(characterWriteReq->hDevice, bd_addr);
		sprintf(bd_address, FMTBD2STR(bd_addr));		

		if (characterWriteReq->CharacteristicUuid.IsShortUuid)
		{
			assigned_num = characterWriteReq->CharacteristicUuid.ShortUuid;
		}
		else
		{
			assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&(characterWriteReq->CharacteristicUuid.LongUuid), 16);
		}
		printf("Device:%s is writing local characteristic [UUID:0x%04X, NAME:%s] \n", bd_address, assigned_num, GetNameByUUID(assigned_num));
	}
	return BTSDK_TRUE;
}

BTBOOL HandleReadDescriptorRequestEvent(BTLPVOID Context, BTSDK_GATT_EVENT_TYPE EventType, BTUINT32 RequestID, BTUINT32 EventParameterSize, PBtsdkGATTDescriptorReadRequestStru descriptorReadReq)
{
	if (EventType == ReadDescriptorRequestEvent && descriptorReadReq != NULL)
	{

		BTUINT16 assigned_num = 0;
		BTUINT8 bd_addr[BTSDK_BDADDR_LEN] = {0};
		char bd_address[MAX_PATH] = {0};
		Btsdk_GetRemoteDeviceAddress(descriptorReadReq->hDevice, bd_addr);
		sprintf(bd_address, FMTBD2STR(bd_addr));

		if (descriptorReadReq->DescriptorUuid.IsShortUuid)
		{
			assigned_num = descriptorReadReq->DescriptorUuid.ShortUuid;
		}
		else
		{
			assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&(descriptorReadReq->DescriptorUuid.LongUuid), 16);
		}
		printf("Device:%s is reading local descriptor [UUID:0x%04X, NAME:%s] \n", bd_address, assigned_num, GetNameByUUID(assigned_num));
	}
	return BTSDK_TRUE;
}

BTBOOL HandleWriteDescriptorRequestEvent(BTLPVOID Context, BTSDK_GATT_EVENT_TYPE EventType, BTUINT32 RequestID, BTUINT32 EventParameterSize, PBtsdkGATTDescriptorWriteRequestStru descriptorWriteReq)
{
	if (EventType == WriteDescriptorRequestEvent && descriptorWriteReq != NULL)
	{
		BTUINT16 assigned_num = 0;
		BTUINT8 bd_addr[BTSDK_BDADDR_LEN] = {0};
		char bd_address[MAX_PATH] = {0};
		Btsdk_GetRemoteDeviceAddress(descriptorWriteReq->hDevice, bd_addr);
		sprintf(bd_address, FMTBD2STR(bd_addr));		

		if (descriptorWriteReq->DescriptorUuid.IsShortUuid)
		{
			assigned_num = descriptorWriteReq->DescriptorUuid.ShortUuid;
		}
		else
		{
			assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&(descriptorWriteReq->DescriptorUuid.LongUuid), 16);
		}
		printf("Device:%s is writing local descriptor [UUID:0x%04X, NAME:%s] \n", bd_address, assigned_num, GetNameByUUID(assigned_num));
	}
	return BTSDK_TRUE;
}

BTBOOL HandleAddServiceCompleteEvent(BTLPVOID Context, BTSDK_GATT_EVENT_TYPE EventType, BTUINT32 ServiceID, BTUINT32 EventParameterSize, PBtsdkGATTServiceStru EventParameter)
{
	BTUINT16 assigned_num = 0;
	if (EventParameter->ServiceUuid.IsShortUuid)
	{
		assigned_num = EventParameter->ServiceUuid.ShortUuid;
	}
	else
	{
		assigned_num = FormateUUIDToBLEAssignedNum((BTUINT8 *)&(EventParameter->ServiceUuid.LongUuid), 16);
	}
	printf("[UUID:0x%04X, NAME:%s] is added, service id=0x%x\n",assigned_num, GetNameByUUID(assigned_num), ServiceID);
	return BTSDK_TRUE;
}

//All this callback events, application can determine whether to handle them.
//Return BTSDK_TRUE, to ask BlueSoleil to handle them by default.
//Return BTSDK_FALSE, and application will call Btsdk_GATTSendResponse to handle them.
BTBOOL GATTSvrCBK(BTLPVOID Context, BTSDK_GATT_EVENT_TYPE EventType, BTUINT32 RequestID, BTUINT32 EventParameterSize, BTLPVOID EventParameter)
{
	BTBOOL ret = BTSDK_TRUE;
	switch(EventType)
	{
	case AddServiceCompleteEvent:
		{
			ret = (FNBLUETOOTH_GATT_ADD_SERVICE_COMPLETE_CALLBACK*)HandleAddServiceCompleteEvent(Context, EventType, RequestID, EventParameterSize, (PBtsdkGATTServiceStru)EventParameter);
		}
		break;
	case ReadCharacteristicRequestEvent:
		{
			ret = (FNBLUETOOTH_GATT_READ_CHARACTERISTIC_REQUEST_CALLBACK*)HandleReadCharacteristicRequestEvent(Context, EventType, RequestID, EventParameterSize, (PBtsdkGATTCharacteristicReadRequestStru)EventParameter);
		}
		break;
	case WriteCharacteristicRequestEvent:
		{
			ret = (FNBLUETOOTH_GATT_WRITE_CHARACTERISTIC_REQUEST_CALLBACK*)HandleWriteCharacteristicRequestEvent(Context, EventType, RequestID, EventParameterSize, (PBtsdkGATTCharacteristicWriteRequestStru)EventParameter);
		}
		break;
	case ReadDescriptorRequestEvent:
		{
			ret = (FNBLUETOOTH_GATT_READ_DESCRIPTOR_REQUEST_CALLBACK*)HandleReadDescriptorRequestEvent(Context, EventType, RequestID, EventParameterSize, (PBtsdkGATTDescriptorReadRequestStru)EventParameter);
		}
		break;
	case WriteDescriptorRequestEvent:
		{
			ret = (FNBLUETOOTH_GATT_WRITE_DESCRIPTOR_REQUEST_CALLBACK*)HandleWriteDescriptorRequestEvent(Context, EventType, RequestID, EventParameterSize, (PBtsdkGATTDescriptorWriteRequestStru)EventParameter);
		}
		break;
	case ExecuteWriteRequestEvent:
		break;
	default:
		break;
	}
	return ret;
}
void ClearServerResource()
{
	if (local_svr_hdl != BTSDK_INVALID_HANDLE)
	{
		Btsdk_GATTUnregisterServer(local_svr_hdl);
		local_svr_hdl = BTSDK_INVALID_HANDLE;
	}
}


void GATTInitLocalAttribute()
{
	BTINT32 ret = BTSDK_ER_SDK_UNINIT;
	char mutext_name[MAX_PATH] = {0};
	strcpy(mutext_name, "IVTSDKSample_Mutext");
	ret = Btsdk_GATTRegisterServerEx(GATTSvrCBK, 0, mutext_name, &local_svr_hdl);
	if (ret != BTSDK_OK && ret != BTSDK_ER_ITEM_EXIST)
	{
		printf("GATT Server:Register Server failed.\r\n");
	}

	if (local_svr_hdl != BTSDK_INVALID_HANDLE && ret == BTSDK_OK)
	{
		//Current Time Service
		{
			BTUINT32 serviceID = 0;
			BtsdkGATTUUIDStru ServiceUuid = {0};
			ServiceUuid.IsShortUuid = TRUE;
			ServiceUuid.ShortUuid = 0x1805;
			ret = Btsdk_GATTCreateService(local_svr_hdl, &ServiceUuid, BTSDK_GATT_SERVICE_TYPE_PRIMARY, &serviceID);
			if (ret == BTSDK_OK)
			{
				//Current Time
				BtsdkGATTCharacteristicStru CharacteristicProperties = {0};
				PBtsdkGATTCharacteristicValueStru pCharacteristicValue = NULL;
				BTUINT32 characteristic_id = 0;
				CharacteristicProperties.CharacteristicUuid.IsShortUuid = BTSDK_TRUE;
				CharacteristicProperties.CharacteristicUuid.ShortUuid = 0x2A2B; //Current time characteristic
				CharacteristicProperties.IsNotifiable = BTSDK_TRUE;
				CharacteristicProperties.IsReadable = BTSDK_TRUE;
				pCharacteristicValue = (PBtsdkGATTCharacteristicValueStru)malloc(sizeof(BtsdkGATTCharacteristicValueStru) + 9);
				if (pCharacteristicValue != NULL)
				{
					memset(pCharacteristicValue, 0, sizeof(BtsdkGATTCharacteristicValueStru) + 9);
					pCharacteristicValue->DataSize = 10;					
					{
						char* pTmp = NULL;
						SYSTEMTIME sysTime = {0};
						GetSystemTime(&sysTime);
						pTmp = pCharacteristicValue->Data;
						*((BTUINT16*)pTmp) = sysTime.wYear; //Year
						pTmp += 2;
						*((BTUINT8*)pTmp) = sysTime.wMonth; //Month
						pTmp++;
						*((BTUINT8*)pTmp) = sysTime.wDay; //Day
						pTmp++;
						*((BTUINT8*)pTmp) = sysTime.wHour + 8;//hours
						pTmp++;
						*((BTUINT8*)pTmp) = sysTime.wMinute; //Minute
						pTmp++;
						*((BTUINT8*)pTmp) = sysTime.wSecond;//Second
						pTmp++;
						*((BTUINT8*)pTmp) = sysTime.wDayOfWeek;
					}
				}
				if (BTSDK_OK == Btsdk_GATTAddCharacteristic(serviceID, &CharacteristicProperties, pCharacteristicValue, &characteristic_id))
				{
					PBtsdkGATTDescriptorValueStru pDescriptorValue = NULL;
					BtsdkGATTDescriptorPermissionStru Permissions = {0};
					Permissions.ReadPermission = BTSDK_GATT_READABLE_WITHOUT_SECURITY;
					Permissions.WritePermission = BTSDK_GATT_WRITABLE_WITHOUT_SECURITY;
					pDescriptorValue = (PBtsdkGATTDescriptorValueStru)malloc(sizeof(BtsdkGATTDescriptorValueStru)+1);
					if (pDescriptorValue != NULL)
					{
						memset(pDescriptorValue, 0, sizeof(BtsdkGATTDescriptorValueStru)+1);
						pDescriptorValue->DescriptorType = ClientCharacteristicConfiguration;
						pDescriptorValue->DescriptorUuid.IsShortUuid = BTSDK_TRUE;
						pDescriptorValue->DescriptorUuid.ShortUuid = 0x2902;//
						pDescriptorValue->ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;
					}
					if (BTSDK_OK != Btsdk_GATTAddDescriptor(characteristic_id, pDescriptorValue, &Permissions))
					{
						printf("GATT Server: Add Client Characteristic Configuration failed.\r\n");
					}
				}
				if (pCharacteristicValue != NULL)
				{
					free(pCharacteristicValue);
					pCharacteristicValue = NULL;
				}
				ret = Btsdk_GATTAddService(serviceID);
				if (ret != BTSDK_OK)
				{
					printf("GATT Server:Add Device Information Service failed.\r\n");
				}
			}
		}
		//Device Information
		{
			BTUINT32 serviceID = 0;
			BtsdkGATTUUIDStru ServiceUuid = {0};
			ServiceUuid.IsShortUuid = TRUE;
			ServiceUuid.ShortUuid = 0x180A; 
			ret = Btsdk_GATTCreateService(local_svr_hdl, &ServiceUuid, BTSDK_GATT_SERVICE_TYPE_PRIMARY, &serviceID);
			if (ret == BTSDK_OK)
			{
				//Manufacturer Name String
				{
					BtsdkGATTCharacteristicStru CharacteristicProperties = {0};
					PBtsdkGATTCharacteristicValueStru pCharacteristicValue = NULL;
					BTUINT32 characteristic_id = 0;
					CharacteristicProperties.CharacteristicUuid.IsShortUuid = BTSDK_TRUE;
					CharacteristicProperties.CharacteristicUuid.ShortUuid = 0x2A29; //Manufacturer name string
					CharacteristicProperties.IsReadable = BTSDK_TRUE;
					pCharacteristicValue = (PBtsdkGATTCharacteristicValueStru)malloc(sizeof(BtsdkGATTCharacteristicValueStru)+3);
					if (pCharacteristicValue != NULL)
					{
						memset(pCharacteristicValue, 0, sizeof(BtsdkGATTCharacteristicValueStru)+3);
						pCharacteristicValue->DataSize = 4;
						strcpy(pCharacteristicValue->Data, "IVT");
						if (BTSDK_OK != Btsdk_GATTAddCharacteristic(serviceID, &CharacteristicProperties, pCharacteristicValue, &characteristic_id))
						{
							printf("GATT Server: Add Characteristic manufacturer name string failed.\r\n");
						}
						if (pCharacteristicValue != NULL)
						{
							free(pCharacteristicValue);
							pCharacteristicValue = NULL;
						}
					}
				}
				//Model Number
				{
					BtsdkGATTCharacteristicStru CharacteristicProperties = {0};
					PBtsdkGATTCharacteristicValueStru pCharacteristicValue = NULL;
					BTUINT32 characteristic_id = 0;
					CharacteristicProperties.CharacteristicUuid.IsShortUuid = BTSDK_TRUE;
					CharacteristicProperties.CharacteristicUuid.ShortUuid = 0x2A24; //Model Number String
					CharacteristicProperties.IsReadable = BTSDK_TRUE;
					pCharacteristicValue = (PBtsdkGATTCharacteristicValueStru)malloc(sizeof(BtsdkGATTCharacteristicValueStru)+7);
					if (pCharacteristicValue != NULL)
					{
						memset(pCharacteristicValue, 0, sizeof(BtsdkGATTCharacteristicValueStru)+7);
						pCharacteristicValue->DataSize = 8;
						strcpy(pCharacteristicValue->Data, "1.0.0.0");
						if (BTSDK_OK != Btsdk_GATTAddCharacteristic(serviceID, &CharacteristicProperties, pCharacteristicValue, &characteristic_id))
						{
							printf("GATT Server: Add Characteristic Model Number String failed.\r\n");
						}
						if (pCharacteristicValue != NULL)
						{
							free(pCharacteristicValue);
							pCharacteristicValue = NULL;
						}
					}
				}
				
				//System ID
				{
					BtsdkGATTCharacteristicStru CharacteristicProperties = {0};
					PBtsdkGATTCharacteristicValueStru pCharacteristicValue = NULL;
					BTUINT32 characteristic_id = 0;
					CharacteristicProperties.CharacteristicUuid.IsShortUuid = BTSDK_TRUE;
					CharacteristicProperties.CharacteristicUuid.ShortUuid = 0x2A23; //System ID
					CharacteristicProperties.IsReadable = BTSDK_TRUE;
					
					//strcpy(robotCMD,"P1M10C00C00C00C00P2M20C03C10C00C00");
					strcpy(robotCMD,"P1M10C10");
					pCharacteristicValue = (PBtsdkGATTCharacteristicValueStru)malloc(sizeof(BtsdkGATTCharacteristicValueStru)+16); //+ {num}= length of string
					if (pCharacteristicValue != NULL)
					{
						memset(pCharacteristicValue, 0, sizeof(BtsdkGATTCharacteristicValueStru)+16); //+ {num}= length of string
						pCharacteristicValue->DataSize = 17; //+ {num}= length of string +1
						//Application can add system ID to the pCharacteristicValue->Data
						
						strcpy(pCharacteristicValue->Data,robotCMD);
						if (BTSDK_OK != Btsdk_GATTAddCharacteristic(serviceID, &CharacteristicProperties, pCharacteristicValue, &characteristic_id))
						{
							printf("GATT Server: Add Characteristic System ID failed.\r\n");
						}
						if (pCharacteristicValue != NULL)
						{
							free(pCharacteristicValue);
							pCharacteristicValue = NULL;
						}
					}
				}
				ret = Btsdk_GATTAddService(serviceID);
				if (ret != BTSDK_OK)
				{
					printf("GATT Server:Add Device Information Service failed.\r\n");
				}
			}
		}
		//Immediate Alert
		{
			BTUINT32 serviceID = 0;
			BtsdkGATTUUIDStru ServiceUuid = {0};
			ServiceUuid.IsShortUuid = TRUE;
			ServiceUuid.ShortUuid = 0x1802; 
			ret = Btsdk_GATTCreateService(local_svr_hdl, &ServiceUuid, BTSDK_GATT_SERVICE_TYPE_PRIMARY, &serviceID);
			if (ret != BTSDK_OK)
			{
				goto _error_exit;
			}
			else
			{
				BtsdkGATTCharacteristicStru CharacteristicProperties = {0};
				PBtsdkGATTCharacteristicValueStru pCharacteristicValue = NULL;
				BTUINT32 characteristic_id = 0;
				CharacteristicProperties.CharacteristicUuid.IsShortUuid = BTSDK_TRUE;
				CharacteristicProperties.CharacteristicUuid.ShortUuid = 0x2A06; //Alert level
				CharacteristicProperties.IsReadable = BTSDK_TRUE;
				CharacteristicProperties.IsWritableWithoutResponse = BTSDK_TRUE;
				pCharacteristicValue = (PBtsdkGATTCharacteristicValueStru)malloc(sizeof(BtsdkGATTCharacteristicValueStru));
				if (pCharacteristicValue != NULL)
				{
					memset(pCharacteristicValue, 0, sizeof(BtsdkGATTCharacteristicValueStru));
					pCharacteristicValue->DataSize = 1;
					pCharacteristicValue->Data[0] = 0;
				}
				if (BTSDK_OK != Btsdk_GATTAddCharacteristic(serviceID, &CharacteristicProperties, pCharacteristicValue, &characteristic_id))
				{
					if (pCharacteristicValue != NULL)
					{
						free(pCharacteristicValue);
						pCharacteristicValue = NULL;
					}
					goto _error_exit;
				}
				ret = Btsdk_GATTAddService(serviceID);
				if (ret != BTSDK_OK)
				{
					printf("GATT Server:Add Immediate Alert Service failed.\r\n");
				}
				if (pCharacteristicValue != NULL)
				{
					free(pCharacteristicValue);
					pCharacteristicValue = NULL;
				}
			}
		}
		
		
	}
	return;
_error_exit:
	ClearServerResource();
}

void GATTListLocalSvrService()
{
	if (local_svr_hdl != BTSDK_INVALID_HANDLE)
	{
		BTUINT16 num = 0;
		Btsdk_GATTGetServices(local_svr_hdl, BUFF_NUM, svc, &num, BTSDK_GATT_FLAG_NONE);
		svc_num = num;
		printf("get services number: %d\n", num);
		ShowGATTServices();
	}
}

void GATTGetLocalSvrCharacteristic()
{
	BTUINT16 num = 0;
	BTUINT32 ch = 0;
	int sel;
	if (ShowGATTServices() <=0)
	{
		return;
	}
SELECT_ID:
	printf("please select a service by type the service's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= svc_num || sel < 0)
	{
		printf("%c is a wrong service's id\n", ch);
		goto SELECT_ID;
	}
	Btsdk_GATTGetCharacteristics(BTSDK_INVALID_HANDLE, &svc[sel], BUFF_NUM, crt, &num, BTSDK_GATT_FLAG_NONE); //When get local service characteristic  hDevice set to BTSDK_INVALID_HANDLE
	crt_num = num;
	printf("get characteristics number: %d\n", num);
	ShowGATTCharacteristics();
}

void GATTGetLocalSvrCharacteristicValue()
{
	BTUINT16 num = 0;
	BTUINT16 size = 0;
	BTUINT32 ch = 0;
	int i;
	int sel;
	PBtsdkGATTCharacteristicValueStru val = NULL;
	if (ShowGATTCharacteristics() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a character by type the character's id\n");
	AppWaitForInputInt(&ch);

	sel  = (int)(ch -1);
	if (sel >= crt_num || sel < 0)
	{
		printf("%c is a wrong character's id\n", ch);
		goto SELECT_ID;
	}	
	Btsdk_GATTGetCharacteristicValue(BTSDK_INVALID_HANDLE, &crt[sel], 0, NULL, &num, BTSDK_GATT_FLAG_NONE);	
	if (num <= 0)
	{
		printf("get characteristics value size is %d\n", num);
		return;
	}
	val = (PBtsdkGATTCharacteristicValueStru)malloc(num);
	memset(val, 0, num);
	Btsdk_GATTGetCharacteristicValue(BTSDK_INVALID_HANDLE, &crt[sel], num, val, &size, BTSDK_GATT_FLAG_NONE);
	if (val != NULL)
	{
		if (val->DataSize > 0)
		{
			printf("get characteristics value data size is %d\n", val->DataSize);
			printf("get characteristics value data is ");
			for (i=0; i<val->DataSize; i++) 
			{
				printf("0x%02x ", val->Data[i]);
			}
			printf("\n");
		}
		else
		{
			printf("get characteristics value size is %d\n", val->DataSize);
		}
	}
	free(val);
}

BTUINT8 *SetBloodPressureMeasurement(BTUINT8 *data, int *pData_num)	
{
	BTUINT16 BloodPressure[3] = {0};

	printf("Please input Blood Pressure Measurement Compound Value-Systolic(mmHg):\n");
	AppWaitForInputSFloat(BloodPressure);

	printf("Please input Blood Pressure Measurement Compound Value-Disatolic(mmHg):\n");
	AppWaitForInputSFloat(BloodPressure+1);

	printf("Please input Blood Pressure Measurement Compound Value-Mean Arterial Pressure(mmHg):\n");
	AppWaitForInputSFloat(BloodPressure+2);

	*pData_num = 7;
	data[0] = 0;//Flags
	data[1] = BloodPressure[0]/256;
	data[2] = BloodPressure[0]%256;
	data[3] = BloodPressure[1]/256;
	data[4] = BloodPressure[1]%256;
	data[5] = BloodPressure[2]/256;
	data[6] = BloodPressure[2]%256;

	return data;
}

BTUINT8 *SetBloodPressureFeature(BTUINT8 *data, int *pData_num)
{
	*pData_num = 2;
	data[0] = 0;
	data[1] = 0;	//BloodPressureFeature 16bit

	return data;
}

BTUINT8 *SetGlucoseMeasurement(BTUINT8 *data, int *pData_num)
{

	SYSTEMTIME now_time;
	BTUINT16 glucose;
	GetLocalTime(&now_time);
	
	printf("Please input Glucose Concentration-uints of kg/L:\n");
	AppWaitForInputSFloat(&glucose);

	*pData_num = 12;
	
	data[0] = 0;//Flags 8bit
	data[1] = 0;
	data[2] = 0;//sequence number 16bit
	data[3] = now_time.wYear/256;
	data[4] = now_time.wYear%256;
	data[5] = now_time.wMonth;
	data[6] = now_time.wDay;
	data[7] = now_time.wHour;
	data[8] = now_time.wMinute;
	data[9] = now_time.wSecond;
	data[10] = glucose/256;
	data[11] = glucose%256;

	return data;
}

BTUINT8 *SetGlucoseFeature(BTUINT8 *data, int *pData_num)
{
	*pData_num = 2;
	data[0] = 0;
	data[1] = 0;//GlucoseFeature 16bit
	
	return data;
}

BTUINT8 *SetRecordAccessControlPoint(BTUINT8 *data, int *pData_num)
{
	*pData_num = 3;
	data[0] = 0;//op code 8bit
	data[1] = 0;//operator 8bit
	data[2] = 0;//operand
	
	return data;
}

void GATTSetLocalSvrCharacteristicValue()
{
	BTUINT32 ch = 0;
	BTINT32 ret;
	int sel;
	int data_num =  0;
	BTUINT8 data[32] = {0};
	INT index = 0;
	INT characterValue = 0;
	if (ShowGATTCharacteristics() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a character by type the character's id\n");
	AppWaitForInputInt(&ch);

	sel  = (int)(ch - 1);
	if (sel >= crt_num || sel < 0)
	{
		printf("%c is a wrong character's id\n", ch);
		goto SELECT_ID;
	}
	if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A2B) //Current Time
	{
		char* pTmp = data;
		SYSTEMTIME sysTime = {0};
		data_num = 10;
		GetSystemTime(&sysTime);
		*((BTUINT16*)pTmp) = sysTime.wYear; //Year
		pTmp += 2;
		*((BTUINT8*)pTmp) = sysTime.wMonth; //Month
		pTmp++;
		*((BTUINT8*)pTmp) = sysTime.wDay; //Day
		pTmp++;
		*((BTUINT8*)pTmp) = sysTime.wHour + 8;//hours
		pTmp++;
		*((BTUINT8*)pTmp) = sysTime.wMinute; //Minute
		pTmp++;
		*((BTUINT8*)pTmp) = sysTime.wSecond;//Second
		pTmp++;
		*((BTUINT8*)pTmp) = sysTime.wDayOfWeek; //Day of week
		pTmp += 2;
		//Adjust reason
		*((BTUINT8*)pTmp) = 1;		
	}
	else if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A35) //Blood Pressure Measurement
	{
		SetBloodPressureMeasurement(data,&data_num);
	}
	else if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A49) //Blood Pressure Feature
	{
		SetBloodPressureFeature(data,&data_num);
	}
	else if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A18) //Glucose Measurement
	{
		SetGlucoseMeasurement(data,&data_num);
	}
	else if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A51) //Glucose Feature
	{
		SetGlucoseFeature(data,&data_num);
	}
	else if (crt[sel].CharacteristicUuid.IsShortUuid == BTSDK_TRUE && crt[sel].CharacteristicUuid.ShortUuid == 0x2A52) //Record Access Control Point
	{
		SetRecordAccessControlPoint(data,&data_num);
	}
	else
	{
		printf("please enter the characteristics value data length\n");
		scanf("%x", &ch);
		data_num = ch;
		if (data_num >= 32)
		{
			goto SELECT_ID;
		}
		index = 0;
		//ect. 0x01 0x02...
		printf("please enter the characteristics value\n");
		while(index < data_num && 1 == scanf("%x", &characterValue))
		{
			data[index] = (BTUINT8)characterValue;
			index++;
		}
	}

	ret = Btsdk_GATTNotifyCharacteristicChanged(BTSDK_INVALID_HANDLE, crt[sel].AttributeHandle, data, data_num);
	if (ret != BTSDK_OK)
	{
		printf("GATTServer:Set characteristics value fail\n");
	}
	else
	{
		printf("GATTServer:Set characteristics value ok\n");
	}	
}

void GATTGetLocalSvrCharDescriptor()
{
	BTUINT16 num = 0;
	BTUINT32 ch = 0;
	int sel;
	if (ShowGATTCharacteristics() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a character by type the character's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= crt_num || sel < 0)
	{
		printf("%c is a wrong character's id\n", ch);
		goto SELECT_ID;
	}
	Btsdk_GATTGetDescriptors(BTSDK_INVALID_HANDLE, &crt[sel], BUFF_NUM, dsc, &num, BTSDK_GATT_FLAG_NONE);
	dsc_num = num;
	printf("get descriptors number: %d\n", num);
	ShowGATTDescriptors();	
}
 
void GATTRemoveLocalSvrService()
{
	BTUINT16 num = 0;
	BTUINT32 ch = 0;
	int sel;
	if (ShowGATTServices() <=0)
	{
		return;
	}
SELECT_ID:
	printf("please select a service by type the service's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= svc_num || sel < 0)
	{
		printf("%c is a wrong service's id\n", ch);
		goto SELECT_ID;
	}
	Btsdk_GATTRemoveService(BTSDK_TRUE, &svc[sel]);
}

void GATTGetLocalSvrCharDescriptorValue()
{
	BTUINT32 ch = 0;
	BTUINT16 size_req;
	int sel;
	PBtsdkGATTDescriptorValueStru val;
	UCHAR str[256];

	memset(str, 0, 256);
	if (ShowGATTDescriptors() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a descriptor by type the descriptor's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= dsc_num || sel < 0)
	{
		printf("%c is a wrong descriptor's id\n", ch);
		goto SELECT_ID;
	}
	
	
	if (BTSDK_OK == Btsdk_GATTGetDescriptorValue(BTSDK_INVALID_HANDLE, &dsc[sel], 0, NULL, &size_req, BTSDK_GATT_FLAG_NONE))
	{
		val = (PBtsdkGATTDescriptorValueStru)malloc(size_req);
		if (val != NULL)
		{
			memset(val, 0, size_req);
			Btsdk_GATTGetDescriptorValue(BTSDK_INVALID_HANDLE, &dsc[sel], size_req, val, &size_req, BTSDK_GATT_FLAG_NONE);
			switch (val->DescriptorType)
			{
				case CharacteristicExtendedProperties:
					printf("RelW = %d, AuxW = %d\n", val->CharacteristicExtendedProperties.IsReliableWriteEnabled, 
						val->CharacteristicExtendedProperties.IsAuxiliariesWritable);
					break;
				case CharacteristicFormat:
					sprintf(&str[strlen(str)],"For:0x2X, Exp:0x2X, NS:0x2X", val->CharacteristicFormat.Format, 
						val->CharacteristicFormat.Exponent, val->CharacteristicFormat.NameSpace);		
					printf("%s\n", str);
					break;
				case ClientCharacteristicConfiguration:
					printf("Noti:%d, Indi:%d\n", val->ClientCharacteristicConfiguration.IsSubscribeToNotification,
						val->ClientCharacteristicConfiguration.IsSubscribeToIndication);
					break;
				case ServerCharacteristicConfiguration:
					printf("Broad:%d\n", val->ServerCharacteristicConfiguration.IsBroadcast);
					break;		
			}
			free(val);
		}
		
	}
}

void GATTSetLocalSvrCharDescriptorValue()
{
	BTUINT32 ch = 0;
	BTUINT16 size_req;
	int sel;
	PBtsdkGATTDescriptorValueStru val = NULL;
	UCHAR str[256];

	memset(str, 0, 256);
	if (ShowGATTDescriptors() <= 0)
	{
		return;
	}
SELECT_ID:
	printf("please select a descriptor by type the descriptor's id\n");
	AppWaitForInputInt(&ch);

	sel = (int)(ch - 1);
	if (sel >= dsc_num || sel < 0)
	{
		printf("%c is a wrong descriptor's id\n", ch);
		goto SELECT_ID;
	}
	if (dsc[sel].DescriptorType == ClientCharacteristicConfiguration)
	{
		val = (PBtsdkGATTDescriptorValueStru)malloc(sizeof(BtsdkGATTDescriptorValueStru));
		if (val != NULL)
		{
			memset(val, 0, sizeof(BtsdkGATTDescriptorValueStru));
			val->DescriptorType = ClientCharacteristicConfiguration;
			val->DescriptorUuid.IsShortUuid = BTSDK_TRUE;
			val->DescriptorUuid.ShortUuid = 0x2902;//
			val->ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;
		}

	}
	if (val != NULL)
	{
		if (BTSDK_OK != Btsdk_GATTChangeDescriptorValue(dsc[sel].AttributeHandle, val))
		{
			printf("GATT Server: Set local server descriptor value failed.\r\n");
		}
		free(val);
	}
	
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

---------------------------------------------------------------------------*/
void GATTServerExecCmd(BTUINT8 choice)
{
	switch (choice) {
	case '1':
		GATTSvrSetLocalName();
		break;
	case '2':
		GATTSvrEnableAdvertising();
		break;
	case '3':
		GATTSvrDisableAdvertising();
		break;
	case '4':
		GATTInitLocalAttribute();
		break;
	case '5':
		GATTListLocalSvrService();
		break;
	case '6':
		GATTRemoveLocalSvrService();
		break;
	case '7':
		GATTGetLocalSvrCharacteristic();
		break;
	case '8':
		GATTGetLocalSvrCharacteristicValue();
		break;
	case '9':
		GATTSetLocalSvrCharacteristicValue();
		break;
	case 'a':
		GATTGetLocalSvrCharDescriptor();
		break;
	case 'b':
		GATTGetLocalSvrCharDescriptorValue();
		break;
	case 'c':
		GATTSetLocalSvrCharDescriptorValue();
		break;
	case 'r':
		{
			ClearServerResource();
		}
		break;
	default:
		printf("*Invalid command!");
		break;
	}
}

void GATTServerShowMenu(void)
{
	printf("\n");
	printf("****************************************\n");
	printf("*         GATT Server Testing Menu     *\n");
	printf("* <1> Set local name and appearance    *\n");
	printf("* <2> Enable advertising               *\n");
	printf("* <3> Disable advertising              *\n");
	printf("* <4> Init local attributes            *\n");
	printf("* <5> Get local services               *\n");
	printf("* <6> Remove local services            *\n");
	printf("* <7> Get local Characteristics        *\n");
	printf("* <8> Get local Characteristics Value  *\n");
	printf("* <9> Set local Characteristics Value  *\n");
	printf("* <a> Get local Descriptors            *\n");
	printf("* <b> Get local Descriptor value       *\n");
	printf("* <c> Set local Descriptor value       *\n");
	printf("* <r> Return to upper menu             *\n");
	printf("****************************************\n");
	printf(">");
}

void TestGATTServerFunc()
{
	BTUINT8 ch = 0;
	GATTServerShowMenu();
	while (ch != 'r')
	{
		AppWaitForInput(&ch, 1);
		//getchar();
		if (ch == '\n')
		{
			printf(">");
		} 
		else
		{
			GATTServerExecCmd(ch);
			printf("\n");
			if (ch != 'r')
			{
				GATTServerShowMenu();
			}
		}
	}
}