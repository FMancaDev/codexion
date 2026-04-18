#include "../../includes/codexion.h"

static void	swap(t_waiting_coder *a, t_waiting_coder *b)
{
	t_waiting_coder temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	pqueue_init(t_priority_queue *pq, int capacity)
{
	pq->entries = malloc(sizeof(t_waiting_coder) * capacity);
	if (!pq->entries)
		return (1);
	pq->count = 0;
	pq->max_capacity = capacity;
	return (0);
}

void	pqueue_free(t_priority_queue *pq)
{
	if (pq->entries)
	{
		free(pq->entries);
		pq->entries = NULL;
	}
	pq->count = 0;
}

int	pqueue_is_empty(t_priority_queue *pq)
{
	if (pq->count == 0)
		return (1);
	return (0);
}

int	pqueue_push(t_priority_queue *pq, int coder_id, long long key)
{
	int	i;
	int	parent;

	if (pq->count >= pq->max_capacity)
		return (1);
	pq->entries[pq->count].coder_id = coder_id;
	pq->entries[pq->count].priority_key = key;
	i = pq->count;
	pq->count++;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (pq->entries[i].priority_key < pq->entries[parent].priority_key)
		{
			swap(&pq->entries[i], &pq->entries[parent]);
			i = parent;
		}
		else
			break ;
	}
	return (0);
}

t_waiting_coder	pqueue_pop(t_priority_queue *pq)
{
	t_waiting_coder top;
	int	i;
	int	smallest;
	int	left;
	int	right;

	top = pq->entries[0];
	pq->count--;
	pq->entries[0] = pq->entries[pq->count];
	i = 0;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;

		if (left < pq->count && pq->entries[left].priority_key < pq->entries[smallest].priority_key)
			smallest = left;
		if (right < pq->count && pq->entries[right].priority_key < pq->entries[smallest].priority_key)
			smallest = right;

		if (smallest == i)
			break ;
		swap(&pq->entries[i], &pq->entries[smallest]);
		i = smallest;
	}
	return (top);
}


