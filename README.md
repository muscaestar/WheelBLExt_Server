# G920蓝牙Hub——服务端（基于ESP32C3）
## 概括
- 以ESP32C3为核心
  - 外部信号输入
    - 蓝牙：接受外部输入的按键指令
    - 按钮：
      - 一个拨动开关控制蓝牙开启关闭，若关闭进入睡眠模式
      - 一个微动开关，清空当前输入指令
  - 信号输出
    - 视觉信号：
      - 红色LED，电源
      - 蓝色LED，灯亮代表蓝牙开启，灯灭代表蓝牙关闭+睡眠
    - 按键电信号
      - 通过两个74HC4051，控制从G920主板引出的按键触点
- Z通路开关
  - 控制ESP32与G920主板的主线路通断
- 电源板块
  - 独立于整体PCB
  - 18690+电量指示+通断开关

## 使用方式
- 开启使用：
  1. 检查：Z路开关关闭，蓝牙开关关闭，电源开关关闭
  2. 蓝牙打开，电源打开
  3. G920插入USB
  4. 根据情况控制Z路
     
- 省电待机：
  1. Z路关闭
  2. 蓝牙关闭进入睡眠
 
- 完全关闭：
  1. 断开G920 USB
  2. Z路开关关闭，蓝牙开关关闭，电源开关关闭
 
## 后续优化
- 没有上电时按键会互相干扰，估计下一版本（如果有）可以加入整流二极管

## 外壳设计
TODO

## 原理图 Version20240922
![SCH_G920_hub_ble_1-G920_HUB_BLE_1_2024-09-28](https://github.com/user-attachments/assets/157d40e3-0d4c-4321-aba3-216ff6d71133)

## PCB制版文件
[PCB制版文件ZIP](https://github.com/muscaestar/WheelBLExt_Server/releases/tag/pcb)

## PCB成品
- 两片74HC4051模块
- ESP32C3 Supermini
- ESP32C3 Supermini充放电扩展模块

![d4b0beb3a87384795545de6cfa6ec8e7_720](https://github.com/user-attachments/assets/82c361e4-9617-4793-9bef-0ab508764606)
![a43b396449782a481f95877c1238f6b2_720](https://github.com/user-attachments/assets/7c959d9a-b83e-4cdb-b4c4-6a0fb361ac7b)


## G920的pcb焊点
![image](https://github.com/user-attachments/assets/1ca34fda-fe66-4d64-ad7a-d61fa5a78efb)
![image](https://github.com/user-attachments/assets/a0f92794-b419-4646-8181-ac8f6706a4fb)
