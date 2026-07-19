/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 17:31:49 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:32:01 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

int	simulation_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_lock);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stopped);
}

int	simulation_stop(t_simulation *sim)
{
	int	changed;

	pthread_mutex_lock(&sim->stop_lock);
	changed = (sim->stop == 0);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_lock);
	return (changed);
}

void	wake_all_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->dongles_initialized)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		pthread_cond_broadcast(&sim->dongles[i].available);
		pthread_mutex_unlock(&sim->dongles[i].lock);
		i++;
	}
}

void	wait_for_start(t_simulation *sim)
{
	pthread_mutex_lock(&sim->start_lock);
	while (!sim->start_released)
		pthread_cond_wait(&sim->start_cond, &sim->start_lock);
	pthread_mutex_unlock(&sim->start_lock);
}
