#include "exynos_4412.h"

void EXT_Config_GPX1_2();
void LED2_Config();
void Delay(unsigned int Time);
void handler_irq();

int main()
{
	EXT_Config_GPX1_2();
	ICC_Config();
	LED2_Config();
	while(1)
	{
	}
	return 0;
}

/*IRQ异常处理*/
void handler_irq()
{
	static int is_LED_ON = 0; 
	unsigned int IrqNum = 0;
	/*从中断控制器中获取当前中断的中断号*/
	IrqNum = CPU0.ICCIAR & 0x3FF;
	/*根据中断号处理不同的中断*/
	switch(IrqNum)
	{
		case 58:
		{
			printf("Key3 Pressed\n");
			if(is_LED_ON == 0)
			{
				GPX2.DAT |= (1 << 7); 			// 点亮LED2
				is_LED_ON = 1;		  			// 更新标志
			}
			else
			{
				GPX2.DAT &= ~(1 << 7);      	// 熄灭LED2
				is_LED_ON = 0;		  			// 更新标志
			}

			/*清除GPIO控制器中GPX1_2的中断挂起标志位*/
			EXT_INT41_PEND = (1 << 2);
			/*将当前中断的中断号写回到中断控制器中，以这种方式来告知中断控制器当前的中断已经处理完成，可以发送其它中断*/
			CPU0.ICCEOIR = CPU0.ICCEOIR & (~(0x3FF)) | (58);
			break;
		}
		default:
			printf("Unhandled interrupt: %d\n", IrqNum);
			break;
	}
}
/*外设层次 - 让外部的硬件控制器产生一个中断信号发送给中断控制器*/
void EXT_Config_GPX1_2()
{
	/*将GPX1_2设置成中断功能*/
	GPX1.CON |= (0xF << 8);
	/*设置GPX1_2的中断触发方式为下降沿触发*/
	EXT_INT41_CON &= ~(0x7 << 8); 				// 初始化GPX1_2的中断触发方式
	EXT_INT41_CON |= (0x2 << 8); 				// 设置GPX1_2为下降沿触发
	/*使能GPX1_2的中断功能*/
	EXT_INT41_MASK &= (~(1 << 2));
}
/*初始化LED2为输出模式*/
void LED2_Config()
{
	GPX2.CON &= (~(0xF << 28)); 				// 清零GPX2_7的配置位
    GPX2.CON |= (1 << 28);      				// 设置GPX2_7为输出模式
    GPX2.DAT |= (1 << 7);       				// 初始化LED2为点亮状态
}
/*中断控制器层次 - 让中断控制器接收外设产生的中断信号并对其进行管理然后再转发给CPU处理*/
void ICC_Config()
{
	/*全局使能中断控制器使其能接收外设产生的中断信号并转发到CPU接口*/
	ICDDCR |= 1;
	/*在中断控制器中使能58号中断，使中断控制器接收到58号中断后能将其转发到CPU接口*/
	ICDISER.ICDISER1 |= (1 << 26);
	/*选择由CPU0来处理58号中断*/
	ICDIPTR.ICDIPTR14 &= ~(0xFF << 16); 		// 清零ICDIPTR14的配置位
	ICDIPTR.ICDIPTR14 |= (0X01 << 16);			// 设置58号中断由CPU0处理

	/*使能中断控制器和CPU0之间的接口，使中断控制器转发的中断信号能够到达CPU0*/
	CPU0.ICCICR = CPU0.ICCICR | 1;
}
/*延时*/
void Delay(unsigned int Time)
{
	while(Time--);
}