#include "../../includes/codexion.h"

void	dongle_destroy(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->access_lock);
	pthread_cond_destroy(&dongle->became_available);
	pqueue_free(&dongle->waiting_coders);
}

int	dongle_init(t_dongle *dongle, int max_waiting_coders)
{
	if (pthread_mutex_init(&dongle->access_lock, NULL) != 0)
		return (1);
	if (pthread_cond_init(&dongle->became_available, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->access_lock);
		return (1);
	}
	dongle->is_held = 0;
	dongle->available_after_ms = 0;
	if (pqueue_init(&dongle->waiting_coders, max_waiting_coders) != 0)
	{
		pthread_mutex_destroy(&dongle->access_lock);
		pthread_cond_destroy(&dongle->became_available);
		return (1);
	}
	return (0);
}

void	dongle_pick_up(t_dongle *dongle, t_coder *coder)
{
	long long		key;
	struct timespec	ts;
	struct timeval	tv;

	pthread_mutex_lock(&dongle->access_lock);
	if (coder->simulation->scheduler_mode == FIRST_IN_FIRST_OUT)
		key = get_time_ms();
	else
		key = coder->last_compile_started_at
			+ coder->simulation->time_to_burnout_ms;
	pqueue_push(&dongle->waiting_coders, coder->id, key);
	while (dongle->is_held
		|| get_time_ms() < dongle->available_after_ms
		|| dongle->waiting_coders.entries[0].coder_id != coder->id)
	{
		if (coder->simulation->simulation_should_stop)
			break ;
		gettimeofday(&tv, NULL);
		ts.tv_sec = tv.tv_sec;
		ts.tv_nsec = tv.tv_usec * 1000 + 1000000;
		if (ts.tv_nsec >= 1000000000)
		{
			ts.tv_sec += 1;
			ts.tv_nsec -= 1000000000;
		}
		pthread_cond_timedwait(&dongle->became_available,
			&dongle->access_lock, &ts);
	}
	if (!coder->simulation->simulation_should_stop)
	{
		pqueue_pop(&dongle->waiting_coders);
		dongle->is_held = 1;
	}
	else
		pqueue_pop(&dongle->waiting_coders);
	pthread_mutex_unlock(&dongle->access_lock);
}

void	dongle_put_down(t_dongle *dongle, t_simulation *sim)
{
	pthread_mutex_lock(&dongle->access_lock);
	dongle->is_held = 0;
	dongle->available_after_ms = get_time_ms() + sim->dongle_cooldown_ms;
	pthread_cond_broadcast(&dongle->became_available);
	pthread_mutex_unlock(&dongle->access_lock);
}
