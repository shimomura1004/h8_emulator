#ifndef _TMR_COMMON_INCLUDED_
#define _TMR_COMMON_INCLUDED_

typedef enum {
    ORIG = 0,
    DIV8,
    DIV64,
    DIV8192,
} CLOCK_KIND;

extern const double periods[4];

#endif
