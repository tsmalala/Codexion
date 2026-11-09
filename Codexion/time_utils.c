#include <sys/time.h>
#include <stdio.h>
#include "main.h"

long long get_time(struct timeval start)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    long long diff_ms =
        (time.tv_sec - start.tv_sec) * 1000LL +
        (time.tv_usec - start.tv_usec) / 1000LL;
    
    return diff_ms;
}

void    log_message(int coder_id, long long program_start, int is_burnout, char *message)
{
    if (is_burnout == 1)
        return ;
    else
        printf(message, program_start, coder_id);
}

int    dongle_cooldown(Dongle *first, Dongle *second, int dongle_cooldown)
{
    if (get_time(first->last_use) < dongle_cooldown || get_time(second->last_use) < dongle_cooldown)
        return 1;
    return 0;
}