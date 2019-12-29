/*
STM8S103F3 Breakout Board comfirmed
Uncomment
#define USER_UART1_INTERRUPT
in HardwareSerial.c.h:34

There are bugs using "enum"
*/

#include "stm8s.h"
#define LED_PIN_R PD4 
#define LED_PIN_G PD3 
#define LED_PIN_B PA3 
#define REFRESH_TIME 20 // in milliseconds
uint8_t DMX1_Tx_buf[20];
uint8_t DMX1_Rx_buf[513]; // start from 1
bool tim_update = false;
uint32_t last_time = 0;
uint8_t addr = 1;
void setup() {

    UART1_DeInit();
    // put your setup code here, to run once:
    UART1_Init((uint32_t)250000, UART1_WORDLENGTH_9D, UART1_STOPBITS_1, UART1_PARITY_NO,
        UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    /* Enable UART1 Transmit interrupt*/
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

    /* Enable general interrupts */
    enableInterrupts();
}

void loop() {
    if (millis() - last_time > REFRESH_TIME) {
        last_time = millis();
        analogWrite(LED_PIN_R, DMX1_Rx_buf[1 + (addr - 1) * 3]);
        analogWrite(LED_PIN_G, DMX1_Rx_buf[2 + (addr - 1) * 3]);
        analogWrite(LED_PIN_B, DMX1_Rx_buf[3 + (addr - 1) * 3]);
        tim_update = false;
    }

}

void DMX_SendPacket(void)
{
    uint16_t pDMX_buf = 0;
    while (pDMX_buf < 3 + 2) //1-512
    {
        /* send data packet to slaves*/
        // UART1_SendData9()
        if (UART1->SR & (1 << 6))
        {
            /*发送起始码 00 第 0& 1個要是0*/
            if (pDMX_buf == 0)
            {
                UART1->DR = ((UART1->DR) & 0xfe00);//第九位置0
            }
            else
            {
                UART1->DR = 0x0100 | DMX1_Tx_buf[pDMX_buf - 1];//第九位置1
            }
            pDMX_buf++;
        }
    }
}


INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
    /* Read one byte from the receive data register */
    //uint8_t c = UART1_ReceiveData8();
    //printf("%c",c);
    uint16_t UDR;
    static uint16_t RXB8;
    static uint16_t pDMX_buf = 0; //数据指针
    static uint8_t fDMX_buf_right = 0;

    //接收数据
    if (UART1_GetITStatus(UART1_IT_RXNE) != RESET)//USART_FLAG_RXNE
    {
        //USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
        UDR = UART1_ReceiveData9();
        RXB8 = (UDR & 0x0100);


        if (RXB8 == 0) //复位信号
        {
            if (!UDR)
            {
                fDMX_buf_right = 1;//接收数据正确
                pDMX_buf = 0; //直接接收第一个数据 不保存第0个数据。
            }
        }
        else //rb8 =1 pDMX_buf=1 调光数据
        {
            if (1 == fDMX_buf_right)
            {
                DMX1_Rx_buf[pDMX_buf++] = (u8)UDR;
                //接收到512个数据
                if (pDMX_buf > 512)
                {
                    fDMX_buf_right = 0;
                    tim_update = SET; //更新调光数据
                }
            }
        }
    }

}
