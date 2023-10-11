
//=================================================
//	Sensor_inital_mem.c
//=================================================
void mem_inital(void);
void mem_main(void);

#define STDV_BASE_LINE							PLC_D_Buf[3]
#define mem_OFF_Count_Max						PLC_D_Buf[0]
#define mem_ON_Loop_Count_Max					PLC_D_Buf[1]
#define mem_NA									PLC_D_Buf[2]

#define TGS2600_SIGNED							PLC_D_Buf[6]
#define TGS2600_OFFSET							PLC_D_Buf[7]
#define TGS2620_SIGNED							PLC_D_Buf[9]
#define TGS2620_OFFSET							PLC_D_Buf[10]
#define HMI_NVM	547

#define Wait_Save_Count_Max						50
typedef struct mem
{
	unsigned char	NowStatus;
	bool			Change_Flag;
	unsigned int	Wait_Save_Count;
	bool			Debug_Flag;
}mem_t;
mem_t Sensor_mem;


typedef enum{
	mem_NOP,
	mem_CHECK,
	mem_Wait_SAVE,
	mem_Wait_SAVE_MSG,
	mem_SAVE,
	mem_LOAD
}mem_RunDown;