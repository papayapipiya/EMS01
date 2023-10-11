
//=============================================================
struct i2c_master_module i2c_master_instance;
struct i2c_master_packet packet;
#define IIC_PA0809	&i2c_master_instance
#define TIMEOUT  10000



typedef enum{
	Reset_Start,
	Reset_Low,
	Reset_High,
	Reset_End,
	Reset_Wait,
	Reset_Nop
}Reset_RunDown;



Item_Dev_t	define_eID;
Item_Dev_t	define_eSSID;
Item_Dev_t	define_ePW;
Item_Dev_t	define_eOT;
Item_Dev_t	define_eUT;
Item_Dev_t	define_ewOT;
Item_Dev_t	define_ewUT;
Item_Dev_t	define_eTolerance;

//==============================

typedef struct IIC_Device_List{
	uint16_t address;
	uint16_t Write_length;
	uint16_t Read_length;
	uint8_t *Write_Buf;
	uint8_t *MSG;
}IIC_Device_List_t;

void IIC_EEPROM_Inital(void);
void IIC_Main_Func(void);
void configure_i2c_master(void);
void IIC_Func(IIC_Device_List_t tmp_Format);
void GPIO_inital_Func(void);
void GPIO_Output_Func(const uint8_t gpio_pin, bool tmp_bool);
void Reset_NH3_CO_Sensor(void);





bool Show_Flag;





//// Device Addr
//// 不用含Write/Read Bit
//#define SLAVE_ADDRESS 0x50
//
////	Write Format
////	AT24C02 Page-Write Number is 8-byte
//#define Write_DATA_LENGTH 9
//static uint8_t write_Data_buffer[Write_DATA_LENGTH] = {
	//0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
	////	Addr, Data -------------------------------------Data
//};
//static uint8_t write_Data_buffer02[Write_DATA_LENGTH] = {
	//0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
	////	Addr, Data -------------------------------------Data
//};
//
////	Write Dummy
//#define Write_Dummy_LENGTH 1
//static uint8_t Write_Dummy_buffer[Write_Dummy_LENGTH] = {
	//0x00
	////	Addr
//};
//
//#define Read_DATA_LENGTH 30
//static uint8_t read_buffer[Read_DATA_LENGTH];





// Device Addr
// 不用含Write/Read Bit
#define SLAVE_ADDRESS 0x44

//	Write Dummy
#define Write_Dummy_LENGTH 2
static uint8_t Write_Dummy_buffer[Write_Dummy_LENGTH] =
{
	0x24, 0x0B
};

static uint8_t Write_disable_heater[Write_Dummy_LENGTH] =
{
	0x30, 0x66
};

#define Read_DATA_LENGTH 100
static uint8_t read_buffer[Read_DATA_LENGTH];


void Read_SHT30_Sensor(void);

unsigned int IC_Temp;
unsigned int IC_Humi;
