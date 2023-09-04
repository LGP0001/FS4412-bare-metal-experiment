# LED点亮与闪烁实验

## 项目概述
此项目旨在检验对GPIO操作的掌握情况，通过编写汇编程序是LED(LED3)点亮并闪烁。

## 开发环境和工具
- **开发板**：Exynos 4412 SCP_UM
- **芯片**：FS4412
- **开发语言**：汇编
- **开发环境**：Ubuntu 22.04 LTS
- **工具链**：arm-none-linux-gnueabi-gcc

## 硬件连接
描述LED与FS4412之间的连接，特别是用到的GPIO端口。
1. 电路逻辑分析：通过[电路原理图](./FS4412-DevBoard-V5.pdf)我们可以确定LED的控制逻辑。结论是，当给LED供上高电平时，它会点亮，低电平则会熄灭。
2. LED与Exynos4412的连接：参照[电路原理图](./FS4412_CoreBoard_V2.pdf)，我们找到LED是连接到Exynos4412的GPX1_0上的。
3. 控制LED的寄存器：通过查阅[数据手册](./4412手册完整版.pdf)，我们确定可以通过操作GPX1CON和GPX1DAT两个寄存器来实现对LED的控制。
4. 程序控制：根据以上硬件信息和分析结果，我们通过程序对指定寄存器进行操作，从而实现对LED的控制。

## 程序流程
1. 初始化GPIO端口
2. 点亮LED
3. 延时
4. 关闭LED
5. 重复以上步骤以实现LED闪烁

## 汇编代码
[点击这里查看完整的汇编代码](./led_arm.s) 

```assembly
; LED 初始化部分
LED_CONFIG:
    LDR R2, =0x11000C20   @ 加载 GPX1CON 的地址到 R2
    LDR R1, =0x00000001   @ 设置为输出
    STR R1, [R2]          @ 将 R1 的值写入 GPX1CON

    LDR R2, =0x11000C24   @ 加载 GPX1DAT 的地址到 R2
    LDR R1, =0x00000000   @ 确保 LED 熄灭
    STR R1, [R2]          @ 将 R1 的值写入 GPX1DAT
    MOV PC, LR            @ 返回

; LED 打开部分
LED_ON:
    LDR R2, =0x11000C24   @ 加载 GPX1DAT 的地址到 R2
    LDR R1, =0x00000001   @ 设置 LED 状态为打开
    STR R1, [R2]          @ 将 R1 的值写入 GPX1DAT
    MOV PC, LR            @ 返回

; 延时部分
DELAY:
    LDR R3, =500000000    @ 加载一个大的延迟值到 R3
L:
    SUBS R3, R3, #1       @ 递减 R3 直到它为 0
    BNE L                 @ 如果 R3 不为 0，则继续延迟循环
    MOV PC, LR            @ 返回
...