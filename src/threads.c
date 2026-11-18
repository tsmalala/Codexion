/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:18:37 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/11 11:10:18 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "coder_routine.h"
#include "burnout.h"
#include "main.h"

void	run_threads(t_Coder **coders, t_Info *data)
{
	int			i;
	int			i_join;
	pthread_t	burnout_thread;

	i = 0;
	i_join = 0;
	pthread_create(&burnout_thread, NULL, check_burnout, coders);
	while (i < data->number_coders)
	{
		pthread_create(&coders[i]->pthread, NULL, coder_routine, coders[i]);
		i++;
	}
	pthread_join(burnout_thread, NULL);
	while (i_join < data->number_coders)
	{
		pthread_join(coders[i_join]->pthread, NULL);
		i_join++;
	}
}
