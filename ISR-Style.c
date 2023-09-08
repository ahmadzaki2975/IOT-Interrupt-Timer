#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "rom/gpio.h"

// Deklarasi variabel untuk menyimpan waktu mulai dan selesai
uint64_t timer_start_time;
uint64_t timer_end_time;

// Fungsi yang akan dipanggil ketika timer interrupt terjadi
void IRAM_ATTR timer_callback(void *arg)
{
  static bool ON;
  ON = !ON;

  // Mendapatkan waktu mulai interrupt
  timer_start_time = esp_timer_get_time();

  for (int i = 0; i < 1000000; i++)
  {
    // Delay untuk memperlambat interrupt, supaya latency lebih terlihat
  }

  // Set GPIO 2 ke nilai ON
  gpio_set_level(GPIO_NUM_2, ON);

  // Mendapatkan waktu selesai interrupt
  timer_end_time = esp_timer_get_time();
  uint64_t interrupt_latency = timer_end_time - timer_start_time;

  // Menampilkan latency interrupt
  printf("Interrupt Latency - ISR: %llu microseconds\n", interrupt_latency);
}

void app_main(void)
{
  gpio_pad_select_gpio(GPIO_NUM_2);
  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_2, 0);

  // Konfigurasi timer
  esp_timer_create_args_t timer_config = {
      .callback = &timer_callback,
      .name = "my_timer"};

  // Membuat timer
  esp_timer_handle_t timer;
  esp_timer_create(&timer_config, &timer);
  esp_timer_start_periodic(timer, 1000000); // interval 1 s

  while (1)
  {
    vTaskDelay(portMAX_DELAY);
  }
}
