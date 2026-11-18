/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:18:29 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/15 16:53:38 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "time_utils.h"

int	fifo(t_Coder *coder, t_Coder *neighbor)
{
	long long	coder_requested;
	long long	neighbor_requested;

	pthread_mutex_lock(&neighbor->is_waiting_mutex);
	if (neighbor->is_waiting)
	{
		pthread_mutex_unlock(&neighbor->is_waiting_mutex);
		return (1);
	}
	pthread_mutex_unlock(&neighbor->is_waiting_mutex);
	coder_requested = get_time(coder->time_request);
	neighbor_requested = get_time(neighbor->time_request);
	return (coder_requested >= neighbor_requested);
}

long long	get_deadline(t_Coder *coder)
{
	long long	last_compilation;

	pthread_mutex_lock(&coder->last_compilation_mutex);
	last_compilation = get_time(coder->data->program_start
			) - get_time(coder->last_compilation);
	pthread_mutex_unlock(&coder->last_compilation_mutex);
	return (last_compilation + coder->data->time_to_burnout);
}

int	edf(t_Coder *coder, t_Coder *neighbor)
{
	pthread_mutex_lock(&neighbor->is_waiting_mutex);
	if (neighbor->is_waiting)
	{
		pthread_mutex_unlock(&neighbor->is_waiting_mutex);
		return (1);
	}
	pthread_mutex_unlock(&neighbor->is_waiting_mutex);
	return (get_deadline(coder) <= get_deadline(neighbor));
}

int	scheduler(t_Coder *coder)
{
	pthread_mutex_lock(&coder->data->has_burnout_mutex);
	if (coder->data->has_burnout)
	{
		pthread_mutex_unlock(&coder->data->has_burnout_mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->data->has_burnout_mutex);
	if (strcmp(coder->data->scheduler, "fifo") == 0)
	{
		if (fifo(coder, coder->left_coder)
			&& fifo(coder, coder->right_coder))
			return (0);
		return (1);
	}
	if (strcmp(coder->data->scheduler, "edf") == 0)
	{
		if (edf(coder, coder->left_coder) && edf(coder, coder->right_coder))
			return (0);
		return (1);
	}
	return (0);
}
