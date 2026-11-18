/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 20:14:49 by username          #+#    #+#             */
/*   Updated: 2026/11/11 10:55:47 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITIALIZATION_H
# define INITIALIZATION_H

# include "main.h"

t_Dongle	*create_dongle(int dongle_cooldown, int i);
t_Coder		*create_coder(int id, t_Dongle *left_dongle,
				t_Dongle *right_dongle);

#endif
