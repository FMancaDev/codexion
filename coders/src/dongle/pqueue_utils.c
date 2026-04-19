#include "../../includes/codexion.h"

void	swap(t_waiting_coder *a, t_waiting_coder *b)
{
	t_waiting_coder temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	pqueue_is_empty(t_priority_queue *pq)
{
	if (pq->count == 0)
		return (1);
	return (0);
}
