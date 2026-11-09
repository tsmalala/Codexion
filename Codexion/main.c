/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 11:44:50 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/09 19:16:05 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "main.h"
#include "parsing.h"
#include "threads.h"
#include "initialization.h"

static int	init_dongles(Dongle **dongles, Info *data)
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

static int	init_coders(Coder **coders, Dongle **dongles, Info *data)
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

static void	link_coders(Coder **coders, int number)
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


static void	clean_up(Coder **coders, Dongle **dongles, int number)
{
	int	i;
    int i_dongle;

	i = 0;
    i_dongle = 0;
	while (i < number)
	{
		free(coders[i]);
		i++;
	}
	free(coders);

    while (i < number)
	{
		pthread_mutex_destroy(&dongles[i]->is_free_mutex);
		free(dongles[i]);
		i_dongle++;
	}
	free(dongles);
}

int	main(int argc, char **argv)
{
	Info	*data;
	Coder	**coders;
	Dongle	**dongles;

	if (argc != 9)
		return (printf("[Error]: Not enough arguments.\n"), 1);
	data = parser(argv);
	if (!data)
		return (1);
	coders = malloc(sizeof(Coder *) * data->number_coders);
	dongles = malloc(sizeof(Dongle *) * data->number_coders);
	if (!coders || !dongles)
		return (free(coders), free(dongles), free(data), 1);
	if (init_dongles(dongles, data) || init_coders(coders,
			dongles, data))
		return (1);
	link_coders(coders, data->number_coders);
	run_threads(coders, data);
	clean_up(coders, dongles, data->number_coders);
	free(data);
	return (0);
}
