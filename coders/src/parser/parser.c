/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:25:31 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:24:34 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

static int	parse_numbers(t_simulation *sim, char **argv)
{
	long long	value;

	if (parse_positive(argv[1], &value, 0))
		return (1);
	sim->number_of_coders = (int)value;
	if (parse_positive(argv[2], &sim->time_to_burnout, 0)
		|| parse_positive(argv[3], &sim->time_to_compile, 1)
		|| parse_positive(argv[4], &sim->time_to_debug, 1)
		|| parse_positive(argv[5], &sim->time_to_refactor, 1))
		return (1);
	if (parse_positive(argv[6], &value, 0))
		return (1);
	sim->compiles_required = (int)value;
	if (parse_positive(argv[7], &sim->dongle_cooldown, 1))
		return (1);
	return (0);
}

int	parse_arguments(t_simulation *sim, int argc, char **argv)
{
	if (argc != 9 || parse_numbers(sim, argv))
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler = FIRST_IN_FIRST_OUT;
	else if (strcmp(argv[8], "edf") == 0)
		sim->scheduler = EARLIEST_DEADLINE_FIRST;
	else
		return (1);
	return (0);
}
