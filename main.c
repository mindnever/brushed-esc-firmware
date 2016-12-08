#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define CpFET_PIN           5
#define CpFET_PORT          D

#define BpFET_PIN           4
#define BpFET_PORT          D

#define ApFET_PIN           3
#define ApFET_PORT          D

#define CnFET_PIN              2
#define CnFET_PORT             B

#define BnFET_PIN              1
#define BnFET_PORT             B

#define AnFET_PIN              0
#define AnFET_PORT             B


#define Rcp_In_PIN          2
#define Rcp_In_PORT         D


//#define Mux_C           PC5
//#define Mux_B           PC4
//#define Temp_Ip         PC1
//#define Volt_Ip         PC0


//#define DebugPin           PB4

#define _CONCAT(x,y) x##y

#define _DDR(x) DDR##x
#define _PORT(x) PORT##x
#define _PIN(x) PIN##x

#define DDR(x) _DDR(x)
#define PORT(x) _PORT(x)
#define PIN(x) _PIN(x)

#define INIT_OUT(x) DDR(_CONCAT(x, _PORT)) |= (1 << (x ## _PIN))
#define INIT_IN(x) DDR(_CONCAT(x, _PORT)) &= ~(1 << (x ## _PIN))

#define ON(x) PORT(_CONCAT(x, _PORT)) |= (1 << (x ## _PIN))
#define OFF(x) PORT(_CONCAT(x, _PORT)) &= ~(1 << (x ## _PIN))

#define IN(x) (PIN(_CONCAT(x, _PORT)) & (1 << (x ## _PIN)))

static volatile uint8_t overflow = 0;

int main(void)
{
    OFF(CpFET);
    OFF(BpFET);
    OFF(ApFET);
    
    OFF(CnFET);
    OFF(BnFET);
    OFF(AnFET);
    
    INIT_OUT(CpFET);
    INIT_OUT(BpFET);
    INIT_OUT(ApFET);
    
    INIT_OUT(CnFET);
    INIT_OUT(BnFET);
    INIT_OUT(AnFET);
    
    INIT_IN(Rcp_In);
    

    /* USE C & B fets for switching */

    uint16_t TCNTstart = 0;
    uint16_t Pulse = 0;
    
    // 1MHz timer clock = 1uS precission
    
    TCCR1A = 0;
    TCCR1B = _BV(CS11); // prescaler: clock/8
    TIMSK = _BV(TOIE1); // overflow interrupt enable
    
    sei();
    
    uint8_t edge = 0;
    uint8_t forward = 0;
    
    uint16_t PWMMargin = 15;
    uint16_t PWMMiddle = 1500;
    uint16_t PWMMin = PWMMiddle - 512;
    uint16_t PWMMax = PWMMiddle + 512;

    while(1) {
        
        if (!edge && IN(Rcp_In)) {
            TCNTstart = TCNT1;        /* capture the time when the signal when high*/
            edge = 1;
        }

        if (edge && !IN(Rcp_In)) {
            uint16_t p = (TCNT1 - TCNTstart);
            uint8_t valid = 1;
            
            edge = 0;
            
            if((p < PWMMin) || (p > PWMMax)) {

                valid = 0;

            } else if(p < (PWMMiddle - PWMMargin)) {

                forward = 0;
                Pulse = PWMMiddle - p;

            } else if(p > (PWMMiddle + PWMMargin)) {

                forward = 1;
                Pulse = p - PWMMiddle;

            } else {

                Pulse = 0;
            }
            
            if(valid) {
                overflow = 0;
            }
            
        }
        
        if(overflow > 3) {
            Pulse = 0;
        }
        
        /* pulse 0-512 */
        
        uint8_t on = (TCNT1 & 0x1FF) < Pulse;

//uint8_t on = TCNT1 & 0x10;
        
        if(!on) {
#if 1
            OFF(BpFET);
            OFF(BnFET);
            OFF(CpFET);
            OFF(CnFET);
#else
            OFF(BpFET);
            OFF(CpFET);
            
            _delay_us(5);
            
            ON(BnFET);
            ON(CnFET);
#endif
        } else if(forward) {
            // Forward //

            
            OFF(BpFET);
            OFF(CnFET);

            _delay_us(5);
            
            ON(CpFET);
            ON(BnFET);

            
        } else {
            
            // Reverse //
            OFF(BnFET);
            OFF(CpFET);

            _delay_us(5);
            
            ON(CnFET);
            ON(BpFET);


            
        }
    }
}

ISR( TIMER1_OVF_vect )
{
    if(overflow < 10) {
        ++overflow;
    }
}
