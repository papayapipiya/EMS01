/*
 * AP_Mode.h
 *
 * Created: 2021/4/19 上午 09:17:44
 *  Author: willchen
 */ 


#ifndef AP_MODE_H_
#define AP_MODE_H_




#define PTR	My_WiFi.Input_buffer
#define SSID_PASSWORD_Max				64
#define AT_Command_Delay_For_AP_MODE	500




			unsigned char	AP_Buffer[15];


volatile	unsigned char	EMAIL_Buffer			[SSID_PASSWORD_Max];


volatile	unsigned char	Input_EMAIL_Buffer		[SSID_PASSWORD_Max];
			

			bool			CMD_ERROR;
			
			unsigned char	Key_Word_Flag;
			bool			By_Browser;
#define		SSID_txt_NVM			600
#define		PASSWORD_txt_NVM		601
#define		EMAIL_txt_NVM			602

void Wifi_AP_Func(void);
void Wifi_Rundown_AP_Func(void);


#endif /* AP_MODE_H_ */