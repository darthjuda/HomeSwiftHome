/*
 * temperature_sensor.ino
 *
 *  Created on: 2020-10-29
 *      Author: darthjuda (Mehdi Jung)
 *
 * Note:
 *
 * Apple's HAP doc 
 * https://developer.apple.com/support/homekit-accessory-protocol/
 *
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include "DHTesp.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

DHTesp dht;

float temperature;
float humidity;
int fault;
bool isActive;


void setup() {
	Serial.begin(115200);
	wifi_connect(); // in wifi_info.h
  //homekit_storage_reset();
  dht.setup(5, DHTesp::DHT11); // Connect DHT sensor to GPIO 05 (D1)
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_temperature;
extern "C" homekit_characteristic_t cha_humidity;
extern "C" homekit_characteristic_t cha_status_fault;
extern "C" homekit_characteristic_t cha_status_active;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
	arduino_homekit_setup(&config);
}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_report_millis) {
		// report sensor values every 10 seconds
		next_report_millis = t + 10 * 1000;
		my_homekit_report();
	}
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}

void my_homekit_report() {
  
  temperature = dht.getTemperature();
  humidity = dht.getHumidity();

	cha_current_temperature.value.float_value = temperature;
	homekit_characteristic_notify(&cha_current_temperature, cha_current_temperature.value);

	cha_current_temperature.value.float_value = humidity;
	homekit_characteristic_notify(&cha_humidity, cha_humidity.value);

  cha_status_active.value.bool_value = isActive;
  homekit_characteristic_notify(&cha_status_active, cha_status_active.value);

	cha_status_fault.value.int_value = fault;
	homekit_characteristic_notify(&cha_status_fault, cha_status_fault.value);

	LOG_D("Current temperature: %.1f", temperature);
	LOG_D("Current humidity: %.0f", humidity);
	LOG_D("Sensor status: %b, Exit Code: %u", isActive, fault);

}

void handle_fault() {
  if (dht.getStatusString() == "OK") {
    fault = 0;
    isActive = true;
  }
  else {
    fault = 1;
    isActive = false;
  }
}
