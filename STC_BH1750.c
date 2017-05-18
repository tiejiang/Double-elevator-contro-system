
#include<reg52.h>         //���ļ�
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>
#define   uchar unsigned char
#define   uint unsigned int	
//#define   DataPort P0	 //LCD1602���ݶ˿�
#define	  SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
//¥��䰴��
sbit	  First_floor = P0^0;      //һ¥����
sbit	  Second_floor = P0^1;      //��¥����
sbit	  Third_floor = P0^2;      //��¥����
sbit	  Fourth_floor = P0^3;      //��¥����
//����A�����ڰ���
sbit	  ElevatorA_Inner_First = P0^4;      
sbit	  ElevatorA_Inner_Second = P0^5;     
sbit	  ElevatorA_Inner_Third = P0^6;      
sbit	  ElevatorA_Inner_Fourth = P0^7;     
//����B�����ڰ���
sbit	  ElevatorB_Inner_First = P2^0;     
sbit	  ElevatorB_Inner_Second = P2^1;    
sbit	  ElevatorB_Inner_Third = P2^2;     
sbit	  ElevatorB_Inner_Fourth = P2^3;    


sbit      MQ_7_DETECT = P2^0; //���������������
sbit      MQ_2_DETECT = P2^1; //

sbit      KITCHEN = P2^2;     //����
sbit      LIVING_ROOM = P2^3;  //����
sbit      DINING_ROOM = P2^4;  //����
sbit      BED_ROOM = P2^5;  //����
sbit      TV_SWITCH = P2^7;            //���ӻ�����
//�����������
/***************
���߷�ʽ:
IN1 ---- P10
IN2 ---- P11
IN3 ---- P12
IN4 ---- P13
+   ---- +5V
-   ---- GND
*********************/
//��ת �����ͨ���� D-C-B-A
//��ת �����ͨ���� A-B-C-D

//����A���
sbit      ElevatroA_a = P1^0;     //����A a��
sbit      ElevatroA_b = P1^1;     //����A b��
sbit      ElevatroA_c = P1^2;     //����A c��
sbit      ElevatroA_d = P1^3;     //����A d��
//����B���
sbit      ElevatroB_a = P1^4;     //����B a��
sbit      ElevatroB_b = P1^5;     //����B b��
sbit      ElevatroB_c = P1^6;     //����B c��
sbit      ElevatroB_d = P1^7;     //����B d��
uchar is_close = 1; //��־��բ�Ƿ�ر� 0 δ�رգ�1 �ر�


uchar rec_dat[9];   //������ʾ�Ľ�����������
uchar  outdata[6];  //���巢�͵��ֽ��� 
uchar  count, count_r=0;
uchar  str[5]={"RS232"};
uchar  indata[5];  //���������ֽ���
typedef   unsigned char BYTE;
typedef   unsigned short WORD;

BYTE    BUF[8];                         //�������ݻ�����      	
uchar   ge,shi,bai,qian,wan;            //��ʾ����
int     dis_data;                       //����

void delay_nms(unsigned int k);
//------------------------------------
void Delay5us();
void Delay5ms();

//ms��ʱ����
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
/*����Aת�����ƺ���*/

//A˳ʱ��ת��
void Motor_A_CW(void)
{
	ElevatroA_a = 1; 
	ElevatroA_b = 0;
	ElevatroA_c = 0;
    ElevatroA_d = 0; 
	Delay_xms(4);

	ElevatroA_a = 0; 
	ElevatroA_b = 1;
	ElevatroA_c = 0;
    ElevatroA_d = 0; 
	Delay_xms(4);

	ElevatroA_a = 0; 
	ElevatroA_b = 0;
	ElevatroA_c = 1;
    ElevatroA_d = 0; 
	Delay_xms(4);

	ElevatroA_a = 0; 
	ElevatroA_b = 0;
	ElevatroA_c = 0;
    ElevatroA_d = 1; 
  	Delay_xms(4);
}
//A��ʱ��ת��
void Motor_A_CCW(void)
{
	ElevatroA_a = 0; 
	ElevatroA_b = 0;
	ElevatroA_c = 0;
    ElevatroA_d = 1; 
	Delay_xms(4);

	ElevatroA_a = 0; 
	ElevatroA_b = 0;
	ElevatroA_c = 1;
    ElevatroA_d = 0; 
	Delay_xms(4);

	ElevatroA_a = 0; 
	ElevatroA_b = 1;
	ElevatroA_c = 0;
    ElevatroA_d = 0; 
	Delay_xms(4);

	ElevatroA_a = 1; 
	ElevatroA_b = 0;
	ElevatroA_c = 0;
    ElevatroA_d = 0; 
  	Delay_xms(4);
}
//Aֹͣת��
void Motor_A_stop(void)
{
	ElevatroA_a = 0; 
	ElevatroA_b = 0;
	ElevatroA_c = 0;
    ElevatroA_d = 0;
}

/*����Bת�����ƺ���*/

