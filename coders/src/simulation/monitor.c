/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:20:12 by fomanca           #+#    #+#             */
/*   Updated: 2026/04/25 19:31:10 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

static int	check_individual_burnout(t_simulation *sim, t_coder *coder)
{
	long long	current_time;
	long long	time_without_compiling;
	int			i;

	current_time = get_time_ms();
	time_without_compiling = current_time - coder->last_compile_started_at;
	if (time_without_compiling >= sim->time_to_burnout_ms)
	{
		pthread_mutex_lock(&sim->stop_flag_lock);
		sim->simulation_should_stop = 1;
		pthread_mutex_unlock(&sim->stop_flag_lock);
		print_burnout(coder);
		i = 0;
		while (i < sim->number_of_coders)
		{
			pthread_mutex_lock(&sim->dongles[i].access_lock);
			pthread_cond_broadcast(&sim->dongles[i].became_available);
			pthread_mutex_unlock(&sim->dongles[i].access_lock);
			i++;
		}
		return (1);
	}
	return (0);
}

void	*burnout_monitor(void *arg)
{
	t_simulation	*sim;
	int				i;

	sim = (t_simulation *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->stop_flag_lock);
		if (sim->simulation_should_stop)
		{
			pthread_mutex_unlock(&sim->stop_flag_lock);
			break ;
		}
		pthread_mutex_unlock(&sim->stop_flag_lock);
		i = 0;
		while (i < sim->number_of_coders)
		{
			if (check_individual_burnout(sim, &sim->coders[i]))
				return (NULL);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
