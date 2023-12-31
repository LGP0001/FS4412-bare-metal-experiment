# 中断

## 目录
1. 中断基础
2. 中断控制器
3. 中断处理
4. 中断编程示例

<a name="中断基础"></a>

## 中断基础

在深入探讨中断之前，我们首先需要明确一点：中断是一个机制，允许计算机在执行一个任务时，因外部或内部的某些特定事件而暂时停下，转去处理这些事件，然后返回到被中断的地方继续执行。

### 中断的分类
1. 外部中断：这些中断是由处理器外部的事件触发的，例如用户按下键盘、外部设备发出的信号等。

2. 内部中断：这些中断是由处理器内部的事件触发的，例如除以零、无效指令或其他异常情况。

3. 软件中断：通常由执行特定的指令或程序调用触发。

### 中断用途
1. 及时响应:
中断机制允许系统快速响应外部或内部的突发事件，确保时间敏感的任务得到及时处理。

2. 高效使用CPU:
在等待外部事件（例如，I/O操作）完成时，处理器可以被其他任务使用，而不是空闲等待。

3. 简化编程:
中断允许硬件和软件之间的解耦，程序员可以专注于业务逻辑，而不用过多关心硬件的具体操作。

### 中断与轮询的比较
- 轮询是指CPU不断地检查某个条件是否满足，以决定是否执行某些操作。例如，检查是否有新的输入或是否有外部事件发生。

- 中断则是允许事件直接通知CPU，从而触发特定的处理过程，不需要CPU不断地进行检查。

- 优点:  
中断：更为及时，CPU可以做其他工作，不需要一直检查。  
轮询：实现简单，但可能造成CPU资源浪费。

<a name="中断控制器"></a>

## 中断控制器
中断控制器是一个硬件设备，负责管理、优先级排序和分派来自多个中断源的中断请求（IRQs）给中央处理器（CPU）。在系统中，可能有多个设备和源希望在特定的时刻中断CPU。中断控制器的作用就是协调这些请求，确保CPU能够以一个有序、有效的方式响应它们。

### 主要功能
1. 中断请求的检测和确认：中断控制器持续监测来自不同源的中断信号，确认是否有有效的中断请求。

2. 中断的优先级管理：在多个中断请求同时到达时，中断控制器会根据预设或动态的优先级规则选择一个最高优先级的中断源，并将其发送到CPU。

3. 中断的屏蔽与使能：为了在特定情况下避免中断的干扰，中断控制器允许系统屏蔽某些中断，或者启用之前被屏蔽的中断。

4. 嵌套中断处理：当一个中断正在处理时，如果另一个更高优先级的中断请求到来，中断控制器可以暂停当前的中断处理，先处理更高优先级的中断。

### 中断控制器的种类
1. 简单的中断控制器：为每个中断源提供一个直接的连接，所有中断请求都被平等地传送给CPU。

2. 可编程中断控制器（PIC）：例如，Intel的8259 PIC，可以为不同的中断源设定优先级，还可以控制中断的使能和屏蔽。

3. 高级可编程中断控制器（APIC）：这是现代多核和多处理器系统中常见的中断控制器，支持复杂的中断分发和处理机制。

### 中断控制器在现代系统中的角色
随着现代计算机系统的复杂性增加，中断控制器的作用变得越来越重要。在多核、多处理器的环境中，中断的分派和处理需要更精细、更灵活的控制。现代中断控制器不仅可以为每个中断源设置优先级，还可以根据不同的策略将中断分派给不同的处理器或核心。


中断控制器是中断处理系统的核心部分，它确保中断请求得到有效、有序和及时的处理。而在复杂的系统中，例如嵌入式系统或高性能服务器，高效的中断处理机制对于提高系统的整体性能和响应速度是至关重要的。

<a name="中断处理"></a>

## 中断处理
当中断发生时，处理器会执行以下流程：

1. 保存当前任务的状态：
在收到中断之前，处理器可能正在执行其他任务。首先，它会保存当前任务的执行状态，如寄存器的内容，以便在处理完中断后可以恢复并继续执行。

2. 识别中断来源：
中断可能来自各种原因，如外部设备、计时器或软件错误。处理器通过查询中断控制器来确定是哪个源产生的中断。

3. 确定中断服务程序：
一旦知道中断的来源，处理器就会查找与该中断源关联的中断服务程序的地址，并跳转到该地址开始执行。

4. 执行中断服务程序：
中断服务程序包含处理特定中断的所有必要代码。例如，如果是外部按键引起的中断，中断服务程序可能会读取该按键的状态。

5. 恢复原来的状态：
在中断服务程序执行完毕后，处理器会恢复它在接收到中断之前保存的状态，这样它就可以继续执行中断前的任务，就像没有发生过中断一样。

6. 返回主任务：
一旦状态被恢复，处理器继续执行之前的任务。

<a name="中断编程示例"></a>

## 中断编程示例

### 编程要求
- 使用中断的方式检测Key3按键的状态，实现按一次按键，LED2点亮，再次按下，LED2熄灭

### 前期准备
- 电路逻辑分析：通过电路原理图我们可以确定Key3按键的控制逻辑。结论是，当按键被按下时，对应的GPIO端口会读到低电平；当按键释放，对应的GPIO端口则会读到高电平。
<img src="./picture/FS4412 DevBoard Key.png" alt="FS4412 DevBoard Key" width="500"/>
<img src="./picture/FS4412 DevBoard LED.png" alt="FS4412 DevBoard LED" width="500"/>

- Key3与Exynos4412的连接：参照电路原理图，我们找到Key3是连接到Exynos4412的GPX1_2(EINT10)上的。而LED2则连接在GPX2_7。
<img src="./picture/FS4412 CoreBoard Key LED.png" alt="FS4412 CoreBoard Key LED" width="600"/>

