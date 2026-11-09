#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "main.h"

Dongle  *create_dongle(int dongle_cooldown, int i);
Coder   *create_coder(int id, Dongle *left_dongle, Dongle *right_dongle);

#endif