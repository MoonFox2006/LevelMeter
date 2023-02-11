#include <driver/touch_pad.h>
#include <Arduino.h>

constexpr touch_pad_t TOUCH_NUM = TOUCH_PAD_NUM5; // GPIO12
constexpr uint8_t GND_PIN = 14;

static void halt(const char *msg) {
  Serial.println(msg);
  Serial.flush();
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);

  pinMode(GND_PIN, OUTPUT);
  digitalWrite(GND_PIN, LOW);

  ESP_ERROR_CHECK(touch_pad_init());
  ESP_ERROR_CHECK(touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V));
  ESP_ERROR_CHECK(touch_pad_config(TOUCH_NUM, 0));
  ESP_ERROR_CHECK(touch_pad_filter_start(10));
}

void loop() {
  static uint16_t raw_min = 65535, raw_max = 0, level_min = 65535, level_max = 0;

  uint16_t raw, level;

  ESP_ERROR_CHECK(touch_pad_read_raw_data(TOUCH_NUM, &raw));
  ESP_ERROR_CHECK(touch_pad_read_filtered(TOUCH_NUM, &level));
  if (raw < raw_min)
    raw_min = raw;
  if (raw > raw_max)
    raw_max = raw;
  if (level < level_min)
    level_min = level;
  if (level > level_max)
    level_max = level;
  Serial.printf("\r%-3u [%-3u-%-3u] %-3u [%-3u-%-3u]", raw, raw_min, raw_max, level, level_min, level_max);
  delay(100);
}
