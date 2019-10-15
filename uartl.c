/*
TxRx cruzados entre . No es necesario Vcc y GND en el conversor
y Putty en /dev/ttyUSB0
*/
#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
void confPin(void);
void confUart(void);
void UART0_IRQHandler(void);
void UART_IntTransmit(void);
void UART_IntReceive(void);
uint8_t info[] = "Hola mundo \n\r";
int main (void){
confPin();
confUart();
UART_Send(LPC_UART0, info, sizeof(info), NONE_BLOCKING);
while(1){}
return 0;
}
void confPin(void){
PINSEL_CFG_Type PinCfg;
//configuración pin de Tx y Rx
PinCfg.Funcnum = 1;
PinCfg.OpenDrain = 0;
PinCfg.Pinmode = 0;
PinCfg.Pinnum = 2;
PinCfg.Portnum = 0;
PINSEL_ConfigPin(&PinCfg);
PinCfg.Pinnum = 3;
PINSEL_ConfigPin(&PinCfg);
return;
}
void confUart(void){
UART_CFG_Type UARTConfigStruct;
UART_FIFO_CFG_Type UARTFIFOConfigStruct;
//configuración por defecto:
UART_ConfigStructInit(&UARTConfigStruct);
//inicializa periférico
UART_Init(LPC_UART0, &UARTConfigStruct);
UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
//Inicializa FIFO
UART_FIFOConfig(LPC_UART0, &UARTFIFOConfigStruct);
//Habilita transmisión
UART_TxCmd(LPC_UART0, ENABLE);
//Habilita interrucpción Tx

UART_IntConfig(LPC_UART0, UART_INTCFG_THRE, ENABLE);
/* preemption = 1, sub-priority = 1 */
//NVIC_SetPriority(UART0_IRQn, ((0x01<<3)|0x01));
//Habilita interrpción por UART0
NVIC_EnableIRQ(UART0_IRQn);
return;
}
void UART0_IRQHandler(void){
uint32_t intsrc, tmp;
//Determina la fuente de interrupción
intsrc = UART_GetIntId(LPC_UART0);
tmp = intsrc & UART_IIR_INTID_MASK;
// Evalúa si Transmit Holding está vacío
if (tmp == UART_IIR_INTID_THRE){
UART_IntTransmit();
}
return;
}
void UART_IntTransmit(void){
static uint8_t i=0;
if (i<20){
//Envía información por Tx
UART_Send(LPC_UART0, info, sizeof(info), NONE_BLOCKING);
i++;
}
else
UART_IntConfig(LPC_UART0, UART_INTCFG_THRE, ENABLE);
}
