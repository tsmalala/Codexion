/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   burnout.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:17:22 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/11 11:05:44 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "main.h"
#include "time_utils.h"

int	is_codexion_finished(t_Coder **coders)
{
	int	i;
	int	number_of_compiles;

	i = 0;
	number_of_compiles = coders[0]->data->number_of_compiles;
	while (i < coders[0]->data->number_coders)
	{
		if (coders[i]->number_of_compilation != number_of_compiles)
			return (0);
		i++;
	}
	return (1);
}

void	*check_burnout(void *arg)
{
	int		i;
	t_Coder	**coders;

	coders = (t_Coder **) arg;
	while (!is_codexion_finished(coders))
	{
		i = 0;
		while (i < coders[0]->data->number_coders)
		{
			if (get_time(coders[i]->last_compilation
				) >= coders[i]->data->time_to_burnout)
			{
				log_message(coders[i]->id, get_time(
						coders[i]->data->program_start),
					coders[i]->data->has_burnout, "%lld %d burned out\n");
				coders[i]->data->has_burnout = 1;
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}
