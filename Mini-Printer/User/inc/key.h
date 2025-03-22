#ifndef KEY_H
#define KEY_H

#include "gpio.h"
#include "tim.h"

/* 短按时间的时间，大于短按时间被判定为长按事件 */
#define key_short_mode_time 250

// 3中模式：短按(测试打印效果)、长按(电机转动)、长按释放(停止转动)
typedef enum
{
    KEY_MODE_SHORT,
    KEY_MODE_LONG,
    KEY_MODE_LONG_RELEASE,
} key_type_t;

// 按键初始化
void key_init(void);

// 获取按键事件
key_type_t key_get_mode(void);

#endif /* key.h */
