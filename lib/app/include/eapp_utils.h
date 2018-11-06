#ifndef _EAPP_UTILS_
#define _EAPP_UTILS_

// Arguments are in a0, just need to tell it that they are
#define EAPP_ARGUMENT(var) register unsigned long var asm("a0");

// This is a hacky way of getting the return value into a0, works for now
void EAPP_RETURN(unsigned long rval) __attribute__((noreturn));

#define EAPP_ENTRY __attribute__((__section__(".text._start")))

#endif /* _EAPP_UTILS_ */
