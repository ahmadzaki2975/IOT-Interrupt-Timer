#include <stdio.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "rom/gpio.h"

// Deklarasi variabel untuk menyimpan waktu mulai dan selesai
uint64_t start_time;
uint64_t end_time;

void timer_interrupt(void *param)
{
    static bool ON;
    ON = !ON;

    // Mendapatkan waktu mulai interrupt
    start_time = esp_timer_get_time();

    for (int i = 0; i < 1000000; i++)
    {
        // Delay untuk memperlambat interrupt, supaya latency lebih terlihat
    }

    gpio_set_level(GPIO_NUM_2, ON);

    // Mendapatkan waktu selesai interrupt
    end_time = esp_timer_get_time();
}

void app_main(void)
{
    // Konfigurasi GPIO
    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    // Konfigurasi timer
    const esp_timer_create_args_t my_timer_args = {
        .callback = &timer_interrupt,
        .name = "My Timer"};

    // Membuat timer
    esp_timer_handle_t timer_handler;

    // Memulai timer
    ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, 1000000)); // 1000000 us = 1 s

    while (true)
    {
        // Menampilkan latency interrupt
        uint64_t interrupt_latency_us = end_time - start_time;
        printf("Interrupt Latency - Callback: %llu microseconds\n", interrupt_latency_us);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
