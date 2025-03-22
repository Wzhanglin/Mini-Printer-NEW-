#ifndef LED_H
#define LED_H
#include "gpio.h"

// led的四种状态
typedef enum
{
    LED_OFF = 0,
    LED_ON,
    LED_BLINK_FAST,
    LED_BLINK_SLOW
} led_state_t;

// led 初始化
void led_init(void);

// led亮
void led_on(void);

// led灭
void led_off(void);

// led闪烁
void led_blink(void);

// 获取当前led状态
led_state_t led_get_state(void);

// 设置led状态
void led_set_state(led_state_t state);

#endif /* led.h */
