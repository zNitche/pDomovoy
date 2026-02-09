#include "../includes/globals.h"

bool alarm_triggered = false;
bool alarm_in_standby = false;

queue_t core0_events_queue;
queue_t core1_events_queue;
