/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 11:44:50 by tsmalala          #+#    #+#             */
/*   Updated: 2026/10/22 10:49:19 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "parsing.h"
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

    pthread_mutex_init(&dongle->is_free_mutex, NULL);

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

void *coder_routine(void *arg)
{
    Coder   *coder;
    Dongle  *first;
    Dongle  *second;

    coder = (Coder *)arg;

    while (coder->number_of_compilation
        < coder->data->number_of_compiles)
    {
        if (coder->left_dongle->id < coder->right_dongle->id)
        {
            first = coder->left_dongle;
            second = coder->right_dongle;
        }
        else
        {
            first = coder->right_dongle;
            second = coder->left_dongle;
        }

        pthread_mutex_lock(&first->is_free_mutex);
        pthread_mutex_lock(&second->is_free_mutex);

        printf("%lld %d has taken a dongle\n",
            get_time(coder->data->program_start), coder->id);
        printf("%lld %d has taken a dongle\n",
            get_time(coder->data->program_start), coder->id);

        printf("%lld %d is compiling\n",
            get_time(coder->data->program_start), coder->id);

        usleep(coder->data->time_to_compile * 1000);

        gettimeofday(&coder->left_dongle->last_use, NULL);
        gettimeofday(&coder->right_dongle->last_use, NULL);

        pthread_mutex_unlock(&second->is_free_mutex);
        pthread_mutex_unlock(&first->is_free_mutex);

        printf("%lld %d is debugging\n",
            get_time(coder->data->program_start), coder->id);

        usleep(coder->data->time_to_refactor * 1000);

        printf("%lld %d is refactoring\n",
            get_time(coder->data->program_start), coder->id);

        coder->number_of_compilation++;
    }
    return NULL;
}

void    run_threads(Coder **coders, Info *data)
{
    int i = 0;
    int i_join = 0;

    while (i < data->number_coders)
    {
        pthread_create(&coders[i]->pthread, NULL, coder_routine, coders[i]);
        i++;
    }

    while (i_join < data->number_coders)
    {
        pthread_join(coders[i_join]->pthread, NULL);
        i_join++;
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
        dongles[i_dongle] = create_dongle(data->dongle_cooldown, i_dongle);
        i_dongle++;
    }
    while (i < data->number_coders)
    {
        coders[i] = create_coder(i + 1, dongles[i], dongles[(i + 1) % data->number_coders]);
        coders[i]->data = data;
        i++;
    }

    // coder_routine(coders[0]);
    run_threads(coders, data);

    i_dongle = 0;
    while (i_dongle < data->number_coders)
    {
        free(dongles[i_dongle]);
        i_dongle++;
    }
    free(dongles);
    i = 0;
    while (i < data->number_coders)
    {
        free(coders[i]);
        i++;
    }
    free(coders);
    free(data);
    return 0;
}
