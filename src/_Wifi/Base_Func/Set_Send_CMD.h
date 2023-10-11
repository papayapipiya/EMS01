/*
 * Set_Send_CMD.h
 *
 * Created: 2021/4/19 上午 10:30:39
 *  Author: willchen
 */ 


#ifndef SET_SEND_CMD_H_
#define SET_SEND_CMD_H_

unsigned char UID_index;

#define wifi_delay_500		500					//約0.4秒
#define wifi_delay_2000		2000				//約1.6秒
#define wifi_delay_3200ms	4000				//約3.2秒
#define wifi_delay_6400ms	8000				//約6.4秒
#define wifi_delay_12800ms	16000				//約12.8秒
#define wifi_delay_01		wifi_delay_500
#define wifi_delay_02		wifi_delay_2000

void Setting_Send_CMD_Check_NowCMD(void);
unsigned char * WifiMACtoASCII(void);

#endif /* SET_SEND_CMD_H_ */