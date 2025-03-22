#include "key.h"

/* 按键触发的是按下中断还是抬起中断 */
uint8_t key_mode_flag;

/* 当前按键模式 */
key_type_t key_mode;

/**
 * @brief  重写外部中断回调函数
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY_Pin)
    {

        // 按键按下
        if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
        {
            key_mode_flag = 0;
        }
        // 按键抬起
        else
        {
            key_mode_flag = 1;
        }

        // 无论按键抬起或按下都要进行按键消抖，都要开启定时器
        HAL_TIM_Base_Start_IT(&htim3);
    }
}

/**
 * @brief  获取当前按键模式
 */
key_type_t key_get_mode(void)
{
    return key_mode;
}

/**
 * @brief  按键初始化
 */
void key_init(void)
{
    key_mode = KEY_MODE_LONG_RELEASE;
}
