/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 17:30:20 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:30:32 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

void	print_status(t_coder *coder, const char *message)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->print_lock);
	if (!simulation_stopped(sim))
		printf("%lld %d %s\n", time_since_start(sim), coder->id, message);
	pthread_mutex_unlock(&sim->print_lock);
}

void	print_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->print_lock);
	printf("%lld %d burned out\n", time_since_start(coder->sim), coder->id);
	pthread_mutex_unlock(&coder->sim->print_lock);
}
