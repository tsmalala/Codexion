/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:18:49 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/11 11:11:34 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_UTILS_H
# define TIME_UTILS_H

# include "main.h"
# include <sys/time.h>

long long	get_time(struct timeval start);
void		log_message(int coder_id, long long program_start,
				int is_burnout, char *message);
int			dongle_cooldown(t_Dongle *first, t_Dongle *second,
				int dongle_cooldown);

#endif
