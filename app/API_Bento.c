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

// A B�����ַ
#define BT_CABINET_A	0
#define BT_CABINET_B	1
#define BT_CABINET_NO	0xAA
#define BT_CABINET_CON	0xFF  //��������



static unsigned char cur_cabinet = BT_CABINET_NO;
static unsigned char cur_addr = 0,cur_cmd = 0,cur_para = 0xFF,cur_crc;




ST_CABINET_DATA st_A,st_B;




/*********************************************************************************************************
** Function name:     	BT_read_flash
** Descriptions:	    ��ȡflash����
** input parameters:   
** output parameters:   ��
** Returned value:      
*********************************************************************************************************/
void BT_read_flash(void)
{
	st_A.addr = IAP_readByte(0x0000);
	st_B.addr = IAP_readByte(0x0004);
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
	IAP_writeByte(0x0000, st_A.addr);
	IAP_writeByte(0x00004, st_B.addr);
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
	crc = cur_addr + cur_cmd + data1  + data2;
	uart1Clear();
	SetRS485AsTxdMode();
	uart1PutCh(BT_START);	//ͬ����
	uart1PutCh(cur_addr);		//���Ӻ���
	uart1PutCh(cur_cmd);		//������
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
	uart1Clear();
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
	unsigned char temp;
	if(!uart1IsNotEmpty())
		return 0;
	temp = uart1GetCh();
	if(temp != BT_START)
		return 2;
	_nop_();
	cur_addr = uart1GetCh();cur_crc = cur_addr;
	cur_cmd = uart1GetCh();	cur_crc += cur_cmd;
	cur_para = uart1GetCh();cur_crc += cur_para;	

	temp = uart1GetCh();//crc	
	if(temp != cur_crc)//У���벻��ȷ
		return 2;
	temp = uart1GetCh();//end
	if(temp != BT_PC_STOP)//��������ȷ
		return 2;
	
	return 1;


	
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
	
	if(cur_cmd == BT_OPEN_REQ) //��������
	{
		if(cur_addr == st_A.addr)
		{
			DB_opendoor(BT_CABINET_A);
			DB_goodsNotEmpty(1);
			DB_ledControl(1);
			BT_send_data(st_A.door,st_A.goods);
		}			
		else
		{
			DB_opendoor(BT_CABINET_B);
			BT_send_data(st_B.door,st_B.goods);
		}
	}
	else if(cur_cmd == BT_DOOR_STATE_REQ || cur_cmd == BT_IR_STATE_REQ)//��ѯ��״̬//��ѯ������״̬
	{
		if(cur_addr == st_A.addr)
			BT_send_data(st_A.door,st_A.goods);		
		else
			BT_send_data(st_B.door,st_B.goods);
	}
	else if(cur_cmd == BT_REAL_TIME_REQ)//��ѯʵʱ״̬
		BT_send_state();
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
	if(cur_cmd == BT_CONFIG_START_REQ)
	{
		data1 = rand();
		data2 = data1 + 1;
	}
	else if(cur_cmd == BT_CONFIG_A_REQ)
	{
		BT_write_flash(BT_CABINET_A,cur_para);
		data1 = 0;data2 = 0;
		cur_addr = cur_para;
	}
	else if(cur_cmd == BT_CONFIG_B_REQ)
	{
		BT_write_flash(BT_CABINET_B,cur_para);
		data1 = 0;data2 = 0;
		cur_addr = cur_para;
	}
	else if(cur_cmd == BT_CONFIG_CHECK_REQ)
	{
		data1 = st_A.addr;
		data2 = st_B.addr;
	}
	else if(cur_cmd == BT_CONFIG_TEST_REQ)//������������ģʽ
	{
	
	}
	else
		return;
	BT_send_data(data1, data1);

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
		if(cur_addr == 0xFF) //����ģʽ
		{
			BT_config_req();
		}
		else if(cur_addr == st_A.addr || cur_addr == st_B.addr)//������
		{
			BT_handle_req();//�������� �����ͻ�Ӧ
		}
		else  //���Ǳ���������ֱ������
			_nop_();
	}

}


