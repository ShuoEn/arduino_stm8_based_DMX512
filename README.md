## Arduino(STM8 based) DMX512
Using cheap and powerful chip STM8 to make a DMX512 decoder(also transmiter).
# dependency
https://github.com/tenbaht/sduino
# Install
Uncomment #define USER_UART1_INTERRUPT in HardwareSerial.c.h:34
Generally could be found at like: "C:\Users\YOUR USER NAME\AppData\Local\Arduino15\packages\sduino\hardware\stm8\0.5.0\cores\sduino"
# Constrain
Only support 250K bps

