## Arduino(STM8 based) DMX512
Using cheap and powerful chip STM8 to make a DMX512 decoder(also transmiter).
# dependency
https://github.com/tenbaht/sduino
# Install
Uncomment #define USER_UART1_INTERRUPT in HardwareSerial.c.h:34
# Constrain
Only support 250K bps

