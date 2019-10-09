/*
===============================================================================
 Name        : FinalProject.c
 Authors      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
/* Directives ------------------------------- */
#define TRUE 1
#define FALSE 0
#define LOW	0
#define HIGH 1
#define PIN22 (uint32_t)(1<<22)
#define PORT0 (uint8_t)((0))
#define OUTPUT (uint8_t)((1))
void setConfigs();
/* Confgiruacion Perifericos ----------------------- */
void configTIMERS();
void configGPIO();
void configDMA(uint32_t*);
void configDAC();
void configADC();
void configUART();
void configPIN();
/* GPIO style ------------------------------- */
void UART_IntReceive();
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
int main(){
	uint32_t TABLA_DAC[60];
	uint32_t i;
	for(i=0;i<NUMERO_MUESTRAS;i++)
		{
			if(i<=15)	//De 0 a 90º
			{
				TABLA_DAC[i] = 0;
				//if(i==15) TABLA_DAC[i]= 1023;
			}
			else if(i<=30)	//De 90º a 180º
			{
				TABLA_DAC[i] = 500;
			}
			else if(i<=45)	//De 180º a 270º
			{
				TABLA_DAC[i] = 750;
			}
			else	//De 270º a 360º
			{
				TABLA_DAC[i] = 1000;
			}
			TABLA_DAC[i] = (TABLA_DAC[i]<<6);
	}
	
	configDAC();//tmp
	configDMA(TABLA_DAC);//tmp
	while(TRUE){

	}
    return FALSE;}
void setConfigs(){
	//configTIMERS();
	//configGPIO();
	//configDMA();
	//configDAC();
	//configADC();
	//configUART();
	//configPIN();
	return;}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void configTIMERS(){
    TIM_TIMERCFG_Type struct_timer;
    TIM_MATCHCFG_Type struct_match;
    struct_timer.PrescaleOption=TIM_PRESCALE_USVAL;
    struct_timer.PrescaleValue=100;
    struct_match.MatchChannel=0;
    struct_match.IntOnMatch=ENABLE;
    struct_match.StopOnMatch=DISABLE;//Que no se detenga luego
    struct_match.ResetOnMatch=ENABLE;
    struct_match.ExtMatchOutputType=TIM_EXTMATCH_NOTHING;
    struct_match.MatchValue=10000;
    TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&struct_timer);
    TIM_ConfigMatch(LPC_TIM0,&struct_match);
    TIM_Cmd(LPC_TIM0,ENABLE);
    NVIC_EnableIRQ(TIMER0_IRQn);
    return;}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void configDMA(uint32_t* tmptestDMA){
    GPDMA_LLI_Type struct_lli_dma;
    GPDMA_Channel_CFG_Type struct_ch_dma;
    struct_lli_dma.SrcAddr=(uint32_t)tmptestDMA;//Suele ser la direccion debe ser la der array
    struct_lli_dma.DstAddr=(uint32_t)&(LPC_DAC->DACR);//Suele ser la direccion del registro de algun periferico.
    struct_lli_dma.NextLLI=(uint32_t)&struct_lli_dma; //?
    struct_lli_dma.Control=(60)|(2<<18)|(2<<21)|(1<<26);// Es el registro de Control Tabla 564
    // ..60 .. Tranfer size. (64 bytes?)
    // ..(2<<18)..  Source Width de 32 bits.
    // ..(2<<21)..  Destination Witch de 32 bits
    // ..(1<<26)..	Para que el source se incremente en 1 despues de cada transferencia
    GPDMA_Init();
    struct_ch_dma.ChannelNum=0;//Selecciono el Channel0
    struct_ch_dma.SrcMemAddr=(uint32_t)(tmptestDMA);
    struct_ch_dma.DstMemAddr=0;//No se usa porque el destino es periferico
    struct_ch_dma.TransferSize=60; //?
    struct_ch_dma.TransferWidth=0;//No se usa. Es para M2M
    struct_ch_dma.TransferType=GPDMA_TRANSFERTYPE_M2P; // El tipo de tanseferncia
    struct_ch_dma.SrcConn=0;// No se usa. Es para P2M P2P
    struct_ch_dma.DstConn=GPDMA_CONN_DAC;//Destino de Conexion: El DAC
    struct_ch_dma.DMALLI=(uint32_t)&struct_lli_dma;
    GPDMA_Setup(&struct_ch_dma);
    GPDMA_ChannelCmd(0,ENABLE);//Activo el Canal 0
    return;}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void configUART(){
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	UART_ConfigStructInit(&UARTConfigStruct);
	UART_Init(LPC_UART0, &UARTConfigStruct);
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
	UART_FIFOConfig(LPC_UART0, &UARTFIFOConfigStruct);
	UART_IntConfig(LPC_UART0, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(LPC_UART0, UART_INTCFG_RLS, ENABLE);
	NVIC_EnableIRQ(UART0_IRQn);
	return;}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void configGPIO(){
	GPIO_SetDir(PORT0,PIN22,OUTPUT);
	return;}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void configPIN(){
	PINSEL_CFG_Type pinsel_cfg;
	pinsel_cfg.Portnum=PINSEL_PORT_0;
	pinsel_cfg.Pinnum=PINSEL_PIN_22;
	pinsel_cfg.Funcnum=PINSEL_FUNC_0;
	pinsel_cfg.Pinmode=PINSEL_PINMODE_PULLUP;
	pinsel_cfg.OpenDrain=PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&pinsel_cfg);
	pinsel_cfg.Portnum=PINSEL_PORT_0;
	pinsel_cfg.Pinnum=PINSEL_PIN_26;
	pinsel_cfg.Funcnum=PINSEL_FUNC_2;//?
	pinsel_cfg.Pinmode=PINSEL_PINMODE_PULLUP;
	pinsel_cfg.OpenDrain=PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&pinsel_cfg);
	return;}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void configDAC(){
	DAC_CONVERTER_CFG_Type DAC_Struct;
	DAC_Struct.CNT_ENA =SET;		//Activa el modo timeout
	DAC_Struct.DMA_ENA = SET;		//Activa el modo
	DAC_Init(LPC_DAC);
	DAC_ConfigDAConverterControl(LPC_DAC, &DAC_Struct);	
	return;}
void configADC(){return;}

void UART0_IRQHandler(void){
		uint32_t intsrc, tmp, tmp1;
		//Determina la fuente de interrupción
		intsrc = UART_GetIntId(LPC_UART0);
		tmp = intsrc & UART_IIR_INTID_MASK;
		// Evalúa Line Status
		if (tmp == UART_IIR_INTID_RLS){
				tmp1 = UART_GetLineStatus(LPC_UART0);
				tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE  | UART_LSR_BI | UART_LSR_RXFE);
				// ingresa a un Loop infinito si hay error
				if (tmp1) {
						while(1){};
				}
		}
		// Receive Data Available or Character time-out
		if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
				UART_IntReceive();

		}
		return;}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void TIMER0_IRQHandler(void){
	TIM_ClearIntPending(LPC_TIM0,TIM_MR0_INT);
	uint32_t tmp_port0=GPIO_ReadValue(PORT0);
	if(tmp_port0&PIN22)
		GPIO_ClearValue(PORT0,PIN22);
	else
		GPIO_SetValue(PORT0,PIN22);
	return;
}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void UART_IntReceive(void){
	/*
	UART_Receive(LPC_UART0, info, sizeof(info), NONE_BLOCKING);
	return;
	*/
	return;
}
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
void testTimer(void){
	configGPIO();
	configTIMERS();
	return;
}
