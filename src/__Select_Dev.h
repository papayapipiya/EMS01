
//	Repeater + HMI

//	#define Dev_Repeater
//	#define HMI_001

//------------------------------------------------------
//	Repeater + PLC DBuf
//	讀取PLC資料，但是借用PLC D暫存器當記憶體
/*
	#define Dev_Repeater
	#define Dev_Repeater_DBuf
*/
//------------------------------------------------------
//	Repeater + MCU RAM
//	讀取PLC資料，用MCU RAM當記憶體
/*
	#define Dev_Repeater
	#define Dev_Repeater_RAM
*/
//------------------------------------------------------
//	Master + 加密 = 超秦

//	#define Dev_Master_RS232
//	#define Dev_Master_RS232_Encoder		//超秦/
//------------------------------------------------------
//	Master
//	#define Dev_Master_RS232
//------------------------------------------------------
//	監看雞隻秤盤電池電量
//	#define Dev_Master_001
//------------------------------------------------------
//	#define Dev_Weight
//	#define Dev_Feed_Weigher
//	#define Dev_Water				
//------------------------------------------------------
//	#define Dev_PLC_001						//竑記
//	#define Dev_PLC_002						//力固
//	#define Dev_PLC_004						//百通飼料秤重
//------------------------------------------------------
//	#define Dev_Sensor
//	#define Dev_Sensor_HT_OnBoard			//溫濕度 + AI
//	#define Dev_Sensor_Module				//溫濕度外拉 + 氣體模組
//	#define Dev_Sensor_AI_OnBoard
//	#define Dev_Sensor_Module_NO_HT			//溫濕度 + 氣體模組
//------------------------------------------------------
//	#define SHH_FeedWeight_01				//自製飼料秤重
//------------------------------------------------------
//	Sensor 溫溼度 ＋ AI (電路版誤差校正)
/*
	#define Dev_Sensor
	#define Dev_Sensor_HT_OnBoard
	#define Dev_Sensor_AI_OnBoard
*/
//------------------------------------------------------
//	Sensor 外掛SHT31版本
/*
	#define Dev_Sensor
	#define Dev_Sensor_Module				//溫濕度外拉 + 氣體模組
	#define Dev_Sensor_AI_OnBoard
		
*/
//------------------------------------------------------


//------------------------------------------------------

	#define Dev_Master_RS232
	#define Dev_Wifi

//------------------------------------------------------
/*
	#define Dev_Sensor
	#define Dev_Sensor_HT_OnBoard
	#define Dev_Sensor_AI_OnBoard
	#define IIC_Dev
	
//	#define Sensor_TYPE_00	//衣櫃
	#define Sensor_TYPE_01	//鞋櫃
*/
//	#define Sensor_Offset
	
