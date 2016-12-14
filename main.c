#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "Afro_30A.h"


#define  T1_TICKS_PER_US (F_CPU/1000000)

#define  T1_INPUT_PWM_RANGE (512 * T1_TICKS_PER_US)
#define  T1_INPUT_PWM_MARGIN (12 * T1_TICKS_PER_US)
#define  T1_INPUT_PWM_NEUTRAL (1500 * T1_TICKS_PER_US)
#define  T1_INPUT_PWM_MIN (T1_INPUT_PWM_NEUTRAL - T1_INPUT_PWM_RANGE)
#define  T1_INPUT_PWM_MAX (T1_INPUT_PWM_NEUTRAL + T1_INPUT_PWM_RANGE)


#define  T1_OUTPUT_PWM_RANGE 1024
#define  T1_OUTPUT_TCNT_MASK (T1_OUTPUT_PWM_RANGE-1)

/* x is 0 - T1_INPUT_PWM_RANGE */
/* x is 0 - 8192 => 0 - 1024 */

#define  T1_OUTPUT_PWM_PULSE(x) (x/(T1_INPUT_PWM_RANGE/T1_OUTPUT_PWM_RANGE))

static volatile uint8_t overflow = 0;

int main(void)
{
    CpFET_OFF;
    BpFET_OFF;
    ApFET_OFF;
    
    CnFET_OFF;
    BnFET_OFF;
    AnFET_OFF;
    
    INIT_OUT(CpFET);
    INIT_OUT(BpFET);
    INIT_OUT(ApFET);
    
    INIT_OUT(CnFET);
    INIT_OUT(BnFET);
    INIT_OUT(AnFET);
    
    INIT_OUT(LED1);
    INIT_OUT(LED2);
    
    INIT_IN(Rcp_In);
    

    /* USE C & B fets for switching */

    uint16_t TCNTstart = 0;
    uint16_t Pulse = 0;
    
    TCCR1A = 0;
    TCCR1B = _BV(CS10); // prescaler: clk/1 (no prescaling)
    TIMSK = _BV(TOIE1); // overflow interrupt enable
    
    sei();
    
    uint8_t edge = 0;
    uint8_t forward = 0;
    
    
    while(1) {
        
        if (!edge && IN(Rcp_In)) {
            TCNTstart = TCNT1;        /* capture the time when the signal when high*/
            edge = 1;
        }

        if (edge && !IN(Rcp_In)) {
            uint16_t p = (TCNT1 - TCNTstart);
            uint8_t valid = 1;
            
            edge = 0;
            
            if((p < T1_INPUT_PWM_MIN) || (p > T1_INPUT_PWM_MAX)) {

                valid = 0;

            } else if(p < (T1_INPUT_PWM_NEUTRAL - T1_INPUT_PWM_MARGIN)) {

                forward = 0;
                Pulse = T1_OUTPUT_PWM_PULSE(T1_INPUT_PWM_NEUTRAL - p);

            } else if(p > (T1_INPUT_PWM_NEUTRAL + T1_INPUT_PWM_MARGIN)) {

                forward = 1;
                Pulse = T1_OUTPUT_PWM_PULSE(p - T1_INPUT_PWM_NEUTRAL);

            } else {

                Pulse = 0;
            }
            
            if(valid) {
                overflow = 0;
            }
            
        }
        
        if(overflow > 2) {
            Pulse = 0;
            LED1_OFF;
            LED2_ON;
        } else {
            LED2_OFF;
            LED1_ON;
        }
        
        
        uint8_t on = (TCNT1 & T1_OUTPUT_TCNT_MASK) < Pulse;

//uint8_t on = TCNT1 & 0x10;
        
        if(!on) {
#if 1
            BpFET_OFF;
            BnFET_OFF;
            CpFET_OFF;
            CnFET_OFF;
#else
            BpFET_OFF;
            CpFET_OFF;
            
            _delay_us(5);
            
            BnFET_ON;
            CnFET_ON;
#endif
        } else if(forward) {
            // Forward //

            BpFET_OFF;
            CnFET_OFF;

            _delay_us(5);
            
            CpFET_ON;
            BnFET_ON;

            
        } else {
            
            // Reverse //
            BnFET_OFF;
            CpFET_OFF;

            _delay_us(5);
            
            CnFET_ON;
            BnFET_ON;
        }
    }
}

ISR( TIMER1_OVF_vect )
{
    if(overflow < 10) {
        ++overflow;
    }
}
