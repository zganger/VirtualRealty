#ifndef SDK_GATT_H
#define SDK_GATT_H

BTDEVHDL SelectRemoteLEDevice();
void GetAllRmtLEDevHdl();
void StartSearchLEDevice();
void DisplayRemoteLEDevices();
void AppInquiryLEDevInd(BTDEVHDL dev_hdl);
void AppInqLEDevCompInd(void);

void AppConnectionCompleteCbk(BTDEVHDL dev_hdl);
void AppDisconnectionCompleteCbk(BTDEVHDL dev_hdl, BTUINT32 reason);
void AppDeviceFoundFuncCbk(BTDEVHDL dev_hdl);
BOOL IsDeviceHdlInList(BTDEVHDL dev_hdl);

void GetBloodPressureMeasurement(BTUCHAR *p);
void GetGlucoseMeasurement(BTUCHAR *p);
BTUINT8 *SetBloodPressureMeasurement(BTUINT8 *data, int *pData_num);
BTUINT8 *SetBloodPressureFeature(BTUINT8 *data, int *pData_num);
BTUINT8 *SetGlucoseMeasurement(BTUINT8 *data, int *pData_num);
BTUINT8 *SetGlucoseFeature(BTUINT8 *data, int *pData_num);
BTUINT8 *SetRecordAccessControlPoint(BTUINT8 *data, int *pData_num);

#endif
