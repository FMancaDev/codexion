#include "../../includes/codexion.h"

void	dongle_destroy(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->access_lock);
	pthread_cond_destroy(&dongle->became_available);
	pqueue_free(&dongle->waiting_coders); // liberta memoria da heap
}
