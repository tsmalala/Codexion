/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:14:53 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/11 11:08:53 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stdio.h>
#include "main.h"

long long	get_time(struct timeval start)
{
	struct timeval	time;
	long long		diff_ms;

	gettimeofday(&time, NULL);
	diff_ms = (time.tv_sec - start.tv_sec) * 1000LL + (
			time.tv_usec - start.tv_usec) / 1000LL;
	return (diff_ms);
}

void	log_message(int coder_id, long long program_start,
						int is_burnout, char *message)
{
	if (is_burnout == 1)
		return ;
	else
		printf(message, program_start, coder_id);
}

int	dongle_cooldown(t_Dongle *first, t_Dongle *second, int dongle_cooldown)
{
	if ((get_time(first->last_use) < dongle_cooldown) || (
			get_time(second->last_use) < dongle_cooldown))
		return (1);
	return (0);
}
