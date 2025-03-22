#include "led.h"

// 当前led状态
static led_state_t led_state;

/**
 * @brief LED初始化
 */
void led_init(void)
{
    led_off();
    led_state = LED_OFF;
}

/**
 * @brief LED亮
 */
void led_on(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

/**
 * @brief LED灭
 */
void led_off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

/**
 * @brief led闪烁
 */
void led_blink(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

/**
 * @brief 获取led状态
 * @return 当前led状态
 */
led_state_t led_get_state(void)
{
    return led_state;
}

/**
 * @brief 设置led状态
 * @param 需要设置的状态
 */
void led_set_state(led_state_t state)
{
    led_state = state;
}
