/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:25:40 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:22:07 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

static t_waiting_coder	make_request(t_dongle *dongle, t_coder *coder)
{
	t_waiting_coder	request;

	request.coder_id = coder->id;
	request.sequence = dongle->next_sequence++;
	if (coder->sim->scheduler == FIRST_IN_FIRST_OUT)
		request.priority_key = request.sequence;
	else
		request.priority_key = coder_last_compile(coder)
			+ coder->sim->time_to_burnout;
	return (request);
}

static int	request_is_first(t_dongle *dongle, int coder_id)
{
	if (pqueue_is_empty(&dongle->queue))
		return (0);
	return (dongle->queue.entries[0].coder_id == coder_id);
}

int	dongle_init(t_dongle *dongle, int capacity)
{
	memset(dongle, 0, sizeof(*dongle));
	if (pthread_mutex_init(&dongle->lock, NULL) != 0)
		return (1);
	if (pthread_cond_init(&dongle->available, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->lock);
		return (1);
	}
	if (pqueue_init(&dongle->queue, capacity) != 0)
	{
		pthread_cond_destroy(&dongle->available);
		pthread_mutex_destroy(&dongle->lock);
		return (1);
	}
	return (0);
}

void	dongle_destroy(t_dongle *dongle)
{
	pqueue_free(&dongle->queue);
	pthread_cond_destroy(&dongle->available);
	pthread_mutex_destroy(&dongle->lock);
}

int	dongle_pick_up(t_dongle *dongle, t_coder *coder)
{
	t_waiting_coder	request;
	struct timespec	timeout;

	pthread_mutex_lock(&dongle->lock);
	request = make_request(dongle, coder);
	if (pqueue_push(&dongle->queue, request) != 0)
		return (pthread_mutex_unlock(&dongle->lock), 1);
	while (!simulation_stopped(coder->sim) && (dongle->held
			|| get_time_ms() < dongle->available_after_ms
			|| !request_is_first(dongle, coder->id)))
	{
		make_timeout(&timeout, 1);
		pthread_cond_timedwait(&dongle->available, &dongle->lock, &timeout);
	}
	if (simulation_stopped(coder->sim))
		pqueue_remove(&dongle->queue, coder->id);
	else
	{
		pqueue_pop(&dongle->queue, NULL);
		dongle->held = 1;
	}
	pthread_mutex_unlock(&dongle->lock);
	if (simulation_stopped(coder->sim))
		return (1);
	print_status(coder, "has taken a dongle");
	return (0);
}

void	dongle_put_down(t_dongle *dongle, t_simulation *sim)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->held = 0;
	dongle->available_after_ms = get_time_ms() + sim->dongle_cooldown;
	pthread_cond_broadcast(&dongle->available);
	pthread_mutex_unlock(&dongle->lock);
}

