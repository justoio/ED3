/*
===============================================================================
 Name        : dma.c
 Author      : @justoio
 Version     : 
 Copyright   : $(copyright)
 Description : Genera una señal escalera en el PIN0.26 usando DMA para mover
 		datos desde memoria (un array) a un periférico (DAC).
===============================================================================
*/
#include "LPC17xx.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"
#define TRUE 1
#define FALSE 0
#define LOW	0
#define HIGH 1
#define PIN22 (uint32_t)(1<<22)
#define PORT0 (uint8_t)((0))
#define OUTPUT (uint8_t)((1))
void configDMA(uint32_t*);
void configDAC();
void configPIN();
void loadTABLA();
/*********************************************************************
 * @brief
 * @param[in]     None
 * @return        None
 *
 * Note:
 **********************************************************************/
int main(){
	uint32_t TABLA_DAC[60];
	loadTABLA(TABLA_DAC);
	configPIN();
	configDAC();//tmp
	configDMA(TABLA_DAC);
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
void configDAC(){
	DAC_CONVERTER_CFG_Type DAC_Struct;
	DAC_Struct.CNT_ENA =SET;		//Activa el modo timeout
	DAC_Struct.DMA_ENA = SET;		//Activa el modo
	DAC_Init(LPC_DAC);
	DAC_ConfigDAConverterControl(LPC_DAC, &DAC_Struct);	
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
void loadTABLA(uint32_t* TABLA_DAC){
	uint32_t i;
	for(i=0;i<60;i++){
		if(i<=15){
			TABLA_DAC[i] = 0;
		}else if(i<=30){
			TABLA_DAC[i] = 500;
		}else if(i<=45){
			TABLA_DAC[i] = 750;
		}else{
			TABLA_DAC[i] = 1000;
		}
		TABLA_DAC[i] = (TABLA_DAC[i]<<6);
	}	
return;}
