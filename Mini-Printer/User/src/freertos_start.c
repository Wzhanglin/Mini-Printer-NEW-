#include "freertos_start.h"

/* 启动任务配置 */
#define START_TASK_STACK 128         // 任务栈大小
#define START_TASK_PRIORITY 1        // 任务优先级，越大优先级越高(config中定义优先级onfigMAX_PRIORITIES最高为5，可修改)
TaskHandle_t start_task_handle;      // 任务句柄
void start_task(void *pvParameters); // 任务函数

/* led控制任务配置 */
#define LEDCTL_TASK_STACK 128    // 任务栈大小
#define LEDCTL_TASK_PRIORITY 2   // 任务优先级，越大优先级越高(config中定义优先级onfigMAX_PRIORITIES最高为5，可修改)                                          5???????)
TaskHandle_t ledctl_handle;      // 任务句柄
void ledctl(void *pvParameters); // 任务函数

/* 按键模块任务配置 */
#define KEYCTL_TASK_STACK 128    // 任务栈大小
#define KEYCTL_TASK_PRIORITY 2   // 任务优先级，越大优先级越高(config中定义优先级onfigMAX_PRIORITIES最高为5，可修改)                                          5???????)
TaskHandle_t keyctl_handle;      // 任务句柄
void keyctl(void *pvParameters); // 任务函数

/* adc模块任务配置 */
#define ADC_TASK_STACK 128    // 任务栈大小
#define ADC_TASK_PRIORITY 2   // 任务优先级，越大优先级越高(config中定义优先级onfigMAX_PRIORITIES最高为5，可修改)                                          5???????)
TaskHandle_t adc_handle;      // 任务句柄
void adc(void *pvParameters); // 任务函数

/**
 * @brief 启动FREERTOS
 */
void freertos_start(void)
{
    /* 创建启动任务 */
    BaseType_t flag = xTaskCreate(start_task,                               // 函数地址
                                  (char *)"start_task",                     // 任务名称
                                  (configSTACK_DEPTH_TYPE)START_TASK_STACK, // 任务栈大小，默认为128，单位4字节
                                  (void *)NULL,                             // 参数列表
                                  (UBaseType_t)START_TASK_PRIORITY,         // 任务优先级
                                  (TaskHandle_t *)&start_task_handle);      // 任务句柄
    if (flag == NULL)
    {
        printf("task \"start_task\" created defeat ! \r\n");
    }

    /*启动调度器：会自动创建空闲任务*/
    vTaskStartScheduler();
    if (xTaskGetSchedulerState() != taskSCHEDULER_RUNNING)
    {
        printf("Scheduler not running!\r\n");
    }
}

/**
 * @brief 启动任务，创建其他任务
 */
void start_task(void *pvParameters)
{
    // 进入临界区
    taskENTER_CRITICAL();

    // led控制模块
    BaseType_t flag = xTaskCreate(ledctl,                                    // 函数地址
                                  (char *)"ledctl",                          // 任务名称
                                  (configSTACK_DEPTH_TYPE)LEDCTL_TASK_STACK, // 任务栈大小，默认为128，单位4字节
                                  (void *)NULL,                              // 参数列表
                                  (UBaseType_t)LEDCTL_TASK_PRIORITY,         // 任务优先级
                                  (TaskHandle_t *)&ledctl_handle);           // 任务句柄
    if (flag == NULL)
    {
        printf("task \"ledctl\" created defeat ! \r\n");
    }

    // 按键控制模块
    flag = xTaskCreate(keyctl,                                    // 函数地址
                       (char *)"keyctl",                          // 任务名称
                       (configSTACK_DEPTH_TYPE)KEYCTL_TASK_STACK, // 任务栈大小，默认为128，单位4字节
                       (void *)NULL,                              // 参数列表
                       (UBaseType_t)KEYCTL_TASK_PRIORITY,         // 任务优先级
                       (TaskHandle_t *)&keyctl_handle);           // 任务句柄
    if (flag == NULL)
    {
        printf("task \"keyctl\" created defeat ! \r\n");
    }

    // 按键控制模块
    flag = xTaskCreate(adc,                                    // 函数地址
                       (char *)"adc",                          // 任务名称
                       (configSTACK_DEPTH_TYPE)ADC_TASK_STACK, // 任务栈大小，默认为128，单位4字节
                       (void *)NULL,                           // 参数列表
                       (UBaseType_t)ADC_TASK_PRIORITY,         // 任务优先级
                       (TaskHandle_t *)&adc_handle);           // 任务句柄
    if (flag == NULL)
    {
        printf("task \"adc\" created defeat ! \r\n");
    }

    // 删除自己释放栈空间
    vTaskDelete(NULL);

    // 退出临界区
    taskEXIT_CRITICAL();
}

/**
 * @brief  LED控制任务
 * @param
 */
void ledctl(void *pvParameters)
{
    while (1)
    {
        // 获取当前节拍
        TickType_t tick = xTaskGetTickCount();

        // 获取当前led状态
        led_state_t state = led_get_state();
        // printf("the current led state is:[%d]\r\n", state);
        switch (state)
        {
        case LED_OFF:
            led_off();
            break;
        case LED_ON:
            led_on();
            break;
        case LED_BLINK_FAST:
            led_blink();
            vTaskDelay(200);
            led_blink();
            vTaskDelay(200);
            led_blink();
            break;
        case LED_BLINK_SLOW:
            led_blink();
            vTaskDelay(600);
            break;
        }

        // 每隔600ms检查一次led状态
        vTaskDelayUntil(&tick, 600);
    }
}

/**
 * @brief 按键控制模块
 */
void keyctl(void *pvParameters)
{
    while (1)
    {
        if (key_get_mode() == KEY_MODE_SHORT)
        {
            // printf("key_mode_short\r\n");
        }
        else if (key_get_mode() == KEY_MODE_LONG)
        {
            // printf("key_mode_long\r\n");
        }
        else if (key_get_mode() == KEY_MODE_LONG_RELEASE)
        {
            // printf("key_mode_long_release\r\n");
        }
        // 每500ms判定一次当前按键的状态
        vTaskDelay(500);
    }
}

/**
 * @brief adc控制模块
 */
void adc(void *pvParameters)
{
    // 记录上次所测的数据，当无法测出来时，则输出上次所测的数据
    float last_adc_value[2] = {0};
    while (1)
    {
        TickType_t pxPreviousWakeTime = xTaskGetTickCount();

        // 开启adc转换
        adc_start();

        // 等待500ms等待处理完毕
        vTaskDelay(500);

        float *adc_data = adc_get_data();
        // 如果获取到数据，将数据赋值并发往消息队列
        if (adc_data != NULL)
        {
            last_adc_value[0] = adc_data[0];
            last_adc_value[1] = adc_data[1];
        }
        else
        {
            printf("failed to get new data: \r\n");
        }

        printf("adc_data: %f, %f\r\n", last_adc_value[0], last_adc_value[1]);

        // 每秒开启一次温度和电量检测
        xTaskDelayUntil(&pxPreviousWakeTime, 1000);
    }
}

/**
 * @brief 重载printf函数
 */
int fputc(int c, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 0xff);
    return c;
}
