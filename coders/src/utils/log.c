#include "../../includes/codexion.h"

void	print_coder_status(t_coder *coder, const char *status_message)
{
	pthread_mutex_lock(&coder->simulation->print_lock);
	pthread_mutex_lock(&coder->simulation->stop_flag_lock);
	if (!coder->simulation->simulation_should_stop)
	{
		printf("%lld %d %s\n",
			time_since_start(coder->simulation),
			coder->id,
			status_message);
	}
	pthread_mutex_unlock(&coder->simulation->stop_flag_lock);
	pthread_mutex_unlock(&coder->simulation->print_lock);
}

void	print_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->simulation->print_lock);
	printf("%lld %d burned out\n",
		time_since_start(coder->simulation),
		coder->id);
	pthread_mutex_unlock(&coder->simulation->print_lock);
}
