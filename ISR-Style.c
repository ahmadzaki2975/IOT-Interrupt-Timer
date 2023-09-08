#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "rom/gpio.h"

// Define variables to store timestamps
uint64_t timer_start_time;
uint64_t timer_end_time;

// Function pointer for the IRAM routine
void IRAM_ATTR timer_callback(void *arg)
{
  static bool ON;
  ON = !ON;

  // Capture timestamp at the beginning of the timer callback
  timer_start_time = esp_timer_get_time();

  for (int i = 0; i < 1000000; i++)
  {
    // Intentional delay
  }

  // Toggle the LED state
  gpio_set_level(GPIO_NUM_2, ON);

  // Capture timestamp at the end of the timer callback
  timer_end_time = esp_timer_get_time();
  uint64_t interrupt_latency = timer_end_time - timer_start_time;
  printf("Interrupt Latency - ISR: %llu microseconds\n", interrupt_latency);
}

void app_main(void)
{
  gpio_pad_select_gpio(GPIO_NUM_2);
  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_2, 0);

  // Define the timer configuration
  esp_timer_create_args_t timer_config = {
      .callback = &timer_callback,
      .name = "my_timer"};

  // Create and start the timer
  esp_timer_handle_t timer;
  esp_timer_create(&timer_config, &timer);
  esp_timer_start_periodic(timer, 1000000); // 1 second interval

  while (1)
  {
    vTaskDelay(portMAX_DELAY);
  }
}
