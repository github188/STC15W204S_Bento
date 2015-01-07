/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           API_bento.h
** Created by:          yoc
** Created date:        2014-12-10
** Version:             V1.0 
** Descriptions:        ������������ͨ��Э��        
********************************************************************************************************/
#ifndef _API_BENTO_H_
#define _API_BENTO_H_

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




void BT_task(void);

void BT_read_flash(void);
#endif
