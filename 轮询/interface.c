#include "exynos_4412.h"

void GPIO_Config(void);

int main()
{
	int is_LED_ON = 0;    									// 0代表LED熄灭，1代表LED点亮
	int delay = 0;											
	GPIO_Config();

	while (1) 												// 无限循环
	{
		if((GPX1.DAT & (1 << 2)) == 0) 						// 检测Key3是否被按下
		{
			if(is_LED_ON == 0)
			{
				GPX2.DAT |= (1 << 7); 						// 点亮LED2
				is_LED_ON = 1;		  						// 更新标志
			}
			else
			{
				GPX2.DAT &= ~(1 << 7);      				// 熄灭LED2
				is_LED_ON = 0;		  						// 更新标志
			}

			// 为了防止按键反跳，加入一个简单的延时
            for (delay = 0; delay < 1000000; delay++);

			// 等待按键释放
            while ((GPX1.DAT & (1 << 2)) == 0);
            
            // 为了防止按键再次反跳，再加入一个简单的延时
            for (delay = 0; delay < 1000000; delay++);
		}
	}
	
	return 0;
}

void GPIO_Config(void)
{
	// 初始化LED2为输出模式
	GPX2.CON &= (~(0xF << 28)); 							// 清零GPX2_7的配置位
    GPX2.CON |= (1 << 28);      							// 设置GPX2_7为输出模式
    GPX2.DAT &= ~(1 << 7);      							// 初始化LED2为熄灭状态

	// 初始化KEY3为输入模式
	GPX1.CON &= (~(0xF << 8)); 				 				// 清零GPX1_2的配置位
	GPX1.CON &= ~(1 << 8);  								// 设置GPX1_2为输入模式
}