- 通过数据手册查阅相关寄存器的详细信息。
 <img src="./picture/FS4412 Interrupt Map Summary.png" alt="FS4412 Interrupt Map Summary" width="600"/>   
 上图可知我们需要调试所有与CPU0有关的寄存器。
 <img src="./picture/FS4412 Interrupt Source.png" alt="FS4412 Interrupt Source" width="600"/>  
 上图可知FS4412中中断控制器中对EINT10的中断编号为58。
 <img src="./picture/FS4412 Interrupt ICDDCR.png" alt="FS4412 Interrupt ICDDCR" width="600"/>  
 上图可知我们需要对ICDDCR寄存器赋为1。  
 中文翻译：  
 对于监控外围中断信号并将待处理的中断转发到CPU接口的全局使能。  
 0 = GIC忽略所有的外围中断信号，不会将待处理的中断转发到CPU接口。  
 1 = GIC会监控外围的中断信号，并将待处理的中断转发到CPU接口。  
 <img src="./picture/FS4412 Interrupt Controller ICDISER_CPU.png" alt="FS4412 Interrupt Controller ICDISER_CPU" width="700"/> 
 上图可知我们需要对ICDISER1_CPU0[26]寄存器赋为1。   
 中文翻译：  
 对于SPIs和PPIs，每个位都有如下意义：  
 读操作：  
 0 = 禁用相应的中断。  
 1 = 启用相应的中断。  
 写操作：  
 0 = 无效果。  
 1 = 启用相应的中断。随后对此位的读取将返回值1。  
 <img src="./picture/FS4412 Interrupt Controller ICDIPTR_CPU.png" alt="FS4412 Interrupt Controller ICDIPTR_CPU" width="700"/>    
 上图可知我们需要对ICDISER1_CPU0[26]寄存器赋为1。  
 中文翻译：  
 系统中的处理器从0开始编号，每个CPU目标字段中的位都指向相应的处理器。    
 例如，值0x3意味着待处理的中断被发送到处理器0和1。  
 对于ICDIPTR0到ICDIPTR7，任何CPU目标字段的读取都返回执行读取操作的处理器的编号。  
 [点击这里查看整理文件](./picture/ICDIPTRn%20Address%20Map.xlsx)  
  <img src="./picture/FS4412 Interrupt Controller ICCICP_CPUn.png" alt="FS4412 Interrupt ICDDCR" width="600"/>   
 上图可知我们需要对ICCICR寄存器赋为1。  
 中文翻译：  
 通过CPU接口向连接的处理器发送中断信号的全局使能。  
 0 = 禁用中断信号  
 1 = 启用中断信号

- 程序控制：根据以上硬件信息和分析结果，我们通过C程序对指定寄存器进行操作，从而实现对LED2的控制，以及检测Key3的状态。

### 实现步骤
[点击这里查看完整程序](./interface.c) 
1. 配置中断控制器：设置GPX1_2为中断模式，并确定中断触发方式（例如：上升沿、下降沿）。
```C
/*外设层次 - 让外部的硬件控制器产生一个中断信号发送给中断控制器*/
void EXT_Init_GPX1_2()
{
	/*将GPX1_2设置成中断功能*/
	GPX1.CON |= (0xF << 8);
	/*设置GPX1_2的中断触发方式为下降沿触发*/
	EXT_INT41_CON &= ~(0x7 << 8); // 初始化GPX1_2的中断触发方式
	EXT_INT41_CON |= (0x2 << 8); // 设置GPX1_2为下降沿触发
	/*使能GPX1_2的中断功能*/
	EXT_INT41_MASK &= ~(1 << 2);
}
```
2. 编写中断服务程序：根据按键状态控制LED2。  
初始化LED2：为LED2设置GPIO。确保LED2为输出模式。
```C
void GPIO_Config(void)
{
	// 初始化LED2为输出模式
	GPX2.CON &= (~(0xF << 28)); 				// 清零GPX2_7的配置位
    GPX2.CON |= (1 << 28);      				// 设置GPX2_7为输出模式
    GPX2.DAT &= ~(1 << 7);      				// 初始化LED2为熄灭状态
}
```
3. 初始化中断控制器：让中断控制器接收外设产生的中断信号并对其进行管理然后再转发给CPU处理
```C
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
```
4. 初始化异常向量表
```assembly
_start:
	/*
	 * Vector table
	 */ 
	b reset
	b .
	b .
	b .
	b .
	b .
	/*
	 * 从异常向量表再跳转到IRQ的异常处理程序
	 */
	b irq_handler
	b .

	irq_handler:
	/*
	 * 因为产生IRQ异常后ARM自动保存到LR中的返回地址是被IRQ打断的指令
     * 的下一条再下一条指令的地址，所以我们需要人为的去修正一下
	 */
	sub lr, lr, #4
	/*
	 * 因为IRQ模式下使用的R0-R12寄存器和USER模式下使用的是同一组
	 * 所以在处理异常之前需要先将之前寄存器中的值压栈保护
	 */
	stmfd sp!, {r0-r12,lr}
	/*
	 * 跳转到handler_irq处理异常
	 */
	bl handler_irq
	/*
	 * 异常返回
	 * 1.将R0-R12寄存器中的值出栈，使其恢复到被异常打断之前的值
	 * 2.将SPSR寄存器中的值恢复到CPSR，使CPU的状态恢复到被异常打断之前
	 * 3.将栈中保存的LR寄存器的值出栈给PC，使程序跳转回被异常打断的点继续执行
	 */
	ldmfd sp!,{r0-r12,pc}^
```
5. 中断程序
```C
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
```





