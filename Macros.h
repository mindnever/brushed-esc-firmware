#ifndef _MACROS_H_
#define _MACROS_H_

#define _CONCAT(x,y) x##y

#define _DDR(x) DDR##x
#define _PORT(x) PORT##x
#define _PIN(x) PIN##x

#define DDR(x) _DDR(x)
#define PORT(x) _PORT(x)
#define PIN(x) _PIN(x)

#define INIT_OUT(x) DDR(_CONCAT(x, _PORT)) |= (1 << (x ## _PIN))
#define INIT_IN(x) DDR(_CONCAT(x, _PORT)) &= ~(1 << (x ## _PIN))


#define PIN_HIGH(x) PORT(_CONCAT(x, _PORT)) |= (1 << (x ## _PIN))
#define PIN_LOW(x) PORT(_CONCAT(x, _PORT)) &= ~(1 << (x ## _PIN))

#define IN(x) (PIN(_CONCAT(x, _PORT)) & (1 << (x ## _PIN)))

#endif /* _MACROS_H_ */
