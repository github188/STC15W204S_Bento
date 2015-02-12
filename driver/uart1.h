/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           uart.h
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        ��������        
********************************************************************************************************/
#ifndef _UART1_H_
#define _UART1_H_


#define BT_DEBUG	0

/*********************************************************************************************************
** Э����궨��  
** add by yoc
*********************************************************************************************************/
#define BT_START			0xBB
#define BT_PC_STOP			0x03
#define BT_STOP				0xFE

#define BT_OPEN_REQ			0x0A	//��ʾ����������Ӧ���ӵ�����
#define BT_DOOR_STATE_REQ	0x0B	//��ʾ��״̬��⣬����Ӧ���ӵ���״̬��
#define BT_IR_STATE_REQ		0x0C	//��ʾ�����߼�⣬����Ӧ���ӵĴ���״̬���Ƿ�Ϊ���䣻
#define BT_REAL_TIME_REQ	0x0D   //��ʾʵʱ��״̬��ʵʱ��Ʒ״̬


#define BT_CONFIG_START_REQ		0xF3  //��ʼ��־
#define BT_CONFIG_B_REQ		0xF2  //b�����õ�ַ����
#define BT_CONFIG_A_REQ		0xF1  //a�����õ�ַ����
#define BT_CONFIG_TEST_REQ	0xF8  //����ģʽ����
#define BT_CONFIG_CHECK_REQ	0xF9  //��ѯ��ַ����

#define BT_ADDR_FLASH_PAGE	0x0000


#define BT_CALL_DIS			0 //�������

#define PC_HEAD 		0
#define PC_ADDR			1
#define PC_CMD			2
#define PC_PARA			3
#define PC_CRC			4
#define PC_END			5
#define PC_LEN			6





#define UART1_BAUD		(9600) 

#define BUF_SIZE 	    8 
extern unsigned char recvbuf[BUF_SIZE];


unsigned char uart1Init(void);
void  uart1PutCh( unsigned char ch );
unsigned char uart1_isRequest(void);
void uart1_clear(void);



#endif
