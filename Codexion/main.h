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
} Info;

typedef struct {
    struct timeval last_use;
    int is_free;
} Dongle;

typedef struct {
    int id;
    Dongle *left_dongle;
    Dongle *right_dongle;
    int number_of_compilation;
} Coder;

#endif