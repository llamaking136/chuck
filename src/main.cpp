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
}



void loop() {
	if (current_state == States::BOOTING)
	{
		change_state(States::IDLE);
	}

	if (current_state == States::IDLE) {
		set_led(false, true, false);
		buzzer_on();
		delay(500);
		buzzer_off();
	}

	if (radio.available())
	{
		radio.read(&incoming_text, sizeof(incoming_text));
	}

	if (strcmp(incoming_text, "countdown10sec"))
	{
		change_state(States::COUNTING_DOWN);
	}

	if (strcmp(incoming_text, "abort")) {
		change_state(States::ABORT);
	}

	if (current_state == States::COUNTING_DOWN) {
		delay(10000);
		change_state(States::IGNITION);
	}
	
	if (current_state == States::IGNITION) {
		digitalWrite(PYRO1, HIGH);
		delay(2000);
		digitalWrite(PYRO1, LOW);
	}

	if (current_state == States::ABORT) {
		sad_outro();
		return;
	}

}
