#include "H/Function_Init.H"
/*****************************************************
*�������ƣ�void IO_Init(void)
*�������ܣ�IO��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void IO_Init(void)
{
	#if (IC_MODEL == SC95F8617B)  //SC92F8617���Ŷ���
	P0CON = 0xFF;  //����P0Ϊǿ����ģʽ
	P0PH  = 0x00;
	P1CON = 0xF5;  //����P1Ϊǿ����ģʽ
	P1PH  = 0x00;
	P2CON = 0x00;  //����P2Ϊ���������ģʽ
	P2PH  = 0xFF;
	P3CON = 0xFF;  //����P3Ϊǿ����ģʽ
	P3PH  = 0x00;
	P4CON = 0xFF;  //����P4Ϊǿ����ģʽ
	P4PH  = 0x01;
	P5CON = 0xFF;  //����P5Ϊǿ����ģʽ
	P5PH  = 0x00;
	#endif
	
	#if (IC_MODEL == SC95F8616B)  //SC92F8616���Ŷ���
	P0CON = 0xFF;  //����P0Ϊǿ����ģʽ
	P0PH  = 0x00;
	P1CON = 0x00;  //����P1Ϊ��������ģʽ
	P1PH  = 0x00;
	P2CON = 0x00;  //����P2Ϊ���������ģʽ
	P2PH  = 0xFF;
	P3CON = 0xFF;  //����P3Ϊǿ����ģʽ
	P3PH  = 0x00;
	P4CON = 0xFF;  //����P4Ϊǿ����ģʽ
	P4PH  = 0x01;
	P5CON = 0xFF;  //����P5Ϊǿ����ģʽ
	P5PH  = 0x00;
	SC95F8616B_NIO_Init(); //δ����IO������
	#endif
    
	#if (IC_MODEL == SC95F8615B)  //SC92F8615���Ŷ���
	P0CON = 0xFF;  //����P0Ϊǿ����ģʽ
	P0PH  = 0x00;
	P1CON = 0x00;  //����P1Ϊ��������ģʽ
	P1PH  = 0x00;
	P2CON = 0x00;  //����P2Ϊ���������ģʽ
	P2PH  = 0xFF;
	P3CON = 0xFF;  //����P3Ϊǿ����ģʽ
	P3PH  = 0x00;
	P4CON = 0xFF;  //����P4Ϊǿ����ģʽ
	P4PH  = 0x01;
	P5CON = 0xFF;  //����P5Ϊǿ����ģʽ
	P5PH  = 0x00;
	SC95F8615B_NIO_Init(); //δ����IO������
	#endif
	
	#if (IC_MODEL == SC95F8613B)  //SC92F8613���Ŷ���
	P0CON = 0xFF;  //����P0Ϊǿ����ģʽ
	P0PH  = 0x00;
	P1CON = 0x00;  //����P1Ϊ��������ģʽ
	P1PH  = 0x00;
	P2CON = 0x00;  //����P2Ϊ���������ģʽ
	P2PH  = 0xFF;
	P3CON = 0xFF;  //����P3Ϊǿ����ģʽ
	P3PH  = 0x00;
	P4CON = 0xFF;  //����P4Ϊǿ����ģʽ
	P4PH  = 0x00;
	SC95F8613B_NIO_Init(); //δ����IO������
	#endif
}

