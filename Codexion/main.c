/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsmalala <tsmalala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 11:44:50 by tsmalala          #+#    #+#             */
/*   Updated: 2026/10/13 11:44:51 by tsmalala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "parsing.h"
#include "main.h"

int main(int argc, char *argv[])
{
    Info *data;

    if (argc != 9) {
        printf("[Error]: Not enough arguments.\n");
        return 1;
    }
    data = parser(argv);
    if (data == NULL)
        return 1;

    free(data);
    return 0;
}
