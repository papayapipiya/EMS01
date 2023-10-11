/*
 * getSettingString.h
 *
 * Created: 2023/10/7 下午 03:39:28
 *  Author: papay
 */ 


#ifndef GETSETTINGSTRING_H_
#define GETSETTINGSTRING_H_


void getSettingString(uSetting_item_Dev_t *obj);

#define uSelect_userKeyin_Buffer	0
#define uSelect_Buffer				1
void userKeyin_Buffer_or_Buffer_to_value(bool tmpSelect, uSetting_item_Dev_t *obj);


#endif /* GETSETTINGSTRING_H_ */