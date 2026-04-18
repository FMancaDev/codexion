#include "../../includes/codexion.h"

void	dongle_destroy(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->access_lock);
	pthread_cond_destroy(&dongle->became_available);
	pqueue_free(&dongle->waiting_coders); // liberta memoria da heap
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
