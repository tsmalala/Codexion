/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.h                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: tsmalala <tsmalala@student.42antananarivo.#+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/11/09 20:17:57 by tsmalala         #+#    #+#              */
/*   Updated: 2026/11/09 20:17:58 by tsmalala        ###   ########.fr        */
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
	char			*scheduler;
	struct timeval	program_start;
	int				has_burnout;
}	Info;

typedef struct Dongle
{
	int				id;
	struct timeval	last_use;
	int				is_free;
	pthread_mutex_t	is_free_mutex;
}	Dongle;

typedef struct Coder
{
	int				id;
	Dongle			*left_dongle;
	Dongle			*right_dongle;
	Info			*data;
	pthread_t		pthread;
	int				number_of_compilation;
	int				is_compiling;
	struct timeval	last_compilation;
	struct timeval	time_request;
	struct Coder	*left_coder;
	struct Coder	*right_coder;
}	Coder;

#endif
