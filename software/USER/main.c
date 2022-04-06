#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 	 
#include "dac.h"
#include "adc.h"
#include "usmart.h"

 int main(void)
 {	 
	u16 adcx;
	float temp;
 	u8 t=0;	 
	u16 dacval=0;
	u8 key;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	KEY_Init();			  //��ʼ����������
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();			 	 //LCD��ʼ��
	usmart_dev.init(72);	//��ʼ��USMART	
 	Adc_Init();		  		//ADC��ʼ��
	Dac1_Init();				//DAC��ʼ��

	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"DC-DC_2100200435");	
	 LCD_ShowString(60,70,200,16,16,"input:24V");	
	 LCD_ShowString(60,90,200,16,16,"OUT:0-20V");
	 LCD_ShowString(60,110,200,16,16,"DAC:PA4   ADC:PA1");	
	LCD_ShowString(60,130,200,16,16,"WK_UP:+  KEY1:-");	
	//��ʾ��ʾ��Ϣ											      
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,150,200,16,16,"DAC VAL:");	      
	LCD_ShowString(60,170,200,16,16,"DAC VOL:0.000V");	      
	LCD_ShowString(60,190,200,16,16,"ADC VOL:0.000V");
	
	 DAC_SetChannel1Data(DAC_Align_12b_R, 0);//��ʼֵΪ0	    	      
	while(1)
	{
		t++;
		key=KEY_Scan(0);
				
		if(key==WKUP_PRES)
		{		 
			if(dacval<4000)dacval+=200;
		  Dac1_Set_Vol(dacval);//����DACֵ	
		}else if(key==KEY1_PRES)	
		{
			if(dacval>200)dacval-=200;
			else dacval=0;
		  Dac1_Set_Vol(dacval);//����DACֵ
		}	 
		
		
		
		
	 if(t==10||key==KEY1_PRES||key==WKUP_PRES) //WKUP/KEY1������,���߶�ʱʱ�䵽��
		{	  
			adcx=DAC_GetDataOutputValue(DAC_Channel_1);//��ȡǰ������DAC��ֵ
			LCD_ShowxNum(124,150,adcx,4,16,0);     	//��ʾDAC�Ĵ���ֵ
			temp=(float)adcx*(3.3/4096);			//�õ�DAC��ѹֵ
			adcx=temp;
 			LCD_ShowxNum(124,170,temp,1,16,0);     	//��ʾ��ѹֵ��������
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,170,temp,3,16,0X80); 	//��ʾ��ѹֵ��С������
 			adcx=Get_Adc_Average(ADC_Channel_1,10);		//�õ�ADCת��ֵ	  
			temp=(float)adcx*(3.3/4096);			//�õ�ADC��ѹֵ
			adcx=temp;
 			LCD_ShowxNum(124,190,temp,1,16,0);     	//��ʾ��ѹֵ��������
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,190,temp,3,16,0X80); 	//��ʾ��ѹֵ��С������
			LED0=!LED0;	   
			t=0;
		}	    
		delay_ms(10);	

	}
 }

