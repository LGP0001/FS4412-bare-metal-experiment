# UART LED 控制实验
本项目演示如何通过UART通信，使用电脑远程控制LED的状态。

## 实验目标
使用电脑的串口发送字符到开发板。
根据接收到的字符控制LED的状态。
使用终端实现简单的用户交互。

## 实验步骤

1. 硬件连接
确保你的开发板和电脑之间的UART连接正确。通常情况下，你可能需要一个USB转串口模块。

2. 代码部署
将提供的代码部署到你的开发板上。

3. 终端配置
使用终端软件（如PuTTY、Tera Term等）打开与开发板相关联的串口。设置正确的波特率和其他串口参数。

4. 远程控制LED

- 在终端上输入2，LED2点亮。
- 再次输入2，LED2熄灭。
- 可以反复进行此操作，远程控制LED的状态。
## 注意事项
确保你的电源和地线连接正确，以避免任何可能的损坏。
请确认在与开发板交互之前已正确设置了终端的波特率和其他参数。

## 实验结果
通过这个实验，你将能够使用电脑远程控制LED的状态，并理解UART通信的基础知识。









