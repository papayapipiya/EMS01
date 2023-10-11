#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"

//=================================================
#ifdef Dev_Wifi
//=================================================




void Send_Html_String(const char *tmp_ptr)
{
	//先用AT+CIPSEND 送出字串長度 (AT+CIPSEND=xxx)
	uart_str_COM(Wifi_COM, "AT+CIPSEND=");
	uTXByte(Wifi_COM, mGateway.Wifi->target_Number);
	uTXByte(Wifi_COM, ',');
	uart_send_word(Wifi_COM , hex_to_bcd(strlen(tmp_ptr)));
	uart_str_COM(Wifi_COM, "\r\n");	
	
	//給wifi module反應時間
	delay_cycles_ms(5);
	
	//發送實際內容
	uart_str_COM(Wifi_COM, tmp_ptr);
}


void debugSend_Html_String(const char *tmp_ptr)
{
	uart_str_COM(Debug_COM, "\rAT+CIPSEND=");
	uTXByte(Debug_COM, mGateway.Wifi->target_Number);
	uTXByte(Debug_COM, ',');
	uart_send_word(Debug_COM , hex_to_bcd(strlen(tmp_ptr)));
	uart_str_COM(Debug_COM, "\r\n");
	uart_str_COM(Debug_COM, tmp_ptr);
}


