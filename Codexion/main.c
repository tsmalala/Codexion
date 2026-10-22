/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 11:44:50 by tsmalala          #+#    #+#             */
/*   Updated: 2026/10/22 10:04:46 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "parsing.h"
#include "main.h"

Dongle *create_dongle(int id,int dongle_cooldown)
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
    dongle->is_free = 1;

    return dongle;
}

Coder *create_coder(int id, Dongle *left_dognle, Dongle *right_dongle)
{
    Coder *coder;

    coder = (Coder *)malloc(sizeof(Coder));
    coder->id = id;
    coder->left_dongle = left_dognle;
    coder->right_dongle = right_dongle;
    coder->number_of_compilation = 0;

    return coder;
}

long long get_time(struct timeval start)
{
    struct timeval time;
    
    gettimeofday(&time, NULL);
    long long diff_ms =
        (time.tv_sec - start.tv_sec) * 1000LL +
        (time.tv_usec - start.tv_usec) / 1000LL;
    
    return diff_ms;
}

void    coder_routine(Coder *coder, Info *data)
{
    while (coder->number_of_compilation < data->number_of_compiles)
    {
            if (coder->left_dongle->is_free)
            {
                coder->left_dongle->is_free = 0;
                printf("%lld %d has taken a dongle\n", get_time(data->program_start), coder->id);
            }
            if (coder->right_dongle->is_free)
            {
                coder->right_dongle->is_free = 0;
                printf("%lld %d has taken a dongle\n", get_time(data->program_start), coder->id);
            }
            printf("%lld %d is compiling\n", get_time(data->program_start), coder->id);
            usleep(data->time_to_compile * 1000);
            gettimeofday(&coder->left_dongle->last_use, NULL);
            gettimeofday(&coder->right_dongle->last_use, NULL);
            coder->left_dongle->is_free = 1;
            coder->right_dongle->is_free = 1;
            printf("%lld %d is debugging\n", get_time(data->program_start), coder->id);
            usleep(data->time_to_refactor * 1000);
            printf("%lld %d is refactoring\n", get_time(data->program_start), coder->id);
            coder->number_of_compilation++;
    }
}

int main(int argc, char *argv[])
{
    Info *data;
    Coder **coders;
    Dongle **dongles;
    int i = 0;
    int i_dongle = 0;

    if (argc != 9) {
        printf("[Error]: Not enough arguments.\n");
        return 1;
    }
    data = parser(argv);
    if (data == NULL)
        return 1;

    coders = (Coder **)malloc(sizeof(Coder *) * data->number_coders);
    dongles = (Dongle **)malloc(sizeof(Dongle *) * data->number_coders);
    while (i_dongle < data->number_coders)
    {
        dongles[i_dongle] = create_dongle(i_dongle + 1, data->dongle_cooldown);
        i_dongle++;
    }
    while (i < data->number_coders)
    {
        coders[i] = create_coder(i + 1, dongles[i], dongles[(i + 1) % data->number_coders]);
        i++;
    }

    coder_routine(coders[0], data);

    i_dongle = 0;
    while (i_dongle < data->number_coders)
    {
        free(dongles[i]);
    }
    free(dongles);
    i = 0;
    while (i < data->number_coders)
    {
        free(coders[i]);
    }
    free(coders);
    free(data);
    return 0;
}
