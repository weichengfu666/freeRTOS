# freeRTOS
Greenhouse temperature and humidity control system
项目名称：温室大棚温湿度控制系统
系统功能：本设计是基于stm32f103单片机设计的温湿度报警系统；设置有四个独立按键，功能分别为设置、加、减、确定，能实现报警阀值的调节；本设计采用AT24C02存储芯片，在掉电情况下仍能保存设置的报警阈值；本设计采用LCD1602液晶显示，上面行显示当前的温度和湿度，下面行显示相应的冷、热、干、湿情况；选用全数字型温湿度传感器DHT11；当温度或湿度超出阈值范围时，有蜂鸣器报警提示，并有对应LED报警灯点亮以及控制输出接口使控温设备（或控湿设备）开始工作。联网控制使用的esp8266的AT（MQTT)指令链接阿里云物联网平台，通过对topic的订阅和发布实现远程控制。
