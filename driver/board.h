#ifndef _BOARD_H_
#define _BOARD_H_



#define FCLIK      11059200UL   /*ʹ��22.1184M����*/


//
sbit RS485_ENABLE 	= P3^2;  
sbit RS485_74HC123_nB 	= P1^1; 	//�����ش���



/*********************************************************************************************************
**�豸IO����
*********************************************************************************************************/
sbit IO_DOOR_A_OUT    	= P3^7;//A�� ��IO��
sbit IO_DOOR_A_PULSE    = P1^1;//A�� ��IO�� ����
sbit IO_DOOR_A_SIGNAL   = P3^6; //�͵�ƽΪ�������ߵ�ƽΪ ����
sbit IO_IR_A_OUT		= P5^5;//�ȴ򿪺��ⷢ��ܣ�P5.5��Ϊ�ߵ�ƽ��������Ҫ��P5.5�������Ϊ�͵�ƽ��
sbit IO_IR_A_SIGNAL		= P5^4;//��⣨P5.4��ƽ���ߵ�ƽΪ�޻����͵�ƽΪ�л���
sbit IO_LED_A		= P1^5;//P1.5��Ϊ�ߵ�ƽ�������ƣ��͵�ƽ�صơ�



sbit IO_DOOR_B_OUT    	= P3^3;//B�� ��IO��
sbit IO_DOOR_B_PULSE    = P1^1;//B�� ��IO�� ����
sbit IO_DOOR_B_SIGNAL   = P1^0; //B�͵�ƽΪ�����ɹ����ߵ�ƽΪ����ʧ��
sbit IO_IR_B_OUT		= P1^4;//
sbit IO_IR_B_SIGNAL		= P1^3;//B���ߵ�ƽΪ�޻����͵�ƽΪ�л���
sbit IO_LED_B		= P1^2;//��Ϊ�ߵ�ƽ�������ƣ��͵�ƽ�صơ�



// A B�����ַ
#define BT_CABINET_A	0
#define BT_CABINET_B	1
#define BT_CABINET_NO	0xAA
#define BT_CABINET_CON	0xFF  //��������

#define BT_OPEN_RCX		3



typedef struct _st_cabinet_data_{

	unsigned char addr;
	unsigned char goods;
	unsigned char door;

}ST_CABINET_DATA;

extern ST_CABINET_DATA st_A,st_B;



/*********************************************************************************************************
**����ͨ�ú꺯��
*********************************************************************************************************/

#define HUINT16(v)   	(((v) >> 8) & 0x0F)
#define LUINT16(v)   	((v) & 0x0F)
#define INTEG16(h,l)  	(((unsigned int)h << 8) | l)

#define DB_ledAControl(s)	do{IO_LED_A = s;}while(0)
#define DB_ledBControl(s)	do{IO_LED_B = s;}while(0)


void delayMs(unsigned int ms);
void systemInit(void);
void SetRS485AsTxdMode(void);
void SetRS485AsRxdMode(void);






unsigned char DB_openAdoor();
unsigned char DB_openBdoor();
unsigned char DB_AgoodsFull();
unsigned char DB_BgoodsFull();


unsigned char IAP_readByte(unsigned int addr);
void IAP_writeByte(unsigned int addr, unsigned char dat);
void IAP_eraseSector(unsigned int addr);


#endif
