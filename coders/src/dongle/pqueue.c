/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pqueue.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:25:45 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:22:48 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

static void	sift_up(t_priority_queue *pq, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!waiter_before(pq->entries[index], pq->entries[parent]))
			break ;
		waiter_swap(&pq->entries[index], &pq->entries[parent]);
		index = parent;
	}
}

static void	sift_down(t_priority_queue *pq, int index)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = index * 2 + 1;
		right = left + 1;
		smallest = index;
		if (left < pq->count
			&& waiter_before(pq->entries[left], pq->entries[smallest]))
			smallest = left;
		if (right < pq->count
			&& waiter_before(pq->entries[right], pq->entries[smallest]))
			smallest = right;
		if (smallest == index)
			break ;
		waiter_swap(&pq->entries[index], &pq->entries[smallest]);
		index = smallest;
	}
}

int	pqueue_init(t_priority_queue *pq, int capacity)
{
	pq->entries = malloc(sizeof(t_waiting_coder) * capacity);
	if (!pq->entries)
		return (1);
	pq->count = 0;
	pq->capacity = capacity;
	return (0);
}

void	pqueue_free(t_priority_queue *pq)
{
	free(pq->entries);
	pq->entries = NULL;
	pq->count = 0;
	pq->capacity = 0;
}

int	pqueue_push(t_priority_queue *pq, t_waiting_coder entry)
{
	int	index;

	if (pq->count >= pq->capacity)
		return (1);
	index = pq->count;
	pq->entries[index] = entry;
	pq->count++;
	sift_up(pq, index);
	return (0);
}

int	pqueue_pop(t_priority_queue *pq, t_waiting_coder *entry)
{
	if (pq->count == 0)
		return (1);
	if (entry)
		*entry = pq->entries[0];
	pq->count--;
	if (pq->count > 0)
	{
		pq->entries[0] = pq->entries[pq->count];
		sift_down(pq, 0);
	}
	return (0);
}

int	pqueue_remove(t_priority_queue *pq, int coder_id)
{
	int	i;

	i = 0;
	while (i < pq->count && pq->entries[i].coder_id != coder_id)
		i++;
	if (i == pq->count)
		return (1);
	pq->count--;
	if (i < pq->count)
		pq->entries[i] = pq->entries[pq->count];
	i = pq->count / 2;
	while (i > 0)
	{
		i--;
		sift_down(pq, i);
	}
	return (0);
}
