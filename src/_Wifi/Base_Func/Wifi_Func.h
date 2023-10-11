/*
 * Wifi_Func.h
 *
 * Created: 2021/4/19 上午 10:35:38
 *  Author: willchen
 */ 


#ifndef WIFI_FUNC_H_
#define WIFI_FUNC_H_

void Get_CMD_Form_PHP(void);
void Run_Wifi_Command_Func(void);
unsigned tmp_MAC_index[3];
bool getOK(void);
bool getERROR(void);
bool getHTTP(void);
bool chkTimeout(void);
bool getTimeout(void);
bool getBusy(void);
bool getFAIL(void);
void ResetWifiFlag(void);

bool getWifiKeyWord(void);
bool getNoChange(void);
bool getWIFI_GOT_IP(void);

bool CheckWifi_meanNG_Flag(void);
bool CheckWifi_meanOK_Flag(void);

void Reset_wifi_get_data(unsigned long tmpCounter);
#endif /* WIFI_FUNC_H_ */