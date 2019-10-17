/*Bridge between MISO and MOSI
  SSEL -- Air
  PCLK -- Airs
*/
#include "LPC17xx.h"
#include "lpc17xx_spi.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#define SPI_DATABIT_SIZE 10
#define BUFFER_SIZE 0x40
void confPin(void);
void confSPI(void);
void readWrite(uint16_t*,uint16_t*,uint32_t);
int main (void){
	uint16_t Tx_Buf[BUFFER_SIZE];
	uint16_t Rx_Buf[BUFFER_SIZE];
	confPin();
	confSPI();
	for (uint8_t i = 0; i < BUFFER_SIZE/2; i++) {
		Tx_Buf[i] = i;
		Rx_Buf[i] = 0;
	}
	readWrite(Tx_Buf,Rx_Buf,0);
	while(1){}
return 0;}
void confPin(void){
	PINSEL_CFG_Type PinCfg;
	/*
	 * Initialize SPI pin connect
	 * P0.15 - SCK;
	 * P0.16 - SSEL - used as GPIO
	 * P0.17 - MISO
	 * P0.18 - MOSI
	 */
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 15;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 17;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 18;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 16;
	PinCfg.Funcnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	return;}
void confSPI(void){
	SPI_CFG_Type SPI_ConfigStruct;
	SPI_ConfigStruct.CPHA = SPI_CPHA_SECOND;//CPHA=1
	SPI_ConfigStruct.CPOL = SPI_CPOL_LO;// CPOL=0
	SPI_ConfigStruct.ClockRate = 2000000;//2Mhz
	SPI_ConfigStruct.DataOrder = SPI_DATA_MSB_FIRST;
	SPI_ConfigStruct.Databit = SPI_DATABIT_SIZE;//10?
	SPI_ConfigStruct.Mode = SPI_MASTER_MODE;
	// Initialize SPI peripheral with parameter given in structure above
	SPI_Init(LPC_SPI, &SPI_ConfigStruct);
return;}
void readWrite(uint16_t* Tx_Buf,uint16_t* Rx_Buf,uint32_t len){
	SPI_DATA_SETUP_Type xferConfig;
	xferConfig.tx_data = Tx_Buf;
	xferConfig.rx_data = Rx_Buf;
	xferConfig.length = BUFFER_SIZE;
	len = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);//Returns 64
	SPI_DeInit(LPC_SPI);
return;}
