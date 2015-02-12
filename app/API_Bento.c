/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           API_bento.c	 
** Created by:          yoc
** Created date:        2014-12-10
** Version:             V1.0 
** Descriptions:        ������ͨ��Э��ӿ�  ���к�������BT ��ͷ �� bento   
********************************************************************************************************/
#include "..\config.h"
#include "API_Bento.h"




static unsigned char cur_cabinet = BT_CABINET_NO;





/*********************************************************************************************************
** Function name:     	BT_read_flash
** Descriptions:	    ��ȡflash����
** input parameters:   
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void BT_read_flash(void)
{
	unsigned char crc;
	st_A.addr = IAP_readByte(BT_FLASH_ADRR);
	st_B.addr = IAP_readByte(BT_FLASH_ADRR + 4);
	crc = IAP_readByte(BT_FLASH_ADRR + 8);
	if(crc != 0xE5)
	{
		st_A.addr = 31;
		st_B.addr = 21;
	}	
}



/*********************************************************************************************************
** Function name:     	BT_read_flash
** Descriptions:	    ��ȡflash����
** input parameters:   
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void BT_write_flash(unsigned char cabinet,unsigned char addr)
{
	if(cabinet == BT_CABINET_A)
		st_A.addr = addr;
	else if(cabinet == BT_CABINET_B)
		st_B.addr = addr;
	else return;
	IAP_eraseSector(0x0000);
	IAP_writeByte(BT_FLASH_ADRR, st_A.addr);
	IAP_writeByte(BT_FLASH_ADRR + 4, st_B.addr);
	IAP_writeByte(BT_FLASH_ADRR + 8, 0xE5);
}







/*********************************************************************************************************
** Function name:     	BT_send_data
** Descriptions:	    �������Ӧ������
** input parameters:    cmd  ������
** output parameters:   ��
** Returned value:      1�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char BT_send_data(unsigned char data1,unsigned char data2)
{
	unsigned char crc = 0;
	crc = recvbuf[PC_ADDR] + recvbuf[PC_CMD] + data1  + data2;
	SetRS485AsTxdMode();
	uart1PutCh(BT_START);	//ͬ����
	uart1PutCh(recvbuf[PC_ADDR]);		//���Ӻ���
	uart1PutCh(recvbuf[PC_CMD]);		//������
	uart1PutCh(data1);		//��״̬
	uart1PutCh(data2);		//��Ʒ״̬
	uart1PutCh(crc);		//У����
	uart1PutCh(BT_STOP);		//������
	SetRS485AsRxdMode();
	return 1;
}


/*********************************************************************************************************
** Function name:     	BT_send_state
** Descriptions:	    �������Ӧ״̬��
** input parameters:    
** output parameters:   ��
** Returned value:      1�ɹ�  0ʧ��
*********************************************************************************************************/
unsigned char BT_send_state()
{
	SetRS485AsTxdMode();
	uart1PutCh(BT_START);	//ͬ����
	uart1PutCh(st_A.addr);		//����A����
	uart1PutCh(st_A.door);		//��״̬
	uart1PutCh(st_A.goods);		//��Ʒ״̬
	
	uart1PutCh(st_B.addr);		//����B����
	uart1PutCh(st_B.door);		//��״̬
	uart1PutCh(st_B.goods);		//��Ʒ״̬
	uart1PutCh(BT_STOP);		//������
	SetRS485AsRxdMode();
	return 1;
}


/*********************************************************************************************************
** Function name:     	BT_recv_cmd
** Descriptions:	    ������������ݰ�
** input parameters:    
** output parameters:   ��
** Returned value:      0 �����ݽ���  1������ȷ����  2�������ݴ���
*********************************************************************************************************/
unsigned char BT_recv_cmd()
{
	if(uart1_isRequest())
	{
		return 1;
	}
	else
		return 0;
}



/*********************************************************************************************************
** Function name:     	BT_handle_req
** Descriptions:	    ������������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void BT_handle_req()
{
	
	if(recvbuf[PC_CMD] == BT_OPEN_REQ) //��������
	{
		if(recvbuf[PC_ADDR] == st_A.addr)
		{
			st_A.door = DB_openAdoor();
			st_A.goods = DB_AgoodsFull();
			BT_send_data(st_A.door,st_A.goods);
		}			
		else
		{
			st_B.door = DB_openBdoor();
			st_B.goods = DB_BgoodsFull();
			BT_send_data(st_B.door,st_B.goods);
		}
	}
	else if(recvbuf[PC_CMD] == BT_DOOR_STATE_REQ || recvbuf[PC_CMD] == BT_IR_STATE_REQ)//��ѯ��״̬//��ѯ������״̬
	{
		if(recvbuf[PC_ADDR] == st_A.addr)
		{
			st_A.goods = DB_AgoodsFull();
			BT_send_data(st_A.door,st_A.goods);	
		}	
		else
		{
			st_B.goods = DB_BgoodsFull();
			BT_send_data(st_B.door,st_B.goods);
		}
			
	}
	else if(recvbuf[PC_CMD] == BT_REAL_TIME_REQ)//��ѯʵʱ״̬
	{
		st_A.goods = DB_AgoodsFull();
		st_B.goods = DB_BgoodsFull();
		BT_send_state();
	}
		
	else 
		_nop_();
	
	

}


/*********************************************************************************************************
** Function name:     	BT_config_req
** Descriptions:	    ��������
** input parameters:    
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void BT_config_req()
{
	unsigned char data1,data2;
	if(recvbuf[PC_CMD] == BT_CONFIG_START_REQ)
	{
		data1 = rand();
		data2 = data1 + 1;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_A_REQ)
	{
		BT_write_flash(BT_CABINET_A,recvbuf[PC_PARA]);
		data1 = 0;data2 = 0;
		BT_read_flash();
		recvbuf[PC_ADDR] = st_A.addr;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_B_REQ)
	{
		BT_write_flash(BT_CABINET_B,recvbuf[PC_PARA]);
		data1 = 0;data2 = 0;	
		BT_read_flash();
		recvbuf[PC_ADDR] = st_B.addr;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_CHECK_REQ)
	{
		data1 = st_A.addr;
		data2 = st_B.addr;
	}
	else if(recvbuf[PC_CMD] == BT_CONFIG_TEST_REQ)//������������ģʽ
	{
		if(DB_AgoodsFull()) 
		{
			if(st_A.istest == 0)
			{
				st_A.istest = 1;
				DB_openAdoor();
			}
		}
		else
			st_A.istest = 0;
		if(DB_BgoodsFull()) 
		{
			if(st_B.istest == 0)
			{
				st_B.istest = 1;
				DB_openBdoor();
			}
			
		}
		else
			st_B.istest = 0;
		return;
	}
	else
		return;
	BT_send_data(data1, data2);

}





/*********************************************************************************************************
** Function name:     	BT_task
** Descriptions:	    ����������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void BT_task(void)
{
	unsigned char res;
	res = BT_recv_cmd();//�������� 
	if(res == 1)//�л�Ӧ ����������ȷ 
	{
		if(recvbuf[PC_ADDR] == 0xFF) //����ģʽ
		{
			BT_config_req();
		}
		else if(recvbuf[PC_ADDR] == st_A.addr || recvbuf[PC_ADDR] == st_B.addr)//������
		{
			BT_handle_req();//�������� �����ͻ�Ӧ
		}
		else  //���Ǳ���������ֱ������
			_nop_();

		uart1_clear();
	}
	else
		_nop_();

}


