



typedef	unsigned char		UINT8_t;		/*!< unsigned  8bit		*/
typedef unsigned short		UINT16_t;		/*!< unsigned 16bit		*/
typedef unsigned long		UINT32_t;		/*!< unsigned 32bit		*/


typedef struct
{
	UINT8_t		b0 :1;	/*!< byte<7>	*/
	UINT8_t		b1 :1;	/*!< byte<6>	*/
	UINT8_t		b2 :1;	/*!< byte<5>	*/
	UINT8_t		b3 :1;	/*!< byte<4>	*/
	UINT8_t		b4 :1;	/*!< byte<3>	*/
	UINT8_t		b5 :1;	/*!< byte<2>	*/
	UINT8_t		b6 :1;	/*!< byte<1>	*/
	UINT8_t		b7 :1;	/*!< byte<0>	*/
} BIT8_t;									/*!<  8bit structure		*/


typedef struct
{
	UINT8_t		L :4;	/*!< byte<6>	*/
	UINT8_t		H :4;	/*!< byte<7>	*/
} HL_t;									/*!<  8bit structure		*/

typedef struct
{
	UINT16_t	b15:1;	/*!< word<15>	*/
	UINT16_t	b14:1;	/*!< word<14>	*/
	UINT16_t	b13:1;	/*!< word<13>	*/
	UINT16_t	b12:1;	/*!< word<12>	*/
	UINT16_t	b11:1;	/*!< word<11>	*/
	UINT16_t	b10:1;	/*!< word<10>	*/
	UINT16_t	b9 :1;	/*!< word<9>	*/
	UINT16_t	b8 :1;	/*!< word<8>	*/

	UINT16_t	b7 :1;	/*!< word<7>	*/
	UINT16_t	b6 :1;	/*!< word<6>	*/
	UINT16_t	b5 :1;	/*!< word<5>	*/
	UINT16_t	b4 :1;	/*!< word<4>	*/
	UINT16_t	b3 :1;	/*!< word<3>	*/
	UINT16_t	b2 :1;	/*!< word<2>	*/
	UINT16_t	b1 :1;	/*!< word<1>	*/
	UINT16_t	b0 :1;	/*!< word<0>	*/
} BIT16_t;									/*!< 16bit structure		*/

typedef union
{
	UINT32_t	dword;
	UINT16_t	word[2];
	UINT8_t		byte[4];
} UINT32u_t;								/*!< 16bit union structure	*/
			
				
typedef union
{
	UINT16_t	word;
	UINT8_t		byte[2];
	BIT16_t		bit;
	//INT_HL_t	bHalf;
} UINT16u_t;								/*!< 16bit union structure	*/

typedef union
{
	UINT8_t		byte;
	BIT8_t		bit;
	HL_t		HL;
} UINT8u_t;									/*!<  8bit union structure	*/




#define KEY0	0
#define KEY1	1
#define KEY2	2
#define KEY3	3
#define KEY4	4
#define KEY5	5
#define KEY6	6
#define KEY7	7
#define KEY8	8
#define KEY9	9
#define KEYA	10
#define KEYB	11
#define KEYC	12
#define KEYD	13
#define KEYE	14
#define KEYF	15
#define KEYG	16
#define KEYH	17
#define KEYI	18
#define KEYJ	19
#define KEYK	20
#define KEYL	21
#define KEYM	22
#define KEYN	23
#define KEYO	24
#define KEYP	25
#define KEYQ	26
#define KEYR	27
#define KEYS	28
#define KEYT	29
#define KEYU	30
#define KEYV	31
#define KEYW	32
#define KEYX	33
#define KEYY	34
#define KEYZ	35