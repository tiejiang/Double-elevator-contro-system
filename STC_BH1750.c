
#include<reg52.h>         //库文件
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>
#define   uchar unsigned char
#define   uint unsigned int	
//#define   DataPort P0	 //LCD1602数据端口
#define	  SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
//楼层间按键
sbit	  First_floor = P0^0;      //一楼按键
sbit	  Second_floor = P0^1;      //二楼按键
sbit	  Third_floor = P0^2;      //三楼按键
sbit	  Fourth_floor = P0^3;      //四楼按键
//电梯A轿厢内按键
sbit	  ElevatorA_Inner_First = P0^4;      
sbit	  ElevatorA_Inner_Second = P0^5;     
sbit	  ElevatorA_Inner_Third = P0^6;      
sbit	  ElevatorA_Inner_Fourth = P0^7;     
//电梯B轿厢内按键
sbit	  ElevatorB_Inner_First = P2^0;     
sbit	  ElevatorB_Inner_Second = P2^1;    
sbit	  ElevatorB_Inner_Third = P2^2;     
sbit	  ElevatorB_Inner_Fourth = P2^3;    


sbit      MQ_7_DETECT = P2^0; //空气质量监测引脚
sbit      MQ_2_DETECT = P2^1; //

sbit      KITCHEN = P2^2;     //厨房
sbit      LIVING_ROOM = P2^3;  //客厅
sbit      DINING_ROOM = P2^4;  //餐厅
sbit      BED_ROOM = P2^5;  //卧室
sbit      TV_SWITCH = P2^7;            //电视机开关
//窗帘电机控制
/***************
接线方式:
IN1 ---- P10
IN2 ---- P11
IN3 ---- P12
IN4 ---- P13
+   ---- +5V
-   ---- GND
*********************/
//正转 电机导通相序 D-C-B-A
//反转 电机导通相序 A-B-C-D

//电梯A电机
sbit      ElevatroA_a = P1^0;     //电梯A a相
sbit      ElevatroA_b = P1^1;     //电梯A b相
sbit      ElevatroA_c = P1^2;     //电梯A c相
sbit      ElevatroA_d = P1^3;     //电梯A d相
//电梯B电机
sbit      ElevatroB_a = P1^4;     //电梯B a相
sbit      ElevatroB_b = P1^5;     //电梯B b相
sbit      ElevatroB_c = P1^6;     //电梯B c相
sbit      ElevatroB_d = P1^7;     //电梯B d相
uchar is_close = 1; //标志车闸是否关闭 0 未关闭，1 关闭


uchar rec_dat[9];   //用于显示的接收数据数组
uchar  outdata[6];  //定义发送的字节数 
uchar  count, count_r=0;
uchar  str[5]={"RS232"};
uchar  indata[5];  //接收数据字节数
typedef   unsigned char BYTE;
typedef   unsigned short WORD;

BYTE    BUF[8];                         //接收数据缓存区      	
uchar   ge,shi,bai,qian,wan;            //显示变量
int     dis_data;                       //变量

void delay_nms(unsigned int k);
//------------------------------------
void Delay5us();
void Delay5ms();

//ms延时函数
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
/*电梯A转动控制函数*/

//A顺时针转动
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
//A逆时针转动
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
//A停止转动
void Motor_A_stop(void)
{
	ElevatroA_a = 0; 
	ElevatroA_b = 0;
	ElevatroA_c = 0;
    ElevatroA_d = 0;
}

/*电梯B转动控制函数*/

//B顺时针转动
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
//B逆时针转动
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
//停止
void Motor_B_stop(void)
{
	ElevatroB_a = 0; 
	ElevatroB_b = 0;
	ElevatroB_c = 0;
    ElevatroB_d = 0; 
}

//毫秒延时**************************
void delay_nms(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}

/**************************************
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
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
                            串口初始化
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

//中断初始化
void init(){
 /* 系统初始化 */
    TMOD = 0x20;          //定时器T1使用工作方式2
    TH1 = 253;        // 设置初值
    TL1 = 253;
    TR1 = 1;          // 开始计时
    SCON = 0x50;          //工作方式1，波特率9600bps，允许接收   
    ES = 1;
    EA = 1;           // 打开所以中断   
    TI = 0;
    RI = 0;
	DHT11_delay_us(100);
}


//*********************************************************
//主程序********
//*********************************************************
void main()
{  
	uchar i;
	uchar j;
	uchar  light_intensity;
	float temp;
    //uartInit();
	init();
   delay_nms(1500);	    //延时100ms
  while(1)              //循环
  {
		if(First_floor == 0){
			for(j=0;j<100;j++){
			//	MotorCW();   //顺时针转动
			Motor_A_CW();
			Motor_B_CW();  //电机B转动测试
			} 
		//	MotorStop();  //停止转动
			Motor_B_stop();
			Motor_B_stop();
			DHT11_delay_ms(2000); //延时 等待车辆通过
		}

		if(Second_floor == 0){
			for(j=0;j<100;j++){
			//	MotorCW();   //顺时针转动
			Motor_A_CCW();
			Motor_B_CCW();  //逆时针转动
			} 
		//	MotorStop();  //停止转动
			Motor_A_stop();
			Motor_B_stop();
			DHT11_delay_ms(2000); //延时 等待车辆通过
		}
		/*
		if(Third_floor == 0){
			for(j=0;j<100;j++){
				MotorCW();   //顺时针转动
			} 
			MotorStop();  //停止转动
			DHT11_delay_ms(2000); //延时 等待车辆通过
		}

		if(Fourth_floor == 0){
			for(j=0;j<100;j++){
				MotorCW();   //顺时针转动
			} 
			MotorStop();  //停止转动
			DHT11_delay_ms(2000); //延时 等待车辆通过
		}
		*/
	/*
		for(j=0;j<100;j++){
			MotorCW();   //顺时针转动
		} 
		MotorStop();  //停止转动
		DHT11_delay_ms(2000); //延时 等待车辆通过
		for(j=0;j<100;j++){
		   MotorCCW();  //逆时针转动
		} 
		MotorStop();  //停止转动
		DHT11_delay_ms(2000);
	*/	
  }
}

void RSINTR() interrupt 4 using 2
{
        uchar InPut3;
        if(TI==1) //发送中断          
        {
                TI=0;
                if(count!=3) //发送完3位数据         
                {
                        SBUF= outdata[count];
                        count++;
                }
        }

        if(RI==1)         //接收中断                  
        {        
                InPut3=SBUF;
                indata[count_r]=InPut3;
                count_r++;
                RI=0;                                                                 
                if (count_r==1)//接收完4位数据 
                {
                //数据接收完毕处理。
			
                        count_r=0;

                str[0]=indata[0];
				/*
				if(str[0] == NULL){
					TV_SWITCH = 0;       //	初始状态设置为继电器（电视开关）未触发-低电平触发
					KITCHEN = 0;
					LIVING_ROOM = 0;
					DINING_ROOM = 0;
					BED_ROOM = 0;
				}
				*/
				if(str[0] == 'S'){		
					is_close = 0;  //检测车牌OK，放行
				}
				else if(str[0] == 'a'){
					BED_ROOM = 0;				
				}
             }
        }
} 

