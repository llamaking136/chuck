#pragma once

enum States
{
	BOOTING = 0,
	BOOT_FAILURE,
	IDLE,
	COUNTING_DOWN,
	IGNITION,
	ABORT
};

extern States current_state;

const char *get_str_from_state(States);
void change_state(States);