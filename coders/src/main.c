#include "../includes/codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (simulation_init(&sim, argc, argv) != 0)
	{
		printf("Error: Initialization failed\n");
		return (1);
	}
	if (simulation_run(&sim) != 0)
	{
		printf("Error: Thread creation failed\n");
	}
	simulation_destroy(&sim);
	return (0);
}
