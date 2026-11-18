/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 11:44:50 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/15 16:51:25 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "main.h"
#include "parsing.h"
#include "threads.h"
#include "initialization.h"

static int	init_dongles(t_Dongle **dongles, t_Info *data)
{
	int	i;

	i = 0;
	while (i < data->number_coders)
	{
		dongles[i] = create_dongle(data->dongle_cooldown, i);
		if (!dongles[i])
			return (1);
		i++;
	}
	return (0);
}

static int	init_coders(t_Coder **coders, t_Dongle **dongles, t_Info *data)
{
	int	i;

	i = 0;
	while (i < data->number_coders)
	{
		coders[i] = create_coder(i + 1, dongles[i],
				dongles[(i + 1) % data->number_coders]);
		if (!coders[i])
			return (1);
		coders[i]->data = data;
		gettimeofday(&coders[i]->last_compilation, NULL);
		gettimeofday(&coders[i]->time_request, NULL);
		i++;
	}
	return (0);
}

static void	link_coders(t_Coder **coders, int number)
{
	int	i;

	i = 0;
	while (i < number)
	{
		coders[i]->left_coder = coders[(i - 1 + number) % number];
		coders[i]->right_coder = coders[(i + 1) % number];
		i++;
	}
}

static void	clean_up(t_Coder **coders, t_Dongle **dongles, int number)
{
	int	i;
	int	i_dongle;

	i = 0;
	i_dongle = 0;
	while (i < number)
	{
		pthread_mutex_destroy(&coders[i]->compilation_mutex);
		pthread_mutex_destroy(&coders[i]->is_waiting_mutex);
		pthread_mutex_destroy(&coders[i]->last_compilation_mutex);
		free(coders[i]);
		i++;
	}
	free(coders);
	while (i_dongle < number)
	{
		pthread_mutex_destroy(&dongles[i_dongle]->is_free_mutex);
		free(dongles[i_dongle]);
		i_dongle++;
	}
	free(dongles);
}

int	main(int argc, char **argv)
{
	t_Info		*data;
	t_Coder		**coders;
	t_Dongle	**dongles;

	if (argc != 9)
		return (printf("[Error]: Not enough arguments.\n"), 1);
	data = parser(argv);
	if (!data)
		return (0);
	pthread_mutex_init(&data->has_burnout_mutex, NULL);
	pthread_mutex_init(&data->printf_mutex, NULL);
	coders = malloc(sizeof(t_Coder *) * data->number_coders);
	dongles = malloc(sizeof(t_Dongle *) * data->number_coders);
	if (!coders || !dongles)
		return (free(coders), free(dongles), free(data), 1);
	if (init_dongles(dongles, data) || init_coders(coders,
			dongles, data))
		return (free(coders), free(dongles), free(data), 1);
	link_coders(coders, data->number_coders);
	run_threads(coders, data);
	clean_up(coders, dongles, data->number_coders);
	pthread_mutex_destroy(&data->has_burnout_mutex);
	pthread_mutex_destroy(&data->printf_mutex);
	free(data);
	return (0);
}
