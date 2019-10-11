/*
===============================================================================
 Name        : FinalProject.c
 Authors      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : Timer0 con Match para parpadear un led en PIN0.22
 	       El led se encuentra 40ms en alto y 40ms en bajo.
Timer Calculation:
CLK = 100MHz  =>  PCLK=CLK/4=25MHz  => 1cycle=40ns 
		1 TC ---> (PR+1)cycles = (100+1)cylces = 4.04us
	    10000 TC ---> 40.4ms 
Los  matchs se producen cada 40.4ms. Y cambian el estado del LED.
(Ej MatchValue=250000 y PR=100 entonces interrumpe cada 1seg)
===============================================================================
*/
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#define TRUE 1
#define FALSE 0
#define LOW	0
#define HIGH 1
#define PIN22 (uint32_t)(1<<22)
#define PORT0 (uint8_t)((0))
#define OUTPUT (uint8_t)((1))
void configTIMERS();
void configGPIO();
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
int main(){
	configGPIO();
	configTIMERS();
	while(TRUE){

	}
    return FALSE;}
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
    struct_timer.PrescaleOption=TIM_PRESCALE_TICKVAL;
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
void TIMER0_IRQHandler(void){
	TIM_ClearIntPending(LPC_TIM0,TIM_MR0_INT);
	uint32_t tmp_port0=GPIO_ReadValue(PORT0);
	if(tmp_port0&PIN22)
		GPIO_ClearValue(PORT0,PIN22);
	else
		GPIO_SetValue(PORT0,PIN22);
	return;
}
