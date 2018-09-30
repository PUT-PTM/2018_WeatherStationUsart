# Overview
Application uses Weather API, Nokia Display and USART handled by STM32 Discovery. Main idea of this project is to check weather in all location in the world.

# Tools
- System Workbench for STM32
- IntelliJ IDEA


# How to run:

Connect :

- Nokia display

RES  <-> PC15

SCE  <-> PC13

DC   <-> PC14

D/IN <-> PC3

CLK  <-> PB10

+V   <-> 3.3V

GND  <-> GND


- USART
TxD -> PC11

RxD -> PC10

GND -> GND

In System Workbench for STM32 install project 'stm', next run project 'WeatherApp' in IntelliJ IDEA and enter the name of city.
Make sure you have access to the internet. 


# Credits

Created by: Remigiusz Wróblewski




