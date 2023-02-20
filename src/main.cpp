/*
Chuck The Launch Computer Code

Written by JRD Propulsion, llamaking136, and The Tech Kid

Created 19th Feburary, 2023
I can't spell Feburary
*/

#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdint.h>
#include <pins.h>
#include <outputs.h>

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
		Serial.println("No radio detected! Is it plugged in properly?");
		sad_outro();
		for (;;);
	}
	
	happy_intro();
}


void loop() {

}
