#include <stdio.h>
#include "esp_system.h"
#include "esp_attr.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "driver/timer.h"
#include "freertos/semphr.h"


#define TIMER_DIVIDER   (16)
#define GPIO1 19  // pra IN1 Ponte H
#define GPIO2 16 // pra IN2 Ponte H

const unsigned char WaveFormTable[100] = {
   50 , 53 , 56 , 59 , 62 , 65 , 68 , 71 , 74 , 76 , 79 , 81 , 84 , 86 , 88 , 90 , 92 , 93
 , 95 , 96 , 97 , 98 , 99 , 99 , 99 ,  100 , 99 , 99 , 99,  98 , 97 , 96 , 95,  93 , 92 , 90
 , 88 , 86 , 84 , 81 , 79 , 76 , 74 , 71 , 68 , 65 , 62 , 59 , 56 , 53 , 50 , 47 , 44 , 41
 , 38 , 35 , 32 , 29 , 26 , 24 , 21 , 19 , 16 , 14 , 12 , 10 ,  8  , 7  , 5  , 4  , 3  , 2
 ,  1 ,  1 ,  1 ,  0 ,  1  , 1 ,  1 ,  2 ,  3 ,  4  , 5  , 7  , 8 , 10 , 12 , 14 , 16 , 19
 , 21 , 24 , 26 , 29 , 32 , 35 , 38 , 41 , 44 };

int freq_spwm = 10;
int index_spwm=0;
static SemaphoreHandle_t timer_sem;
mcpwm_config_t pwm_config;

static bool IRAM_ATTR timer_group_isr_callback(void *args)
{
    BaseType_t high_task_awoken = pdFALSE;
    		
    if(index_spwm >= 99)
        index_spwm = 0;
    index_spwm++;
    xSemaphoreGiveFromISR(timer_sem, &high_task_awoken);
    return (high_task_awoken == pdTRUE); 
}


void app_main()
{
    timer_sem = xSemaphoreCreateBinary();
    if (timer_sem == NULL) 
        printf("Binary semaphore can not be created");

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO1);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO2);
    pwm_config.frequency = 100*freq_spwm;    
    pwm_config.cmpr_a = 40;    		
    pwm_config.cmpr_b = 60;    		
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   
    mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE, 0, 0);  //Enable deadtime on PWM0A and PWM0B with red = (656)*100ns & fed = (67)*100ns on PWM0A and PWM0B generated from PWM0A

    //timer configs ... intervalo, up, if stars in 1 or 0, isr on, reload on 
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = TIMER_AUTORELOAD_EN
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1.0/(freq_spwm*100)*(TIMER_BASE_CLK / TIMER_DIVIDER));
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, timer_group_isr_callback, NULL, 0);
    timer_start(TIMER_GROUP_0, TIMER_0);


    while(1){
        if (xSemaphoreTake(timer_sem, portMAX_DELAY) == pdPASS) 
        {
            pwm_config.cmpr_a = WaveFormTable[index_spwm];   
                mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   


        }
    }
}












 