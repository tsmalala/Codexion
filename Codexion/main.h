#ifndef MAIN_H
#define MAIN_H

typedef struct {
    int number_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles;
    int dongle_cooldown;
    char *scheduler;
} Info;

typedef struct {
    int id;
    int last_use;
    int is_free;
} Dongle;

typedef struct {
    int id;
    int dongle_gauche;
    int dongle_droite;
    int nomber_of_compilation;
} Coder;

#endif