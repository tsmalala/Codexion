/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:18:29 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/11 11:09:47 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "time_utils.h"

int	fifo(t_Coder *coder, t_Coder *neighbor)
{
	if (neighbor->is_compiling)
		return (1);
	return (get_time(coder->time_request) >= get_time(neighbor->time_request));
}

long long	get_deadline(t_Coder *coder)
{
	return (get_time(coder->last_compilation) + coder->data->time_to_burnout);
}

int	edf(t_Coder *coder, t_Coder *neighbor)
{
	if (neighbor->is_compiling)
		return (1);
	return (get_deadline(coder) <= get_deadline(neighbor));
}

int	scheduler(t_Coder *coder)
{
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
