/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   initialization.h                                  :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/11/09 20:14:49 by username         #+#    #+#              */
/*   Updated: 2026/11/09 20:14:50 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITIALIZATION_H
# define INITIALIZATION_H

# include "main.h"

Dongle	*create_dongle(int dongle_cooldown, int i);
Coder	*create_coder(int id, Dongle *left_dongle, Dongle *right_dongle);

#endif
