#include "../../includes/codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

long long	time_since_start(t_simulation *sim)
{
	return (get_time_ms() - sim->simulation_started_at);
}
