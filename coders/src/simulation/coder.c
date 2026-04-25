/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:10:48 by fomanca           #+#    #+#             */
/*   Updated: 2026/04/25 19:31:10 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

static int	stop_requested(t_coder *coder)
{
	int	should_stop;

	pthread_mutex_lock(&coder->simulation->stop_flag_lock);
	should_stop = coder->simulation->simulation_should_stop;
	pthread_mutex_unlock(&coder->simulation->stop_flag_lock);
	return (should_stop);
}

static void	wake_all_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].access_lock);
		pthread_cond_broadcast(&sim->dongles[i].became_available);
		pthread_mutex_unlock(&sim->dongles[i].access_lock);
		i++;
	}
}

static int	pick_up_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id % 2 == 0)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	else
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	dongle_pick_up(first, coder);
	if (stop_requested(coder))
		return (1);
	dongle_pick_up(second, coder);
	if (stop_requested(coder))
	{
		dongle_put_down(first, coder->simulation);
		return (1);
	}
	return (0);
}

static int	perform_cycle(t_coder *coder)
{
	if (pick_up_both_dongles(coder) != 0)
		return (1);
	coder->last_compile_started_at = get_time_ms();
	print_coder_status(coder, "is compiling");
	usleep(coder->simulation->time_to_compile_ms * 1000);
	coder->times_compiled++;
	dongle_put_down(coder->left_dongle, coder->simulation);
	dongle_put_down(coder->right_dongle, coder->simulation);
	if (stop_requested(coder))
		return (1);
	print_coder_status(coder, "is debugging");
	usleep(coder->simulation->time_to_debug_ms * 1000);
	if (stop_requested(coder))
		return (1);
	print_coder_status(coder, "is refactoring");
	usleep(coder->simulation->time_to_refactor_ms * 1000);
	return (0);
}

void	*coder_life_cycle(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	coder->last_compile_started_at = coder->simulation->simulation_started_at;
	while (1)
	{
		if (stop_requested(coder))
			break ;
		if (coder->times_compiled
			>= coder->simulation->compiles_required_to_stop)
		{
			pthread_mutex_lock(&coder->simulation->stop_flag_lock);
			coder->simulation->simulation_should_stop = 1;
			pthread_mutex_unlock(&coder->simulation->stop_flag_lock);
			wake_all_dongles(coder->simulation);
			break ;
		}
		if (perform_cycle(coder) != 0)
			break ;
	}
	return (NULL);
}
