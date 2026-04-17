#include "../../includes/codexion.h"

static int	is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	simulation_init(t_simulation *sim, int argc, char **argv)
{
	int	i;

	if (argc != 9)
		return (1);

	i = 1;
	while (i <= 7)
	{
		if (!is_digit(argv[i]))
			return (1);
		i++;
	}
	sim->number_of_coders = atoi(argv[1]);
	sim->time_to_burnout_ms = atoi(argv[2]);
	sim->time_to_compile_ms = atoi(argv[3]);
	sim->time_to_debug_ms = atoi(argv[4]);
	sim->time_to_refactor_ms = atoi(argv[5]);
	sim->compiles_required_to_stop = atoi(argv[6]);
	sim->dongle_cooldown_ms = atoi(argv[7]);
	if (sim->number_of_coders < 1)
		return (1);
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler_mode = FIRST_IN_FIRST_OUT;
	else if (strcmp(argv[0], "edf") == 0)
		sim->scheduler_mode = EARLIEST_DEADLINE_FIRST;
	else
		return (1);
	sim->simulation_should_stop = 0;
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->stop_flag_lock, NULL) != 0)
		return (1);
	return (0);
}