void print_html_main_string(void)
{
	unsigned int html_lens;
	const char header[256]="HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n"
	"Content-Length:";
	char html[2048]="";
	char html2[2048]="";

	
	sprintf(html, "%s",				"<html><head><style>input{font-size: 26px;}h1{color: blue;font-size: 26px;}input:focus{outline:0;background-color:#ffffe0;}</style></head><font style='font-size:26px'>");
	//[string] Ping Google 反應時間 : 
	sprintf(html, "%s%s", html,		"<h1>Ping Google \xe5\x8f\x8d\xe6\x87\x89\xe6\x99\x82\xe9\x96\x93 "); 
	//[data] Ping google的反應時間
	sprintf(html, "%s%s", html,		mGateway.Wifi->Ping_Buffer);
	sprintf(html, "%s%s", html,		"ms</h1>");
	
	//[string] 換行 + 水平線
	sprintf(html, "%s%s", html,		"<p><hr>");
	//[string] 可用的無線網路名稱
	sprintf(html, "%s%s", html,		"<h1>\xe5\x8f\xaf\xe7\x94\xa8\xe7\x9a\x84\xe7\x84\xa1\xe7\xb7\x9a\xe7\xb6\xb2\xe8\xb7\xaf\xe5\x90\x8d\xe7\xa8\xb1</h1>");
	sprintf(html, "%s%s", html,		mGateway.Wifi->ssid.ApListSrting);
	sprintf(html, "%s%s", html,		"<hr>");
	
	//[string] form表頭
	sprintf(html, "%s%s", html,		"<p><form action='http://192.168.4.1' method='GET'>");
	//[string] 無線網路名稱
	sprintf(html, "%s%s", html,		"<h1>\xe7\x84\xa1\xe7\xb7\x9a\xe7\xb6\xb2\xe8\xb7\xaf\xe5\x90\x8d\xe7\xa8\xb1 & \xe5\xaf\x86\xe7\xa2\xbc</h1>");
	//[string] 無線網路名稱的text表頭
	sprintf(html, "%s%s", html,		"SSID <input type='text' name='_SSID' value='");
	//[data] eeprom ssid string 
	sprintf(html, "%s%s", html,		mGateway.Wifi->SSID_Buffer);
	sprintf(html, "%s%s", html,		"'>");
	
	sprintf(html, "%s%s", html,		"<p>");

	//[string] 無線網路密碼的text表頭
	sprintf(html, "%s%s", html,		"Password <input type='text' name='_PASS' value='");
	//[data] eeprom password string 
	sprintf(html, "%s%s", html,		mGateway.Wifi->PASSWORD_Buffer);
	sprintf(html, "%s%s", html,		"'>");
	
	
	sprintf(html, "%s%s", html,		"<p><hr>");
	sprintf(html, "%s%s", html,		"<h1>\xe7\x9b\xae\xe5\x89\x8d\xe6\xba\xab\xe5\xba\xa6\xe7\x8b\x80\xe6\x85\x8b</h1>");
	sprintf(html, "%s%s", html,		"[1] : ");
	sprintf(html, "%s%d.%d", html,	mGateway.uEMS.uModule.uADC.NTC_No_01 / 10, mGateway.uEMS.uModule.uADC.NTC_No_01 % 10);
	sprintf(html, "%s%s", html,		"\xe2\x84\x83");	//	℃
	
	sprintf(html, "%s%s", html,		"&emsp;");			//	tab
	
	sprintf(html, "%s%s", html,		"[4] : ");
	sprintf(html, "%s%d.%d", html,	mGateway.uEMS.uModule.uADC.NTC_No_04 / 10, mGateway.uEMS.uModule.uADC.NTC_No_04 % 10);
	sprintf(html, "%s%s", html,		"\xe2\x84\x83");	//	℃
	
	sprintf(html, "%s%s", html,		"&emsp;");			//	tab
	sprintf(html, "%s%s", html,		"[7] : ");
	sprintf(html, "%s%d.%d", html,		mGateway.uEMS.uModule.uADC.NTC_No_07 / 10, mGateway.uEMS.uModule.uADC.NTC_No_07 % 10);
	sprintf(html, "%s%s", html,		"\xe2\x84\x83");	//	℃

	sprintf(html, "%s%s", html,		"<br>");			//	<br>
	
	sprintf(html, "%s%s", html,		"[2] : ");
	sprintf(html, "%s%d.%d", html,	mGateway.uEMS.uModule.uADC.NTC_No_02 / 10, mGateway.uEMS.uModule.uADC.NTC_No_02 % 10);
	sprintf(html, "%s%s", html,		"\xe2\x84\x83");	//	℃
	
	sprintf(html, "%s%s", html,		"&emsp;");			//	tab
	
	sprintf(html, "%s%s", html,		"[5] : ");
	sprintf(html, "%s%d.%d", html,	mGateway.uEMS.uModule.uADC.NTC_No_05 / 10, mGateway.uEMS.uModule.uADC.NTC_No_05 % 10);
	sprintf(html, "%s%s", html,		"\xe2\x84\x83"); //	℃
	
	
	sprintf(html, "%s%s", html,		"<br>");			//	<br>
	
	sprintf(html, "%s%s", html,		"[3] : ");
	sprintf(html, "%s%d.%d", html,	mGateway.uEMS.uModule.uADC.NTC_No_03 / 10, mGateway.uEMS.uModule.uADC.NTC_No_03 % 10);
	sprintf(html, "%s%s", html,		"\xe2\x84\x83"); //	℃
	
	sprintf(html, "%s%s", html,		"&emsp;");			//	tab
		
	sprintf(html, "%s%s", html,		"[6] : ");
	sprintf(html, "%s%d.%d", html,	mGateway.uEMS.uModule.uADC.NTC_No_06 / 10, mGateway.uEMS.uModule.uADC.NTC_No_06 % 10);
	sprintf(html, "%s%s", html,		"\xe2\x84\x83"); //	℃
	
	
	
	
	sprintf(html, "%s%s", html,		"<br><hr>");
	sprintf(html, "%s%s", html,		"<h1>\xe6\xba\xab\xe5\xba\xa6\xe6\x8e\xa7\xe5\x88\xb6\xe5\x8f\x83\xe6\x95\xb8</h1>");
	sprintf(html, "%s%s", html,		"OT(0.1C) <input type='text' name='_OT' value='");
	sprintf(html, "%s%d", html,		mGateway.uEMS.uSetting.OT.Value);
	sprintf(html, "%s%s", html,		"'>");
	
	sprintf(html, "%s%s", html,		"<p>");
	sprintf(html, "%s%s", html,		"UT(0.1C) <input type='text' name='_UT' value='");
	sprintf(html, "%s%d", html,		mGateway.uEMS.uSetting.UT.Value);
	sprintf(html, "%s%s", html,		"'>");
	
	sprintf(html, "%s%s", html,		"<p>");
	sprintf(html, "%s%s", html,		"wOT(0.1C) <input type='text' name='_wOT' value='");
	sprintf(html, "%s%d", html,		mGateway.uEMS.uSetting.wOT.Value);
	sprintf(html, "%s%s", html,		"'>");
	
	sprintf(html, "%s%s", html,		"<p>");
	sprintf(html, "%s%s", html,		"wUT(0.1C) <input type='text' name='_wUT' value='");
	sprintf(html, "%s%d", html,		mGateway.uEMS.uSetting.wUT.Value);
	sprintf(html, "%s%s", html,		"'>");
	
	sprintf(html, "%s%s", html,		"<p>");
	sprintf(html, "%s%s", html,		"Tolerance(0.1C) <input type='text' name='_Tol' value='");
	sprintf(html, "%s%d", html,		mGateway.uEMS.uSetting.Tolerance.Value);
	sprintf(html, "%s%s", html,		"'>");
	
	sprintf(html, "%s%s", html,		"<br><hr>");
	sprintf(html, "%s%s", html,		"<h1>\xe8\xa8\xad\xe5\x82\x99\xe8\xb3\x87\xe8\xa8\x8a</h1>");
	sprintf(html, "%s%s", html,		"<p>");
	sprintf(html, "%s%s", html,		"\xe7\xb7\xa8\xe8\x99\x9f");
	sprintf(html, "%s%s", html,		" <input type='text' name='_ID' value='");
	sprintf(html, "%s%s", html,		mGateway.uEMS.uModule.uID.Buffer);
	sprintf(html, "%s%s", html,		"'>");
	sprintf(html, "%s%s", html,		"<p>");
	sprintf(html, "%s%s", html,		"\xe4\xbf\xae\xe6\x94\xb9\xe7\x94\xa8\xe5\xaf\x86\xe7\xa2\xbc");
	sprintf(html, "%s%s", html,		" <input type='text' name='_IDpass' value=''>");

	
	sprintf(html, "%s%s", html,		"<br><hr>");
	sprintf(html, "%s%s", html,		"<input type='submit''>");
	sprintf(html, "%s%s", html,		"</form></font></html>\r\n");
	
	int len = strlen(html);
	sprintf(html2, "%s %d\r\n\r\n%s", header, len, html);
	Send_Html_String(&html2);
	debugSend_Html_String(&html2);	
}




//=================================================
#endif
//=================================================