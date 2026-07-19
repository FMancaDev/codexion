/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:20:18 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:27:08 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

static void	release_start(t_simulation *sim)
{
	int	i;

	pthread_mutex_lock(&sim->start_lock);
	sim->start_time = get_time_ms();
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_lock);
		sim->coders[i].last_compile_start = sim->start_time;
		pthread_mutex_unlock(&sim->coders[i].state_lock);
		i++;
	}
	sim->start_released = 1;
	pthread_cond_broadcast(&sim->start_cond);
	pthread_mutex_unlock(&sim->start_lock);
}

static void	join_created_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->threads_created)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	if (sim->monitor_created)
		pthread_join(sim->monitor_thread, NULL);
}

static int	create_threads(t_simulation *sim)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL,
			burnout_monitor, sim) != 0)
		return (1);
	sim->monitor_created = 1;
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_life_cycle, &sim->coders[i]) != 0)
			return (1);
		sim->threads_created++;
		i++;
	}
	return (0);
}

int	simulation_run(t_simulation *sim)
{
	if (create_threads(sim) != 0)
	{
		simulation_stop(sim);
		release_start(sim);
		wake_all_dongles(sim);
		join_created_threads(sim);
		return (1);
	}
	release_start(sim);
	join_created_threads(sim);
	return (0);
}

void	simulation_destroy(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->coders_initialized)
		pthread_mutex_destroy(&sim->coders[i++].state_lock);
	i = 0;
	while (i < sim->dongles_initialized)
		dongle_destroy(&sim->dongles[i++]);
	free(sim->coders);
	free(sim->dongles);
	if (sim->base_mutexes_initialized)
	{
		pthread_cond_destroy(&sim->start_cond);
		pthread_mutex_destroy(&sim->start_lock);
		pthread_mutex_destroy(&sim->print_lock);
		pthread_mutex_destroy(&sim->stop_lock);
	}
}
