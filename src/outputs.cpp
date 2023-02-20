#include <Arduino.h>
#include <pins.h>

void set_led(uint8_t red, uint8_t green, uint8_t blue)
{
	// we invert the color because the LED is common anode
	// and since it's common anode, we pull low to turn an LED
	// on and pull high to turn off an LED
	
	digitalWrite(LEDRED, !red);
	digitalWrite(LEDGRN, !green);
	digitalWrite(LEDBLU, !blue);
}

void led_off()
{
	// yeah we have two functions to do the same thing
	// oh well
	// some of us are lazy
    // lol

	digitalWrite(LEDRED, HIGH);
	digitalWrite(LEDGRN, HIGH);
	digitalWrite(LEDBLU, HIGH);
}

void buzzer_on()
{
	digitalWrite(BUZZER, HIGH);
}

void buzzer_off()
{
	digitalWrite(BUZZER, LOW);
}
