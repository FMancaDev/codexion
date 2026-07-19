/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 17:31:26 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:31:44 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

long long	coder_last_compile(t_coder *coder)
{
	long long	value;

	pthread_mutex_lock(&coder->state_lock);
	value = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_lock);
	return (value);
}

int	coder_compile_count(t_coder *coder)
{
	int	value;

	pthread_mutex_lock(&coder->state_lock);
	value = coder->times_compiled;
	pthread_mutex_unlock(&coder->state_lock);
	return (value);
}

void	coder_mark_compile_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_lock);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->state_lock);
}

void	coder_increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_lock);
	coder->times_compiled++;
	pthread_mutex_unlock(&coder->state_lock);
}
