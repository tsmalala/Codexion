/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:17:38 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/15 16:48:46 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "main.h"
#include "scheduler.h"
#include "time_utils.h"

int	has_burnout(t_Coder *coder)
{
	pthread_mutex_lock(&coder->data->has_burnout_mutex);
	if (coder->data->has_burnout == 1)
	{
		pthread_mutex_unlock(&coder->data->has_burnout_mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->data->has_burnout_mutex);
	return (1);
}

int	wait_and_acquire(t_Coder *coder,
	t_Dongle *first, t_Dongle *second)
{
	pthread_mutex_lock(&coder->is_waiting_mutex);
	coder->is_waiting = 0;
	pthread_mutex_unlock(&coder->is_waiting_mutex);
	gettimeofday(&coder->time_request, NULL);
	while (scheduler(coder) && !coder->data->has_burnout)
	{
	}
	pthread_mutex_lock(&first->is_free_mutex);
	pthread_mutex_lock(&second->is_free_mutex);
	first->is_free = 0;
	second->is_free = 0;
	pthread_mutex_lock(&coder->is_waiting_mutex);
	coder->is_waiting = 1;
	pthread_mutex_unlock(&coder->is_waiting_mutex);
	return (1);
}

void	run_cycle(t_Coder *coder,
	t_Dongle *first, t_Dongle *second)
{
	while (dongle_cooldown(first, second, coder->data->dongle_cooldown
		) && !coder->data->has_burnout)
	{
	}
	log_message(coder->id, get_time(coder->data->program_start
			), coder->data->has_burnout, "%lld %d has taken a dongle\n");
	log_message(coder->id, get_time(coder->data->program_start
			), coder->data->has_burnout, "%lld %d has taken a dongle\n");
	gettimeofday(&coder->last_compilation, NULL);
	log_message(coder->id, get_time(coder->data->program_start
			), coder->data->has_burnout, "%lld %d is compiling\n");
	usleep(coder->data->time_to_compile * 1000);
	gettimeofday(&coder->left_dongle->last_use, NULL);
	gettimeofday(&coder->right_dongle->last_use, NULL);
	first->is_free = 1;
	second->is_free = 1;
	pthread_mutex_unlock(&second->is_free_mutex);
	pthread_mutex_unlock(&first->is_free_mutex);
	log_message(coder->id, get_time(coder->data->program_start
			), coder->data->has_burnout, "%lld %d is debugging\n");
	usleep(coder->data->time_to_debug * 1000);
	log_message(coder->id, get_time(coder->data->program_start
			), coder->data->has_burnout, "%lld %d is refactoring\n");
	usleep(coder->data->time_to_refactor * 1000);
	coder->number_of_compilation++;
}

void	combined_function(t_Coder *coder, t_Dongle *first, t_Dongle *second)
{
	if (!wait_and_acquire(coder, first, second))
		return ;
	run_cycle(coder, first, second);
}

void	*coder_routine(void *arg)
{
	t_Coder		*coder;
	t_Dongle	*first;
	t_Dongle	*second;

	coder = (t_Coder *) arg;
	while (coder->number_of_compilation
		< coder->data->number_of_compiles)
	{
		if (!has_burnout(coder))
			return (NULL);
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
		if (first->id == second->id)
			return (NULL);
		combined_function(coder, first, second);
	}
	return (NULL);
}
