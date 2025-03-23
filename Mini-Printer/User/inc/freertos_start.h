#ifndef FREERTOS_START_H
#define FREERTOS_START_H

#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "key.h"
#include "adc_dection.h"

int fputc(int c, FILE *f);

void freertos_start(void);

#endif /* freeRTos */