//B˳ʱ��ת��
void Motor_B_CW(void)
{
	ElevatroB_a = 1; 
	ElevatroB_b = 0;
	ElevatroB_c = 0;
    ElevatroB_d = 0; 
	Delay_xms(4);

	ElevatroB_a = 0; 
	ElevatroB_b = 1;
	ElevatroB_c = 0;
    ElevatroB_d = 0; 
	Delay_xms(4);

	ElevatroB_a = 0; 
	ElevatroB_b = 0;
	ElevatroB_c = 1;
    ElevatroB_d = 0; 
	Delay_xms(4);

	ElevatroB_a = 0; 
	ElevatroB_b = 0;
	ElevatroB_c = 0;
    ElevatroB_d = 1; 
  	Delay_xms(4);
}
//B��ʱ��ת��
void Motor_B_CCW(void)
{
	ElevatroB_a = 0; 
	ElevatroB_b = 0;
	ElevatroB_c = 0;
    ElevatroB_d = 1; 
	Delay_xms(4);

	ElevatroB_a = 0; 
	ElevatroB_b = 0;
	ElevatroB_c = 1;
    ElevatroB_d = 0; 
	Delay_xms(4);

	ElevatroB_a = 0; 
	ElevatroB_b = 1;
	ElevatroB_c = 0;
    ElevatroB_d = 0; 
	Delay_xms(4);

	ElevatroB_a = 1; 
	ElevatroB_b = 0;
	ElevatroB_c = 0;
    ElevatroB_d = 0; 
  	Delay_xms(4);
}
//ֹͣ
void Motor_B_stop(void)
{
	ElevatroB_a = 0; 
	ElevatroB_b = 0;
	ElevatroB_c = 0;
    ElevatroB_d = 0; 
}

//������ʱ**************************
void delay_nms(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}

/**************************************
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5ms()
{
    WORD n = 560;

    while (n--);
}
void DHT11_delay_us(uchar n)
{
    while(--n);
}

void DHT11_delay_ms(uint z)
{
   uint i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}
/********************************************************************
                            ���ڳ�ʼ��
*********************************************************************/
void uartInit(){
	/*--------------------------------------
	Set serial port for 9600 baud at
	11.0592 MHz.  Note that we use Timer 1
	for the baud rate generator.
	--------------------------------------*/
	SCON  = 0x50;
	TMOD |= 0x20;
	TH1   = 0xFA;
	TR1   = 1;
	TI    = 1;
	PCON |= 0x80;

	printf ("\r\nATMOSPHERE Program\r\n");
	printf ("\r\nUART START\r\n\r\n");
	/*--------------------------------------
	Set Timer0 for 16-bit counter mode.
	Set the Timer0 Run control bit.
	--------------------------------------*/
	TMOD = (TMOD & 0xF0) | 0x05;
	TR0 = 1;	
}

//�жϳ�ʼ��
void init(){
 /* ϵͳ��ʼ�� */
    TMOD = 0x20;          //��ʱ��T1ʹ�ù�����ʽ2
    TH1 = 253;        // ���ó�ֵ
    TL1 = 253;
    TR1 = 1;          // ��ʼ��ʱ
    SCON = 0x50;          //������ʽ1��������9600bps���������   
    ES = 1;
    EA = 1;           // �������ж�   
    TI = 0;
    RI = 0;
	DHT11_delay_us(100);
}


//*********************************************************
//������********
//*********************************************************
void main()
{  
	uchar i;
	uchar j;
	uchar  light_intensity;
	float temp;
    //uartInit();
	init();
   delay_nms(1500);	    //��ʱ100ms
  while(1)              //ѭ��
  {
		if(First_floor == 0){
			for(j=0;j<100;j++){
			//	MotorCW();   //˳ʱ��ת��
			Motor_A_CW();
			Motor_B_CW();  //���Bת������
			} 
		//	MotorStop();  //ֹͣת��
			Motor_B_stop();
			Motor_B_stop();
			DHT11_delay_ms(2000); //��ʱ �ȴ�����ͨ��
		}

		if(Second_floor == 0){
			for(j=0;j<100;j++){
			//	MotorCW();   //˳ʱ��ת��
			Motor_A_CCW();
			Motor_B_CCW();  //��ʱ��ת��
			} 
		//	MotorStop();  //ֹͣת��
			Motor_A_stop();
			Motor_B_stop();
			DHT11_delay_ms(2000); //��ʱ �ȴ�����ͨ��
		}
		/*
		if(Third_floor == 0){
			for(j=0;j<100;j++){
				MotorCW();   //˳ʱ��ת��
			} 
			MotorStop();  //ֹͣת��
			DHT11_delay_ms(2000); //��ʱ �ȴ�����ͨ��
		}

		if(Fourth_floor == 0){
			for(j=0;j<100;j++){
				MotorCW();   //˳ʱ��ת��
			} 
			MotorStop();  //ֹͣת��
			DHT11_delay_ms(2000); //��ʱ �ȴ�����ͨ��
		}
		*/
	/*
		for(j=0;j<100;j++){
			MotorCW();   //˳ʱ��ת��
		} 
		MotorStop();  //ֹͣת��
		DHT11_delay_ms(2000); //��ʱ �ȴ�����ͨ��
		for(j=0;j<100;j++){
		   MotorCCW();  //��ʱ��ת��
		} 
		MotorStop();  //ֹͣת��
		DHT11_delay_ms(2000);
	*/	
  }
}

void RSINTR() interrupt 4 using 2
{
        uchar InPut3;
        if(TI==1) //�����ж�          
        {
                TI=0;
                if(count!=3) //������3λ����         
                {
                        SBUF= outdata[count];
                        count++;
                }
        }

        if(RI==1)         //�����ж�                  
        {        
                InPut3=SBUF;
                indata[count_r]=InPut3;
                count_r++;
                RI=0;                                                                 
                if (count_r==1)//������4λ���� 
                {
                //���ݽ�����ϴ���
			
                        count_r=0;

                str[0]=indata[0];
				/*
				if(str[0] == NULL){
					TV_SWITCH = 0;       //	��ʼ״̬����Ϊ�̵��������ӿ��أ�δ����-�͵�ƽ����
					KITCHEN = 0;
					LIVING_ROOM = 0;
					DINING_ROOM = 0;
					BED_ROOM = 0;
				}
				*/
				if(str[0] == 'S'){		
					is_close = 0;  //��⳵��OK������
				}
				else if(str[0] == 'a'){
					BED_ROOM = 0;				
				}
             }
        }
} 

