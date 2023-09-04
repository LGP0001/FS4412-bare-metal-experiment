# include "exynos_4412.h"

void Delay(unsigned int Time); // 延时
int main()
{
	WDT.WTCON = WDT.WTCON | (0x4D << 8); 	// 设置一级分频WTCON[15:8]
	WDT.WTCON = WDT.WTCON | (0X3 << 3); 	// 设置二级分频WTCON[4:3]
	/*WTCNT递减频率 = PLCK(100000000)/(0x4D + 1)/128 = 10016*/
	WDT.WTCON = WDT.WTCON & (~(1 << 2)); 	// 禁止WDT产生中断信号WTCON[2]
	/*实验要求减到零进行复位*/
	WDT.WTCON = WDT.WTCON | 1;				// 使能WDT产生复位信号WTCON[0]
	WDT.WTCNT = (10000 * 10);				// 设置计数器的初始值
	WDT.WTCON = WDT.WTCON | (1 << 5);		// 使能WDT,计数器开始递减
	
	while (1)
	{
		printf("WDT.WTCNT = %d\n", WDT.WTCNT);
		WDT.WTCNT = 10000; // 喂狗
		Delay(100000);
	}
	return 0;
}

void Delay(unsigned int Time)
{
	while(Time--);
}