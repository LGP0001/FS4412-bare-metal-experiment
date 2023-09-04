# LED流水灯实验

## 项目概述
此项目通过流水灯实验来检验对GPIO操作的掌握情况。通过编写C程序控制LED（LED2、LED3、LED4、LED5）依次闪烁。

## 开发环境和工具
- **开发板**：Exynos 4412 SCP_UM
- **芯片**：FS4412
- **开发语言**：汇编
- **开发环境**：Ubuntu 22.04 LTS
- **工具链**：arm-none-linux-gnueabi-gcc

## 硬件连接
描述LED与FS4412之间的连接，并特别指出所使用的GPIO端口。

1. 电路逻辑分析：我们可以通过电路原理图(见GPIO章节)确定LED的控制逻辑。
LED与Exynos4412的连接：通过参考电路原理图(见GPIO章节)，我们得知LED是连接到Exynos4412的GPX上的。
2.控制LED的寄存器：通过查阅相关的数据手册(见GPIO章节)，我们可以确定通过操作GPXCON和GPXDAT两个寄存器来实现对LED的控制。
3.程序控制：根据上述的硬件连接和分析，我们可以编写程序来操作指定的寄存器，从而达到控制LED的目的。

## 程序流程
以下是程序的基本流程：

1. 初始化GPIO端口：配置GPXCON寄存器，设置为输出模式。
2. 点亮LED：将对应的LED控制位在GPXDAT寄存器中设置为低电平。
3. 延时：使用简单的循环或其他方法实现延时，保证LED亮的时间。
4. 关闭LED：将对应的LED控制位在GPXDAT寄存器中设置为高电平。
5. 重复：返回第2步，将下一个LED点亮，直到所有LED都闪烁过后，再重新开始。

## 基础教程
这里将为你展示如何从直接基于芯片手册的硬编码地址转换为使用头文件定义的宏和结构体来实现LED的控制  
[点击这里查看完整的教程](./教程.c) 

### 1. 存储器访问方式

#### 汇编语言访问存储器:
- 读存储器: `LDR R1,[R2]`
- 写存储器: `STR R1,[R2]`

#### C语言访问存储器:
- 读存储器: `data = *ADDR`
- 写存储器: `*ADDR = data`

### 2. 直接硬编码地址控制LED

在此方法中，我们直接使用芯片手册中给出的地址对LED进行控制。此方法最直接，但不利于代码维护。

```c
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
```
### 3. 使用宏定义

通过预处理器的宏定义，我们可以为这些硬编码的地址提供更有描述性的名称

```c
/*寄存器封装方式*/

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
```
### 4. 使用结构体

此方法进一步抽象，我们定义一个结构体来表示一个完整的GPIO端口。这使得我们可以通过结构体成员来直接访问相关的寄存器。

```c
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
```

### 5. 使用头文件

最后，我们将这些定义移到一个头文件exynos_4412.h中。这样，任何需要控制LED的C文件只需简单地包括这个头文件。

```c
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
```









