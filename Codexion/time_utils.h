#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "main.h"
#include <sys/time.h>

long long get_time(struct timeval start);
void    log_message(int coder_id, long long program_start, int is_burnout, char *message);
int    dongle_cooldown(Dongle *first, Dongle *second, int dongle_cooldown);

#endif