/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 17:28:59 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:29:12 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

static int	init_base_mutexes(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (pthread_mutex_destroy(&sim->stop_lock), 1);
	if (pthread_mutex_init(&sim->start_lock, NULL) != 0)
		return (pthread_mutex_destroy(&sim->print_lock),
			pthread_mutex_destroy(&sim->stop_lock), 1);
	if (pthread_cond_init(&sim->start_cond, NULL) != 0)
		return (pthread_mutex_destroy(&sim->start_lock),
			pthread_mutex_destroy(&sim->print_lock),
			pthread_mutex_destroy(&sim->stop_lock), 1);
	sim->base_mutexes_initialized = 1;
	return (0);
}

static int	init_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (dongle_init(&sim->dongles[i], sim->number_of_coders) != 0)
			return (1);
		sim->dongles_initialized++;
		i++;
	}
	return (0);
}

static int	init_coders(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1)
			% sim->number_of_coders];
		if (pthread_mutex_init(&sim->coders[i].state_lock, NULL) != 0)
			return (1);
		sim->coders_initialized++;
		i++;
	}
	return (0);
}

int	simulation_init(t_simulation *sim, int argc, char **argv)
{
	memset(sim, 0, sizeof(*sim));
	if (parse_arguments(sim, argc, argv) != 0)
		return (1);
	if (init_base_mutexes(sim) != 0)
		return (1);
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->dongles || !sim->coders)
		return (simulation_destroy(sim), 1);
	memset(sim->coders, 0, sizeof(t_coder) * sim->number_of_coders);
	if (init_dongles(sim) != 0 || init_coders(sim) != 0)
		return (simulation_destroy(sim), 1);
	return (0);
}
