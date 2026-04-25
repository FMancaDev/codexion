#include "../../includes/codexion.h"

int	simulation_run(t_simulation *sim)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL, burnout_monitor, sim) != 0)
		return (1);
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_life_cycle, &sim->coders[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
	return (0);
}

void	simulation_destroy(t_simulation *sim)
{
	int	i;

	if (!sim)
		return ;
	if (sim->dongles)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			dongle_destroy(&sim->dongles[i]);
			i++;
		}
		free(sim->dongles);
		sim->dongles = NULL;
	}
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->stop_flag_lock);
}
