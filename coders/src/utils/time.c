/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 17:30:38 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:30:56 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

long long	time_since_start(t_simulation *sim)
{
	return (get_time_ms() - sim->start_time);
}

int	smart_sleep(t_simulation *sim, long long duration_ms)
{
	long long	end;

	end = get_time_ms() + duration_ms;
	while (!simulation_stopped(sim) && get_time_ms() < end)
		usleep(200);
	return (simulation_stopped(sim));
}

void	make_timeout(struct timespec *ts, long long wait_ms)
{
	struct timeval	tv;
	long long		microseconds;

	gettimeofday(&tv, NULL);
	microseconds = tv.tv_usec + wait_ms * 1000;
	ts->tv_sec = tv.tv_sec + microseconds / 1000000;
	ts->tv_nsec = (microseconds % 1000000) * 1000;
}
