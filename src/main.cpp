/*
Chuck The Launch Computer Code

Written by JRD Propulsion, llamaking136, and The Tech Kid

Created 19th Feburary, 2023
I can't spell Feburary

	State meanings:
	0: Idle
	1: Countdown
	2: Ignition

*/

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>
#include <pins.h>
#include <outputs.h>
#include <radio.h>
#include <states.h>

States current_state = States::BOOTING;

unsigned long timer_0 = 0;
uint8_t countdown = 10;

char incoming_text[32];

// uint8_t is another word for unsigned char

RF24 radio(RADIO_CS, RADIO_CSN);

void happy_intro()
{
	// red
	set_led(true, false, false);
	buzzer_on();
	delay(500);
	led_off();
	buzzer_off();
	delay(500);

	// green
	set_led(false, true, false);
	buzzer_on();
	delay(500);
	led_off();
	buzzer_off();
	delay(500);

	// blue
	set_led(false, false, true);
	buzzer_on();
	delay(500);
	led_off();
	buzzer_off();
	delay(500);

	// white
	set_led(true, true, true);
	buzzer_on();
	delay(1000);
	led_off();
	buzzer_off();
	delay(500);
}

void sad_outro()
{
	// red
	set_led(true, false, false);
	buzzer_on();
	delay(500);
	led_off();
	buzzer_off();
	delay(500);

	// red
	set_led(true, false, false);
	buzzer_on();
	delay(500);
	led_off();
	buzzer_off();
	delay(500);

	// red
	set_led(true, false, false);
	buzzer_on();
	delay(2000);
	led_off();
	buzzer_off();
}

void setup() {
	Serial.begin(9600);

	Serial.println("Chuck Startup");

	uint8_t is_radio_alive = radio.begin();
	if (!is_radio_alive)
	{
		change_state(States::BOOT_FAILURE);
		Serial.println("No radio detected! Is it plugged in properly?");
		sad_outro();
		for (;;);
	}

	// TODO: radio setup

	happy_intro();
	change_state(States::IDLE);
}



void loop() {
	if (radio.available())
	{
		radio.read(&incoming_text, sizeof(incoming_text));
		if (strcmp(incoming_text, "abort") == 0){
			change_state(States::ABORT);
		}
		if (strcmp(incoming_text, "countdown10sec") == 0 && current_state == States::IDLE){
			timer_0 = millis();
			led_off();
			buzzer_off();
			countdown = 10;
			change_state(States::COUNTING_DOWN);
			Serial.println(countdown);
		}
	}

	switch(current_state){
		case States::ABORT: {
			sad_outro();
			digitalWrite(PYRO1, LOW);
			Serial.println("Launch Aborted");
			while (1);
			break;
		}
		case States::IDLE: {
			set_led(false, true, false);
			if (millis() - timer_0 > 500){
				buzzer_on();
			}
			if (millis() - timer_0 > 1000){
				timer_0 = millis();
				buzzer_off();
			}
			break;
		}
		case States::COUNTING_DOWN: {
			if (millis() - timer_0 > 1000){
				timer_0 = millis();
				countdown -= 1;
				Serial.println(countdown);
				if (countdown == 0){
					change_state(States::IGNITION);
				}
			}
			break;
		}
		case States::IGNITION: {
			if (millis() - timer_0 > 2000){
				digitalWrite(PYRO1, HIGH);
			}else{
				digitalWrite(PYRO1, LOW);
			}
			break;
		}
		default: {
			break;
		}
	}
}
