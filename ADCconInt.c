#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_adc.h"
#define TRUE 1
#define FALSE 0
#define LOW	0
#define HIGH 1
#define PIN22 (uint32_t)(1<<22)
#define PORT0 (uint8_t)((0))
#define OUTPUT (uint8_t)((1))
void configTIMERS();
void configGPIO();
void configPIN();
void configADC();
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
int main(){
	GPIO_SetDir(PORT0, PIN22,OUTPUT);
	configPIN();
	configADC();
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
void configPIN(){
	PINSEL_CFG_Type PinCfg;
	PinCfg.Portnum=PINSEL_PORT_0;    /**< Port Number, should be PINSEL_PORT_x,
	                        where x should be in range from 0 to 4 */
	PinCfg.Pinnum=PINSEL_PIN_23;        /**< Pin Number, should be PINSEL_PIN_x,
	                        where x should be in range from 0 to 31 */
	PinCfg.Funcnum=PINSEL_FUNC_1;    /**< Function Number, should be PINSEL_FUNC_x,
	                        where x should be in range from 0 to 3 */
	PinCfg.Pinmode= PINSEL_PINMODE_TRISTATE;   /**< Pin Mode, should be:
	                        - PINSEL_PINMODE_PULLUP: Internal pull-up resistor
	                        - PINSEL_PINMODE_TRISTATE: Tri-state
	                        - PINSEL_PINMODE_PULLDOWN: Internal pull-down resistor */
	PinCfg.OpenDrain=PINSEL_PINMODE_NORMAL;    /**< OpenDrain mode, should be:
	                        - PINSEL_PINMODE_NORMAL: Pin is in the normal (not open drain) mode
	                        - PINSEL_PINMODE_OPENDRAIN: Pin is in the open drain mode */
	PINSEL_ConfigPin(&PinCfg);
	return;
}
void configADC(void){
	ADC_Init(LPC_ADC, 100000);
	ADC_BurstCmd(LPC_ADC, ENABLE);
	ADC_ChannelCmd (LPC_ADC,0, ENABLE);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN0,ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);
	return;
}
void ADC_IRQHandler(){
	uint16_t ADC0Value=0;
	ADC0Value=ADC_ChannelGetData(LPC_ADC,0);
	if(ADC0Value<2054)
		GPIO_SetValue(PORT0,PIN22);
	else
		GPIO_ClearValue(PORT0,PIN22);
	return;
}
