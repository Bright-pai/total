#include "Key.h"
#include "oled.h"
Type_KEY Key_GetNum(void)
{
	Type_KEY KeyNum = No_KEY;
	if (DL_GPIO_readPins(KEY_KEY0_PORT, KEY_KEY0_PIN) )
	{
		delay_ms(20);
		while (DL_GPIO_readPins(KEY_KEY0_PORT, KEY_KEY0_PIN) );
		delay_ms(20);
		KeyNum = KEY0;
	}
	if (DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_PIN) )
	{
		delay_ms(20);
		while (DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_PIN) );
		delay_ms(20);
		KeyNum =KEY1;
	}
    if (DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_PIN) )
	{
		delay_ms(20);
		while (DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_PIN) );
		delay_ms(20);
		KeyNum = KEY2;
	}
	
	return KeyNum;
}

int Key_Detect_Task(void)
{
	int Task_Key_Record1;
	int Task_Key_Record2;
	int Task_Key_Record3;
	Type_KEY key=No_KEY;
	while(key==No_KEY)
	{
		 key=Key_GetNum();
		 switch(key)
		 {
			case KEY0:
			{
				Task_Key_Record1=0;
				break;
			}
			case KEY1:
			{
				Task_Key_Record1=1;
				break;
			}
			default:
			{
				key=No_KEY;
				break;
			}
		 }
	}
	OLED_ShowNum(0,0,(int)Task_Key_Record1,1,16);

	key=No_KEY;
	while(key==No_KEY)
	{
		 key=Key_GetNum();
		 switch(key)
		 {
			case KEY0:
			{
				Task_Key_Record2=0;
				break;
			}
			case KEY1:
			{
				Task_Key_Record2=1;
				break;
			}
			default:
			{
				key=No_KEY;
				break;
			}
		 }
	}
	OLED_ShowNum(9,0,(int)Task_Key_Record2,1,16);

	key=No_KEY;
	while(key==No_KEY)
	{
		 key=Key_GetNum();
		 switch(key)
		 {
			case KEY0:
			{
				Task_Key_Record3=0;
				break;
			}
			case KEY1:
			{
				Task_Key_Record3=1;
				break;
			}
			default:
			{
				key=No_KEY;
				break;
			}
		 }
	}
	OLED_ShowNum(18,0,(int)Task_Key_Record3,1,16);
	
	int task=(Task_Key_Record1<<2)+(Task_Key_Record2<<1)+(Task_Key_Record3);
	OLED_ShowNum(36,0,(int)task,1,16);
	return task;

}

int Key_Task1_CircleNum(void)
{
	int Circle_Key_Record1;
	int Circle_Key_Record2;
	int Circle_Key_Record3;
	Type_KEY key=No_KEY;
	while(key==No_KEY)
	{
		 key=Key_GetNum();
		 switch(key)
		 {
			case KEY0:
			{
				Circle_Key_Record1=0;
				break;
			}
			case KEY1:
			{
				Circle_Key_Record1=1;
				break;
			}
			default:
			{
				key=No_KEY;
				break;
			}
		 }
	}
	OLED_ShowNum(0,2,(int)Circle_Key_Record1,1,16);

	key=No_KEY;
	while(key==No_KEY)
	{
		 key=Key_GetNum();
		 switch(key)
		 {
			case KEY0:
			{
				Circle_Key_Record2=0;
				break;
			}
			case KEY1:
			{
				Circle_Key_Record2=1;
				break;
			}
			default:
			{
				key=No_KEY;
				break;
			}
		 }
	}
	OLED_ShowNum(9,2,(int)Circle_Key_Record2,1,16);

	key=No_KEY;
	while(key==No_KEY)
	{
		 key=Key_GetNum();
		 switch(key)
		 {
			case KEY0:
			{
				Circle_Key_Record3=0;
				break;
			}
			case KEY1:
			{
				Circle_Key_Record3=1;
				break;
			}
			default:
			{
				key=No_KEY;
				break;
			}
		 }
	}
	OLED_ShowNum(18,2,(int)Circle_Key_Record3,1,16);
	
	int CircleNum=(Circle_Key_Record1<<2)+(Circle_Key_Record2<<1)+(Circle_Key_Record3);
	OLED_ShowNum(36,2,(int)CircleNum,1,16);
	return CircleNum;


}