/*
 * Wifi_String_Func.h
 *
 * Created: 2021/4/19 上午 10:19:19
 *  Author: willchen
 */ 


#ifndef WIFI_STRING_FUNC_H_
#define WIFI_STRING_FUNC_H_




void WiFi_txt_Separation_line(void);
void WiFi_txt_Function(volatile WiFi_t *tmpWiFi);
void replace_txt(char *t, char *s, char *tmp_val);

//--------------------

char tmp_txt[100];	//for [ char * hex_to_val_ascii(unsigned int tmpVal); ]
char * hex_to_val_ascii(unsigned int tmpVal);

//--------------------

bool check_keyword_in_String(char *t, char *s);
bool Check_Send_CMD_Response(void);

//--------------------

unsigned char tmpMAC[8];	//for [ unsigned char * MACtoASCII(unsigned char tmpIndex); ]
unsigned char * MACtoASCII(unsigned char tmpIndex);

//--------------------


#endif /* WIFI_STRING_FUNC_H_ */