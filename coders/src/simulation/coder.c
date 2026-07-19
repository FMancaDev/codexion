/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:10:48 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:25:56 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

static int	pick_up_pair(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->id % 2 == 0)
	{
		first = coder->right;
		second = coder->left;
	}
	else
	{
		first = coder->left;
		second = coder->right;
	}
	if (dongle_pick_up(first, coder) != 0)
		return (1);
	if (dongle_pick_up(second, coder) != 0)
	{
		dongle_put_down(first, coder->sim);
		return (1);
	}
	return (0);
}

static void	put_down_pair(t_coder *coder)
{
	dongle_put_down(coder->left, coder->sim);
	dongle_put_down(coder->right, coder->sim);
}

static int	perform_cycle(t_coder *coder)
{
	if (pick_up_pair(coder) != 0)
		return (1);
	coder_mark_compile_start(coder);
	print_status(coder, "is compiling");
	if (smart_sleep(coder->sim, coder->sim->time_to_compile))
		return (put_down_pair(coder), 1);
	coder_increment_compile_count(coder);
	put_down_pair(coder);
	print_status(coder, "is debugging");
	if (smart_sleep(coder->sim, coder->sim->time_to_debug))
		return (1);
	print_status(coder, "is refactoring");
	return (smart_sleep(coder->sim, coder->sim->time_to_refactor));
}

static void	single_coder(t_coder *coder)
{
	if (dongle_pick_up(coder->left, coder) == 0)
	{
		while (!simulation_stopped(coder->sim))
			usleep(200);
		dongle_put_down(coder->left, coder->sim);
	}
}

void	*coder_life_cycle(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	wait_for_start(coder->sim);
	if (coder->sim->number_of_coders == 1)
		single_coder(coder);
	else
	{
		if (coder->id % 2 == 0)
			usleep(500);
		while (!simulation_stopped(coder->sim))
		{
			if (perform_cycle(coder) != 0)
				break ;
		}
	}
	return (NULL);
}
