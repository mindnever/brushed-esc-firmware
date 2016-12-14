#ifndef _AFRO_30A_H_
#define _AFRO_30A_H_

#include "Macros.h"

/* Afro 30A */
#define CpFET_PIN           1
#define CpFET_PORT          B

#define BpFET_PIN           2
#define BpFET_PORT          B

#define ApFET_PIN           2
#define ApFET_PORT          D

#define CnFET_PIN              5
#define CnFET_PORT             D

#define BnFET_PIN              4
#define BnFET_PORT             D

#define AnFET_PIN              3
#define AnFET_PORT             D

#define Rcp_In_PIN          0
#define Rcp_In_PORT         B


#define LED1_PIN 3
#define LED1_PORT C

#define LED2_PIN 2
#define LED2_PORT C


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