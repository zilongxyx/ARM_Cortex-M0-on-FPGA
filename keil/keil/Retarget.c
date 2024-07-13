/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include <stdio.h>
#include <time.h>
#include <rt_misc.h>
#include "code_def.h"

int uart_out(int ch);

#define UART_Tx 0x40010008

#pragma import(__use_no_semihosting_swi)


struct __FILE { unsigned char * ptr; /* Add whatever you need here */ };

FILE __stdout = {(unsigned char *)UART_Tx};
FILE __stdin;


int fputc(int ch, FILE *f) {
  return (uart_out(ch));
}

int uart_out(int ch)
{
	unsigned char * UARTPtr;
	UARTPtr  = (unsigned char *)UART_Tx;
	*UARTPtr = (char)ch;
	return ch;
}
	
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return 0;
}


void _ttywrch(int ch) {
  fputc(ch, &__stdout);
}


void _sys_exit(void) {
  while (1);    /* endless loop */
}
