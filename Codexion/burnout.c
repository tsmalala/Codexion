/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burnout.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:17:22 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/15 16:54:21 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "main.h"
#include "time_utils.h"

int	is_codexion_finished(t_Coder **coders)
{
	int	i;
	int	number_of_compiles;
	int	number_of_coder;

	i = 0;
	number_of_compiles = coders[0]->data->number_of_compiles;
	number_of_coder = coders[0]->data->number_coders;
	while (i < number_of_coder)
	{
		pthread_mutex_lock(&coders[i]->compilation_mutex);
		if (coders[i]->number_of_compilation < number_of_compiles)
		{
			pthread_mutex_unlock(&coders[i]->compilation_mutex);
			return (0);
		}
		pthread_mutex_unlock(&coders[i]->compilation_mutex);
		i++;
	}
	return (1);
}

int	check_time(t_Coder *coder, t_Coder *coder_ref)
{
	pthread_mutex_lock(&coder->last_compilation_mutex);
	if (get_time(coder->last_compilation
		) >= coder_ref->data->time_to_burnout)
	{
		pthread_mutex_unlock(&coder->last_compilation_mutex);
		log_message(coder, get_time(
				coder_ref->data->program_start),
			coder->data->has_burnout, "%lld %d burned out\n");
		pthread_mutex_lock(&coder->data->has_burnout_mutex);
		coder->data->has_burnout = 1;
		pthread_mutex_unlock(&coder->data->has_burnout_mutex);
		return (0);
	}
	pthread_mutex_unlock(&coder->last_compilation_mutex);
	return (1);
}

void	*check_burnout(void *arg)
{
	int		i;
	int		number_of_compiles;
	t_Coder	**coders;

	coders = (t_Coder **) arg;
	number_of_compiles = coders[0]->data->number_of_compiles;
	while (!is_codexion_finished(coders))
	{
		i = 0;
		while (i < coders[0]->data->number_coders)
		{
			pthread_mutex_lock(&coders[i]->compilation_mutex);
			if (coders[i]->number_of_compilation < number_of_compiles)
			{
				pthread_mutex_unlock(&coders[i]->compilation_mutex);
				if (!check_time(coders[i], coders[0]))
					return (NULL);
			}
			else
				pthread_mutex_unlock(&coders[i]->compilation_mutex);
			i++;
			usleep(10);
		}
	}
	return (NULL);
}
