#include <stdio.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "rom/gpio.h"
// Define variables to store timestamps
uint64_t isr_start_time;
uint64_t isr_end_time;

uint64_t expected_interval = 1000000; // 1 second in microseconds
uint64_t prev_interrupt_time = 0;

void timer_interrupt(void *param)
{
    static bool ON;
    ON = !ON;

    // Capture timestamp at the beginning of the ISR
    isr_start_time = esp_timer_get_time();

    for (int i = 0; i < 1000000; i++)
    {
        // Intentional delay
    }

    gpio_set_level(GPIO_NUM_2, ON);

    // Capture timestamp at the end of the ISR
    isr_end_time = esp_timer_get_time();
}

void app_main(void)
{
    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    const esp_timer_create_args_t my_timer_args = {
        .callback = &timer_interrupt,
        .name = "My Timer"};

    esp_timer_handle_t timer_handler;

    ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, 1000000)); // 1000000 us = 1 s

    while (true)
    {
        // Calculate and print interrupt latency
        uint64_t interrupt_latency_us = isr_end_time - isr_start_time;
        printf("Interrupt Latency - Callback: %llu microseconds\n", interrupt_latency_us);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
