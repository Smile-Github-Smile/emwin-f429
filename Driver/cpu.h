/**
  ******************************************************************************
  * @file    cpu.h
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 
#ifndef __CPU_H__
#define __CPU_H__

#include "stm32f4xx.h" 
#include "stm32f4xx_rcc.h"
#include "includes.h"





int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void systemRunLedTimer_ISR(void);
void setSystemLedZt(BOOLEAN State);
void setBlLedZt(BOOLEAN State);
void hardwareInit(void);

#endif	// __CPU_H__

/* ------------------------------------------end of file---------------------------------------- */
