
/*
============================================
500 ~ 524 : Dev_List	
	Dev_List[i][Dev_No_Index] = i;
	Dev_List[i][Dev_En_Index] = Dev_Dis;
	Dev_List[i][2] = 0;	//Mac
	Dev_List[i][3] = 0;
	Dev_List[i][4] = 0;
	Dev_List[i][5] = 0;
	
525 : LoRA Define

526 ~ 534: Dev_List_Time_Counter 
	Dev_Time[i][0] = 0xff;
	Dev_Time[i][1] = 0xff;

535 ~ 545: Write PLC Table 	
	Write_PLC_List_Index[i].MAC_ID_Index = i;
	Write_PLC_List_Index[i].Channel_Index  = i;
	Write_PLC_List_Index[i].Channel_En = Dev_Dis;

546: Weight Correction Value
	RawDataZero : [0] ~ [3]
	RawDataUser : [4] ~ [7]
	RawDataBase : [8] ~ [11] 

547: Weight Correction Value
	RawDataZero : [0] ~ [3]
	RawDataUser : [4] ~ [7]
	RawDataBase : [8] ~ [11]
	
	
547: HMI數值
	RawDataZero : [0] ~ [3]
	RawDataUser : [4] ~ [7]
	RawDataBase : [8] ~ [11]


============================================
*/