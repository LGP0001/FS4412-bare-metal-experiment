/*
 * 一.汇编语言访问存储器
 * 	1.读存储器
 * 		LDR R1,[R2]
 * 	2.写存储器
 * 		STR R1,[R2]
 *
 * 二.C语言访问存储器
 * 	1.读存储器
 * 		data = *ADDR
 * 	2.写存储器
 * 	 	*ADDR = data
 */

// 由于是裸机，所以只能自己写函数，不能调用任何系统函数（例如sleep）
#if 0
void Delay(unsigned int Time)
{
	while(Time --);
}
#endif

#if 0

int main()
{
	/*通过设置GPX2CON寄存器来将GPX2_7引脚设置成输出功能*/
	*(unsigned int *)0x11000c40 = 0x10000000;

	while(1)
	{
		/*点亮LED2*/
		*(unsigned int *)0x11000c44 = 0x00000080;
		/*延时*/
		Delay(1000000);
		/*熄灭LED2*/
		*(unsigned int *)0x11000c44 = 0x00000000;
		/*延时*/
		Delay(1000000);
	}
	return 0;
}
#endif

# if 0
/*
* 寄存器封装方式
*/
#define GPX2CON (*(unsigned int *)0x11000c40)
#define GPX2DAT (*(unsigned int *)0x11000c44)

int main()
{
	/*通过设置GPX2CON寄存器来将GPX2_7引脚设置成输出功能*/
	GPX2CON = 0x10000000;

	while(1)
	{
		/*点亮LED2*/
		GPX2DAT = 0x00000080;
		/*延时*/
		Delay(1000000);
		/*熄灭LED2*/
		GPX2DAT = 0x00000000;
		/*延时*/
		Delay(1000000);
	}
	return 0;
}
#endif

#if 0
typedef struct 
{
	unsigned int CON;
	unsigned int DAT;
	unsigned int PUD;
	unsigned int DRV;
}gpx2;

#define GPX2 (*(gpx2 *)0x11000c40)

int main()
{
	GPX2.CON = 0x10000000;

	while(1)
	{
		/*点亮LED2*/
		GPX2.DAT = 0x00000080;
		/*延时*/
		Delay(1000000);
		/*熄灭LED2*/
		GPX2.DAT = 0x00000000;
		/*延时*/
		Delay(1000000);
	}
	return 0;
}
#endif

#if 0
#include "exynos_4412.h"

int main()
{
	GPX2.CON = 0x10000000;

	while(1)
	{
		/*点亮LED2*/
		GPX2.DAT = 0x00000080;
		/*延时*/
		Delay(1000000);
		/*熄灭LED2*/
		GPX2.DAT = 0x00000000;
		/*延时*/
		Delay(1000000);
	}
	return 0;
}
#endif
# if 0
#include "exynos_4412.h"

int main()
{
	GPX2.CON = GPX2.CON & (~(0xF << 28)) | (0x1 << 28);

	while(1)
	{
		/*点亮LED2*/
		GPX2.DAT = GPX2.DAT | (1 << 7);
		/*延时*/
		Delay(1000000);
		/*熄灭LED2*/
		GPX2.DAT = GPX2.DAT & (~(1 << 7));
		/*延时*/
		Delay(1000000);
	}
	return 0;
}

#endif


/*
 * 1.unsigned int a; 将a的第3位置1，其他位保持不变
 * 	******** ******** ******** ********
 * 	******** ******** ******** ****1***
 * 	00000000 00000000 00000000 00001000
 *
 * 	a = a | (1 << 3); (或运算)
 *
 * 2.unsigned int a; 将a的第3位置0，其他位保持不变
 * 	******** ******** ******** ********
 * 	******** ******** ******** ****0***
 * 	11111111 11111111 11111111 11110111
 *
 * 	a = a & (~(1 << 3)); （与运算）
 *
 * 3.unsigned int a; 将a的第[7:4]位置为0101，其他位保持不变
 * 	******** ******** ******** ********
 * 	******** ******** ******** 0101****
 *
 * 	1).先清零
 * 	11111111 11111111 11111111 00001111
 * 	00000000 00000000 00000000 11110000
 *  00000000 00000000 00000000 00001111
 *
 * 	a = a & (~(0xF << 4));
 *
 * 	2).再置位
 * 	00000000 00000000 00000000 01010000
 * 	00000000 00000000 00000000 00000101
 *
 * 	a = a | (0x5 << 4);
 *
 * 	=> a = a & (~(0xF << 4)) | (0x5 << 4);
 */
