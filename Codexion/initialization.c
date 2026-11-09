#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "main.h"

Dongle *create_dongle(int dongle_cooldown, int i)
{
    Dongle  *dongle;
    struct timeval last_use;
    
    gettimeofday(&last_use, NULL);

    last_use.tv_usec -= (dongle_cooldown % 1000) * 1000;
    last_use.tv_sec  -= dongle_cooldown / 1000;

    if (last_use.tv_usec < 0) {
        last_use.tv_usec += 1000000;
        last_use.tv_sec--;
    }

    dongle = (Dongle *)malloc(sizeof(Dongle));
    dongle->last_use = last_use;
    dongle->id = i + 1;
    dongle->is_free = 1;

    pthread_mutex_init(&dongle->is_free_mutex, NULL);

    return dongle;
}

Coder *create_coder(int id, Dongle *left_dongle, Dongle *right_dongle)
{
    Coder *coder;

    coder = (Coder *)malloc(sizeof(Coder));
    coder->id = id;
    coder->left_dongle = left_dongle;
    coder->right_dongle = right_dongle;
    coder->number_of_compilation = 0;

    return coder;
}