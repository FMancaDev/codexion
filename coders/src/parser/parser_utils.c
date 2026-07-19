/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:25:23 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:25:00 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/codexion.h"

int	parse_positive(const char *str, long long *value, int allow_zero)
{
	long long	number;
	int			i;

	if (!str || !str[0])
		return (1);
	number = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		number = number * 10 + (str[i] - '0');
		if (number > MAX_INPUT_VALUE)
			return (1);
		i++;
	}
	if (!allow_zero && number == 0)
		return (1);
	*value = number;
	return (0);
}
