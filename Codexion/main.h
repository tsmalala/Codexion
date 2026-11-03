#ifndef MAIN_H
#define MAIN_H

#include <sys/time.h>


typedef struct {
    int number_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles;
    int dongle_cooldown;
    char *scheduler;
    struct timeval program_start;
    int has_burnout;
} Info;

typedef struct {
    struct timeval last_use;
    pthread_mutex_t is_free_mutex;
    int id;
} Dongle;

typedef struct {
    int id;
    Dongle *left_dongle;
    Dongle *right_dongle;
    Info *data;
    pthread_t pthread;
    int number_of_compilation;
    struct timeval last_compilation;
} Coder;

#endif