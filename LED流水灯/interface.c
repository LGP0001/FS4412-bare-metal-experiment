#include "exynos_4412.h"

void Delay(unsigned int Time); // 延时

int main(int argc, const char *argv[]) 
{
	GPX2.CON = GPX2.CON & (~(0xF << 28)) | (0x1 << 28); // 初始化LED2
	GPX1.CON = GPX1.CON & (~(0xF)) | (0x1); // 初始化LED3
	GPF3.CON = GPF3.CON & (~(0xF << 16)) | (0x1 << 16); // 初始化LED4
	GPF3.CON = GPF3.CON & (~(0xF << 20)) | (0x1 << 20); // 初始化LED5

	// 熄灭LED
	GPX2.DAT = GPX2.DAT & (~(0x1 << 7)); // 熄灭LED2
	GPX1.DAT = GPX1.DAT & (~(0x1)); // 熄灭LED3
	GPF3.DAT = GPF3.DAT & (~(0x1 << 4)); // 熄灭LED4
	GPF3.DAT = GPF3.DAT & (~(0x1 << 5)); // 熄灭LED5


	while(1)
	{
		GPX2.DAT = GPX2.DAT | (0x1 << 7); // 点亮LED2
		Delay(1000000); 
		GPX2.DAT = GPX2.DAT & (~(0x1 << 7)); // 熄灭LED2
		Delay(1000000);
		GPX1.DAT = GPX1.DAT | (0x1); // 点亮LED3
		Delay(1000000);
		GPX1.DAT = GPX1.DAT & (~(0x1)); // 熄灭LED3
		Delay(1000000);
		GPF3.DAT = GPF3.DAT | (0x1 << 4); // 点亮LED4
		Delay(1000000);
		GPF3.DAT = GPF3.DAT & (~(0x1 << 4)); // 熄灭LED4
		Delay(1000000);
		GPF3.DAT = GPF3.DAT | (0x1 << 5); // 点亮LED5
		Delay(1000000);
		GPF3.DAT = GPF3.DAT & (~(0x1 << 5)); // 熄灭LED5
		Delay(1000000);
	}
	return 0;

}

void Delay(unsigned int Time)
{
	while(Time --);
}
