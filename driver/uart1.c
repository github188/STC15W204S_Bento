/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           uart.c
** Created by:          yoc
** Created date:        2014-07-10
** Version:             V1.0 
** Descriptions:        ��������        
********************************************************************************************************/

#include "..\config.h"
#include <stdarg.h>



#define FOSC 			11059200L //33177600//22118400 // 


//unsigned char uartbuf[BUF_SIZE] = {0};//���ջ�����
unsigned char recvbuf[BUF_SIZE] = {0};//���ջ�����
volatile unsigned char tx;//������д����
volatile unsigned char isRequest = 0;
unsigned char uart1Init(void)
{
	uart1_clear();
	SCON  = 0X50; 		//8λ�ɱ䲨���ʣ�����żУ��	
	//BRT   = -(FCLIK/32/UART1_BAUD);
	AUXR  = 0x15; 
	REN   =	1;      		//�������
	T2L = (65536 - (FOSC/4/UART1_BAUD));   //���ò�������װֵ
  	T2H = (65536 - (FOSC/4/UART1_BAUD))>>8;
	
	AUXR |= 0x10;			//start timer2
	ES    =	1;   			// ������1�ж�
	EA    = 1;   		// �����ж�

	
	return 1;
}





void uart1_isr( void ) interrupt 4 using 1 
{
	unsigned char temp;

	if(TI)
	{
		TI = 0;
	}	
	if(RI)//recv isr
	{
		RI = 0;
		temp = SBUF;	
		if(isRequest == 1)
			return;
		if(temp == BT_START && tx == 0)//��ͷ
		{
			recvbuf[tx++] = temp;
		}
		else
		{
			if(tx > 0)//������������
			{
				recvbuf[tx++] = temp;
				if(tx >= PC_LEN)//��������
				{
					if(recvbuf[PC_END] == BT_PC_STOP)
					{
						temp = recvbuf[PC_ADDR]+recvbuf[PC_CMD]+recvbuf[PC_PARA];
						if(temp == recvbuf[PC_CRC])//У����ȷ
						{
							if((recvbuf[PC_ADDR] == st_A.addr) || 
								(recvbuf[PC_ADDR] == st_B.addr) || 
								(recvbuf[PC_ADDR] == 0xFF))//��Ϊ��������
							{																
								isRequest = 1;																
							}
						}
					}	
					tx = 0;
				}
				
			}
			
		}
		
	}
	

}


/*********************************************************************************************************
** Function name:     	uartPutChar
** Descriptions:	    ����һ�ֽ�����
** input parameters:    ch ���͵��ֽ�
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void uart1PutCh(unsigned char ch)
{
	//SetRS485AsTxdMode1();
	ES=0;
	TI=0;
	SBUF=ch;
	while(TI==0);
	TI=0;
	ES=1;

}




/*********************************************************************************************************
** Function name:     	uart1IsNotEmpty
** Descriptions:	    �жϽ��ջ�������Ϊ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char uart1_isRequest(void)
{
	return (isRequest);
}




/*********************************************************************************************************
** Function name:     	uartClear
** Descriptions:	    �崮�ڻ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void uart1_clear(void)
{
	tx = 0;
	isRequest = 0;
}










