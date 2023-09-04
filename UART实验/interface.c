# include "exynos_4412.h"

void UART_Init(void); // 初始化UART
void UART_Send_Byte(char dat); // 发送一个字节的数据
char UART_Rec_Byte(void); // 接收一个字节的数据
void UART_Send_Str(char * pstr); // 发送字符串
void Delay(unsigned int Time); // 延时
void LED2_Init(void); // 初始化LED2
void LED2_On(void); // LED点亮
void LED2_Off(void); // LED熄灭

int main()
{
	int rcdat;
	int flag = 0;
	UART_Init(); // 初始化UART
	LED2_Init(); // 初始化LED2

	while (1)
	{
		rcdat = UART_Rec_Byte(); // 从UART接收数据

		if(rcdat == '2') // 判断是否接收到字符'2'
		{
			/* 切换LED状态
			* 当 flag 为 0 时，~flag 为 0xFFFFFFFF（32位系统下，-1的二进制形式）
			* 当 flag 为 0xFFFFFFFF 时，~flag 为 0*/
			flag = ~flag; 
			if(flag)
			{
				LED2_On(); // 点亮LED
				Delay(1000000);
			}
			else
			{
				LED2_Off(); // 熄灭LED
				Delay(1000000);
			}
		}
	}
	return 0;
}

void UART_Init(void)
{
	/*1.将GPA1_0和GPA1_1设置成UART2的接收和发送引脚 GPA1CON[7:0]*/
	GPA1.CON = GPA1.CON & ((~(0xFF)) | (0x22));
	/*2.设置UART2的帧格式 8位数据位 1位停止位 无校验 正常模式 ULCON2[6:0]*/
	UART2.ULCON2 = UART2.ULCON2 & ((~(0x7F)) | (0x3));
	/*3.设置UART2的接收和发送模式为轮询模式 UCON2[3:0]*/
	UART2.UCON2 = UART2.UCON2 &((~(0xF)) | (0X5));
	/*4.设置UART2的波特率为115200 UBRDIV2/UFRACVAL2*/
	UART2.UBRDIV2 = 53;
	UART2.UFRACVAL2 = 4;
}

void UART_Send_Byte(char dat) 
{
	/*等待发送寄存器为空，即上一个数据已经发送完成 UTRSTAT2[1]*/
	while(!(UART2.UTRSTAT2 & (1 << 1)));
	/*将要发送的数据写入发送寄存器 UTXH2*/
	UART2.UTXH2 = dat;
}

char UART_Rec_Byte(void)
{
	/* 循环等待，直到接收寄存器有数据 UTRSTAT2[0] */
	while (!(UART2.UTRSTAT2 & 1));

	// 当循环结束时，表示接收到数据
	return UART2.URXH2;
}

void Delay(unsigned int Time)
{
	while(Time --);
}

void LED2_Init(void)
{
	GPX2.CON = GPX2.CON & (~(0xF << 28)) | (0x1 << 28); // 初始化LED2
}
void LED2_On(void)
{
	GPX2.DAT = GPX2.DAT | (0x1 << 7);
}

void LED2_Off(void)
{
	GPX2.DAT = GPX2.DAT & (~(0x1 << 7));
}