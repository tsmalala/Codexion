/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:14:40 by username          #+#    #+#             */
/*   Updated: 2026/11/11 10:58:02 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "main.h"

t_Dongle	*create_dongle(int dongle_cooldown, int i)
{
	t_Dongle			*dongle;
	struct timeval		last_use;

	gettimeofday(&last_use, NULL);
	last_use.tv_usec -= (dongle_cooldown % 1000) * 1000;
	last_use.tv_sec -= dongle_cooldown / 1000;
	if (last_use.tv_usec < 0)
	{
		last_use.tv_usec += 1000000;
		last_use.tv_sec--;
	}
	dongle = (t_Dongle *) malloc(sizeof(t_Dongle));
	dongle->last_use = last_use;
	dongle->id = i + 1;
	dongle->is_free = 1;
	pthread_mutex_init(&dongle->is_free_mutex, NULL);
	return (dongle);
}

t_Coder	*create_coder(int id, t_Dongle *left_dongle, t_Dongle *right_dongle)
{
	t_Coder	*coder;

	coder = (t_Coder *) malloc(sizeof(t_Coder));
	coder->id = id;
	coder->left_dongle = left_dongle;
	coder->right_dongle = right_dongle;
	coder->number_of_compilation = 0;
	return (coder);
}
