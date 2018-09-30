Overview
Application uses Weather API, Nokia Display and USART handled by STM32 Discovery. Main idea of this project is to check weather in all location in the world.

Tools
System Workbench for STM32
IntelliJ IDEA


How to run:

Connect :
Nokia display
RES  <-> PC15	
SCE  <-> PC13	
DC   <-> PC14	
D/IN <-> PC3	
CLK  <-> PB10	
+V   <-> 3.3V	
GND  <-> GND

USART
TxD -> PC11
RxD -> PC10
GND -> GND

In System Workbench for STM32 install project 'display', next run project 'WeatherApp' in IntelliJ IDEA and enter the name of city.
Make sure you have access to the internet 

Attributions:
PCD8544 library
RXTXcomm library
java-json library
https://openweathermap.org/api

Credits

Created by: Remigiusz Wróblewski


https://api.openweathermap.org/data/2.5/weather?q=Poznan&units=metric&appid=0aa634ac9a3758b22253d8765c81ebc2




