/*
 * configure_i2c_master.h
 *
 * Created: 2023/9/24 下午 10:27:50
 *  Author: papay
 */ 


#ifndef CONFIGURE_I2C_MASTER_H_
#define CONFIGURE_I2C_MASTER_H_


//==============================
//	2023 09 17 SAMC21的設定
typedef struct IIC_Dev{
	uint16_t address;
	uint16_t Write_length;
	uint8_t *Write_Buf;
	uint16_t Read_length;
	uint8_t *Read_Buf;
	uint8_t *Read_Msg;
	uint8_t *MSG;
	uint8_t	*Debug;
	unsigned int Rundown_timer;
	unsigned int Rundown_timer_max;
	bool	Funtion_Enable;
}IIC_Dev_t;

typedef struct Item_Dev{
	IIC_Dev_t		*Read;
	IIC_Dev_t		*Write;
	bool			Flag;
	bool			SFirst_PowerON_Flag;
}Item_Dev_t;



#endif /* CONFIGURE_I2C_MASTER_H_ */