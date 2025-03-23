#ifndef ADC_DECTION_H
#define ADC_DECTION_H

#include "adc.h"
#include "math.h"
#include "freertos_start.h"

// 初始化adc通道
void adc_init(void);

// 开启adc转换
void adc_start(void);

// 返回处理完毕的数据地址
float *adc_get_data(void);

#endif /* adc_dection.h */
