.text
_start:

MAIN:
    BL LED_CONFIG @ 初始化LED为输出，并确保LED熄灭
LOOP:
    BL LED_ON     @ 打开LED
    BL DELAY      @ 延时
    BL LED_OFF    @ 关闭LED
    BL DELAY      @ 延时
    B LOOP        @ 无限循环

LED_CONFIG:
    LDR R2, =0x11000C20     @ GPX1CON的地址
    LDR R1, =0x00000001     @ 设置为输出
    STR R1, [R2]            @ 将值写入GPX1CON
    LDR R2, =0x11000C24     @ GPX1DAT的地址
    LDR R1, =0x00000000     @ 确保LED熄灭
    STR R1, [R2]            @ 将值写入GPX1DAT
    MOV PC, LR

LED_ON:
    LDR R2, =0x11000C24  @ GPX1DAT的地址
    LDR R1, =0x00000001  @ 确保LED点亮
    STR R1, [R2]         @ 将值写入GPX1DAT
    MOV PC, LR           @ 返回

LED_OFF:
    LDR R2, =0x11000C24  @ GPX1DAT的地址
    LDR R1, =0x00000000  @ 确保LED熄灭
    STR R1, [R2]         @ 将值写入GPX1DAT
    MOV PC, LR           @ 返回
DELAY:
    LDR R3, =500000000    @ 为延迟加载一个大值到R3
L:
    SUBS R3, R3, #1      @ 从R3中减去1
    BNE L       @ 如果R3不为0，返回到L
    MOV PC, LR           @ 返回

STOP:
    B STOP

.end


