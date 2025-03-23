#include "adc_dection.h"

// adc数据存储地址，0 -- 电池电量， 1 -- 热敏电阻
uint32_t adc_data[2] = {0};

// 存储adc转换后的数据，0 -- 电池电量， 1 -- 热敏电阻
float adc_data_float[2] = {0};

// 记录当前中断了多少次，中断满10次进行滤波处理
uint8_t isr_count = 0;
uint8_t compltflag = 0;

/**
 * @brief 获取一次电池的电量以及温度
 */
void adc_start(void)
{
    // 打开adc转换，adc转换结束触发adc中断
    HAL_ADC_Start_DMA(&hadc1, adc_data, 2);
    // 将数据完成标志位置0
    compltflag = 0;
}

/**
 * @brief 温度计算函数
 */
float em_temp_calculate(float Rt)
{
    float Rp = 30000; // 30k
    float T2 = 273.15 + 25;
    float Bx = 3950; // B值
    float Ka = 273.15;
    float temp = 0.0f;

    temp = 1 / (log(Rt / Rp) / Bx + 1 / T2) - Ka + 0.5;
    return temp;
}

/**
 * @brief adc中断回调函数
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // 停止传输处理数据
    HAL_ADC_Stop_DMA(&hadc1);

    // 中断次数递增
    isr_count++;

    // 滤波处理
    if (isr_count == 10)
    {
        // 电量(电压：1.65-2.1，用0-100进行映射)
        adc_data_float[0] += adc_data[0]; // 加上最后一次所获得的值
        // printf("电量为：%f", (adc_data_float[0] / 10 / 4096 * 3.3 - 1.65));
        adc_data_float[0] = (adc_data_float[0] / 10 / 4096 * 3.3 - 1.65) / (2.1 - 1.65) * 100;

        // 温度
        adc_data_float[1] += adc_data[1]; // 加上最后一次所获得的值
        adc_data_float[1] = ((adc_data_float[1] / 10 / 4096 * 3.3) * 10000) / (3.3 - (adc_data_float[1] / 10 / 4096 * 3.3));
        adc_data_float[1] = em_temp_calculate(adc_data_float[1]);

        // 将count清0
        isr_count = 0;
        // 将数据处理完毕的标志位置1
        compltflag = 1;
    }
    // 每收集到10次数据时，继续进行数据收集
    else if (isr_count < 10)
    {
        // 将接收到的值加到adc_data_float中
        adc_data_float[0] += (float)(adc_data[0] & 0xffff);
        adc_data_float[1] += (float)(adc_data[1] & 0xffff);

        // 开启下一次数据采集
        adc_start();
    }
}

/**
 * @brief 返回处理完毕的数据
 */
float *adc_get_data(void)
{
    if (compltflag)
        return adc_data_float;
    else
        return NULL;
}

/**
 * @brief adc初始化
 */
void adc_init(void)
{
    // 校准函数
    HAL_ADCEx_Calibration_Start(&hadc1);
}
