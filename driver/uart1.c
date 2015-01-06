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


#define BUF_SIZE 	    16 
#define FOSC 			11059200L 

unsigned char    uart1RxBuf[BUF_SIZE];//���ջ�����
unsigned char    uart1Rd;//������������
unsigned char    uart1Wd;//������д����

unsigned char uart1Init(void)
{
	uart1Clear();
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
	if(RI)//recv isr
	{
		RI = 0;
		uart1RxBuf[uart1Wd] = SBUF;
		uart1Wd = (++uart1Wd) % BUF_SIZE;
	}
	if(TI)
	{
		TI = 0;
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
** Function name:     	uartPutStr
** Descriptions:	    �����ַ���
** input parameters:    str �����ַ���ָ�� len ���ͳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void uart1PutStr(unsigned char *str,unsigned int len)
{
	unsigned int i;
	for(i = 0;i < len;i++)
		uart1PutCh(str[i]);				
}

/*********************************************************************************************************
** Function name:     	uart1IsNotEmpty
** Descriptions:	    �жϽ��ջ�������Ϊ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char uart1IsNotEmpty(void)
{
	if(uart1Rd == uart1Wd) return 0;
	else	return 1;
}

/*********************************************************************************************************
** Function name:     	uartGetChar
** Descriptions:	    ����һ�ֽ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

unsigned char uart1GetCh(void)
{
	unsigned char ch;
	ch =  uart1RxBuf[uart1Rd];
	uart1RxBuf[uart1Rd] = 0;	
	uart1Rd = (++uart1Rd) % BUF_SIZE;
	return ch;
}



/*********************************************************************************************************
** Function name:     	uartClear
** Descriptions:	    �崮�ڻ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void uart1Clear(void)
{
	uart1Wd = 0;
	uart1Rd = 0;
}





/*********************************************************************************************************
** Function name:	    Trace
** Descriptions:	    ����Trace
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
#if (BT_DEBUG == 1)
void trace(unsigned char *format,...)
{

	va_list arg_ptr;
	unsigned char  StringTemp[16];
	unsigned int  len;
	va_start(arg_ptr,format);
	len = vsprintf((char *)StringTemp,(const char *)format,arg_ptr);
	va_end(arg_ptr);

	uart1PutStr(StringTemp,len);

}

#endif	

