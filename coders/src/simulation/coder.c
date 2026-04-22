#include "../../includes/codexion.h"

static int	perform_cycle(t_coder *coder)
{
	// take left dongle
	dongle_pick_up(coder->left_dongle, coder);
	pthread_mutex_lock(&coder->simulation->stop_flag_lock);

	if (coder->simulation->simulation_should_stop)
	{
		pthread_mutex_unlock(&coder->simulation->stop_flag_lock);
		return (1);
	}
	pthread_mutex_unlock(&coder->simulation->stop_flag_lock);

	// take right dongle
	dongle_pick_up(coder->right_dongle, coder);
	pthread_mutex_lock(&coder->simulation->stop_flag_lock);
	if (coder->simulation->simulation_should_stop)
	{
		pthread_mutex_unlock(&coder->simulation->stop_flag_lock);
		dongle_put_down(coder->left_dongle, coder->simulation);
		return (1);
	}
	pthread_mutex_unlock(&coder->simulation->stop_flag_lock);

	// compiling
	coder->last_compile_started_at = get_time_ms();
	print_coder_status(coder, "is compiling");
	usleep(coder->simulation->time_to_compile_ms * 1000);
	coder->times_compiled++;

	// dropping dongles
	dongle_put_down(coder->left_dongle, coder->simulation);
	dongle_put_down(coder->right_dongle, coder->simulation);

	// debugging and refactoring
	print_coder_status(coder, "is debugging");
	usleep(coder->simulation->time_to_debug_ms * 1000);
	
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
		pthread_mutex_lock(&coder->simulation->stop_flag_lock);
		if (coder->simulation->simulation_should_stop)
		{
			pthread_mutex_unlock(&coder->simulation->stop_flag_lock);
			break ;
		}
		pthread_mutex_unlock(&coder->simulation->stop_flag_lock);

		if (coder->simulation->compiles_required_to_stop != -1 &&
			coder->times_compiled >= coder->simulation->compiles_required_to_stop)
			break ;

		if (perform_cycle(coder) != 0)
			break ;
	}
	return (NULL);
}
