#include <Arduino.h>
#include <states.h>

const char *get_str_from_state(States state)
{
    switch (state)
    {
        case States::BOOTING:
            return "States::BOOTING";
            break;
        case States::BOOT_FAILURE:
            return "States::BOOT_FAILURE";
            break;
        case States::IDLE:
            return "States::IDLE";
            break;
        case States::COUNTING_DOWN:
            return "States::COUNTING_DOWN";
            break;
        case States::IGNITION:
            return "States::IGNITION";
            break;
        case States::ABORT:
            return "States::ABORT";
            break;
        default:
            return "States::UNKNOWN_STATE";
            break;
    }
}

void change_state(States state)
{
    current_state = state;
    Serial.print("Changed state to ");
    Serial.println(get_str_from_state(current_state));
}