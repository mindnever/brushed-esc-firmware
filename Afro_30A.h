#ifndef _AFRO_30A_H_
#define _AFRO_30A_H_

#include "Macros.h"

/*
 * High side is slow to go on (10s of us) but fast to go off
 *
.EQU    NFETON_DELAY                    =       7       ; Wait delay from pfets off to nfets on
.EQU    PFETON_DELAY                    =       250     ; Wait delay from nfets off to pfets on
.EQU    ADC_LIMIT_L                     =       186     ; 3k3/18k divider. Power supply measurement ADC value for which motor power is limited (low byte)
.EQU    ADC_LIMIT_H                     =       0       ; 3k3/18k divider. Power supply measurement ADC value for which motor power is limited (2 MSBs)
.EQU    TEMP_LIMIT                      =       185     ; 3k3/10kNTC. Temperature measurement ADC value for which main motor power is limited
.EQU    TEMP_LIMIT_STEP         =       15      ; 3k3/10kNTC. Temperature measurement ADC value increment for which main motor power is further limited
*/

/* Afro 30A */
#define CpFET_PIN 1
#define CpFET_PORT B

#define BpFET_PIN 2
#define BpFET_PORT B

#define ApFET_PIN 2
#define ApFET_PORT D

#define CnFET_PIN 5
#define CnFET_PORT D

#define BnFET_PIN 4
#define BnFET_PORT D

#define AnFET_PIN 3
#define AnFET_PORT D

#define Rcp_In_PIN 0
#define Rcp_In_PORT B

#define LED1_PIN 3
#define LED1_PORT C

#define LED2_PIN 2
#define LED2_PORT C

/* 3k3/18k */
#define Volt_PIN 7
#define Volt_PORT C

#define Temp_PIN 6
#define Temp_PORT C

/* ?? but definitely output */
#define Rpm_Out_PIN 4
#define Rpm_Out_PORT C

#define Debug_Out_PIN 4
#define Debug_Out_PORT B

/* ADMUX value for phase A & B (analog comparator mux enabled)
 * For phase C, we disable analog comparator mux
*/
#define MUX_A 0
#define MUX_B 1

/*
 * Also available:
 */

#define UART_RXD_PIN 0
#define UART_RXD_PORT D

#define UART_TXD_PIN 1
#define UART_TXD_PORT D

#define I2C_SCL_PIN 5
#define I2C_SCL_PORT C

#define I2C_SDA_PIN 4
#define I2C_SDA_PORT C

#define ApFET_ON PIN_LOW(ApFET)
#define BpFET_ON PIN_LOW(BpFET)
#define CpFET_ON PIN_LOW(CpFET)

#define ApFET_OFF PIN_HIGH(ApFET)
#define BpFET_OFF PIN_HIGH(BpFET)
#define CpFET_OFF PIN_HIGH(CpFET)

#define AnFET_ON PIN_HIGH(AnFET)
#define BnFET_ON PIN_HIGH(BnFET)
#define CnFET_ON PIN_HIGH(CnFET)

#define AnFET_OFF PIN_LOW(AnFET)
#define BnFET_OFF PIN_LOW(BnFET)
#define CnFET_OFF PIN_LOW(CnFET)

#define LED1_ON PIN_LOW(LED1)
#define LED1_OFF PIN_HIGH(LED1)

#define LED2_ON PIN_LOW(LED2)
#define LED2_OFF PIN_HIGH(LED2)

#endif /* _AFRO_30A_H_ */
