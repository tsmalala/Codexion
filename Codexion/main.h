/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:17:57 by tsmalala          #+#    #+#             */
/*   Updated: 2026/11/15 16:50:36 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <sys/time.h>
# include <pthread.h>

typedef struct Info
{
	int				number_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles;
	int				dongle_cooldown;
	int				has_burnout;
	char			*scheduler;
	struct timeval	program_start;
	pthread_mutex_t	printf_mutex;
	pthread_mutex_t	has_burnout_mutex;
}	t_Info;

typedef struct Dongle
{
	int				id;
	struct timeval	last_use;
	pthread_mutex_t	is_free_mutex;
}	t_Dongle;

typedef struct Coder
{
	int				id;
	t_Dongle		*left_dongle;
	t_Dongle		*right_dongle;
	t_Info			*data;
	pthread_t		pthread;
	int				number_of_compilation;
	pthread_mutex_t	compilation_mutex;
	int				is_waiting;
	pthread_mutex_t	is_waiting_mutex;
	struct timeval	last_compilation;
	pthread_mutex_t	last_compilation_mutex;
	struct timeval	time_request;
	struct Coder	*left_coder;
	struct Coder	*right_coder;
}	t_Coder;

#endif
