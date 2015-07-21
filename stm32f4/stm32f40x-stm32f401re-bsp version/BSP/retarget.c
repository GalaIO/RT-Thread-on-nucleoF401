#include <stdio.h>
#include "bsp_uart.h"

#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

struct __FILE {

  int handle;                 // Add whatever you need here 

};

FILE __stdout;

FILE __stdin;

int fputc(int ch, FILE *f){
  UARTsendch((u8)ch);
  return ch;
}

int fgetc(FILE *f)
{                          
    char c;
	c=UARTgetch__();
	UARTsendch((u8)c);
	return c;
}

void _ttywrch(int ch) {

 UARTsendch (ch);

}

int ferror(FILE *f) {                            // Your implementation of ferror

  return EOF;

}

void _sys_exit(int return_code) {

label:  goto label;           // endless loop

}

