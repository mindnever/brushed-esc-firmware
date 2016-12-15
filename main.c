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
#define  T1_OUTPUT_PWM_OFFSET 64

/* x is 0 - T1_INPUT_PWM_RANGE */
/* x is 0 - 8192 => 0 - 1024 */

#define  T1_OUTPUT_PWM_PULSE(x) (((x)/(T1_INPUT_PWM_RANGE/T1_OUTPUT_PWM_RANGE)) + T1_OUTPUT_PWM_OFFSET)
//#define T1_OUTPUT_PWM_PULSE(x) ((x)>>3)

static volatile uint8_t timeout, overflow;
static volatile uint16_t Pulse;
static volatile uint8_t forward;

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

    TCCR1A = 0;
    TCCR1B = _BV(CS10) | _BV(ICNC1) | _BV(ICES1); // prescaler: clk/1 (no prescaling), input capture noise canceler, rising edge
    TIMSK = _BV(TOIE1) | _BV(TICIE1); // overflow interrupt enable, input capture enable
    
    sei();
    
    while(1) {
        
        uint8_t on = (TCNT1 & 0x3FF) < Pulse;

        if(timeout > 30) {
            on = 0;
            LED2_OFF;
            LED1_ON;
        } else {
            LED1_OFF;
            LED2_ON;
        }

        
        if(!on) {
#if 0
            BpFET_OFF;
            BnFET_OFF;
            CpFET_OFF;
            CnFET_OFF;
#else
            BpFET_OFF;
            CpFET_OFF;
            
            _delay_us(1);
            
            BnFET_ON;
            CnFET_ON;
#endif
        } else if(forward) {
            // Forward //

            BpFET_OFF;
            CnFET_OFF;

            _delay_us(1);
            
            CpFET_ON;
            BnFET_ON;

            
        } else {
            
            // Reverse //
            BnFET_OFF;
            CpFET_OFF;

            _delay_us(1);
            
            CnFET_ON;
            BpFET_ON;
        }
    }
}


ISR( TIMER1_OVF_vect )
{
    if(overflow < 255) {
        ++overflow;
    }
    if(timeout < 255) {
        ++timeout;
    }
}

ISR( TIMER1_CAPT_vect )
{
    static uint16_t risingICP;
    
    if( TCCR1B & _BV(ICES1) ) { // Rising edge

        risingICP = ICR1;
        
        TCCR1B &= ~_BV(ICES1);
        
    } else {

        uint16_t p = ICR1 - risingICP;

        TCCR1B |= _BV(ICES1);
        
        if(overflow < 2) {

            timeout = 0;

            if((p < T1_INPUT_PWM_MIN) || (p > T1_INPUT_PWM_MAX)) {


            } else if(p < (T1_INPUT_PWM_NEUTRAL - T1_INPUT_PWM_MARGIN)) {

                forward = 0;
                Pulse = T1_OUTPUT_PWM_PULSE(T1_INPUT_PWM_NEUTRAL - p);
                
            } else if(p > (T1_INPUT_PWM_NEUTRAL + T1_INPUT_PWM_MARGIN)) {

                forward = 1;
                Pulse = T1_OUTPUT_PWM_PULSE(p - T1_INPUT_PWM_NEUTRAL);

            } else {

                Pulse = 0;
            }
        }
    }
    
    overflow = 0;
}
