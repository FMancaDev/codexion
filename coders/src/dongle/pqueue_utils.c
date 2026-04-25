/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pqueue_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:25:42 by fomanca           #+#    #+#             */
/*   Updated: 2026/04/25 19:31:10 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/codexion.h"

void	swap(t_waiting_coder *a, t_waiting_coder *b)
{
	t_waiting_coder	temp;

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
