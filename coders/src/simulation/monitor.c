/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:20:12 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:26:39 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

static int	all_required_compiles_done(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (coder_compile_count(&sim->coders[i]) < sim->compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static t_coder	*find_burned_out_coder(t_simulation *sim)
{
	long long	now;
	int			i;

	now = get_time_ms();
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (now - coder_last_compile(&sim->coders[i])
			>= sim->time_to_burnout)
			return (&sim->coders[i]);
		i++;
	}
	return (NULL);
}

void	*burnout_monitor(void *arg)
{
	t_simulation	*sim;
	t_coder			*burned_out;

	sim = (t_simulation *)arg;
	wait_for_start(sim);
	while (!simulation_stopped(sim))
	{
		burned_out = find_burned_out_coder(sim);
		if (burned_out)
		{
			if (simulation_stop(sim))
				print_burnout(burned_out);
			wake_all_dongles(sim);
			break ;
		}
		if (all_required_compiles_done(sim))
		{
			simulation_stop(sim);
			wake_all_dongles(sim);
			break ;
		}
		usleep(200);
	}
	return (NULL);
}
