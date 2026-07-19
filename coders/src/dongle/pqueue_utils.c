/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pqueue_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:25:42 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:23:12 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

int	waiter_before(t_waiting_coder a, t_waiting_coder b)
{
	if (a.priority_key != b.priority_key)
		return (a.priority_key < b.priority_key);
	if (a.sequence != b.sequence)
		return (a.sequence < b.sequence);
	return (a.coder_id < b.coder_id);
}

void	waiter_swap(t_waiting_coder *a, t_waiting_coder *b)
{
	t_waiting_coder	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	pqueue_is_empty(t_priority_queue *pq)
{
	return (pq->count == 0);
}
