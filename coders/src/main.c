/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fomanca <fomanca@student.42porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 19:27:00 by fomanca           #+#    #+#             */
/*   Updated: 2026/07/19 17:23:53 by fomanca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;
	int			status;

	if (simulation_init(&sim, argc, argv) != 0)
	{
		fprintf(stderr, "Error: invalid arguments or initialization failure\n");
		return (1);
	}
	status = simulation_run(&sim);
	simulation_destroy(&sim);
	if (status != 0)
	{
		fprintf(stderr, "Error: thread creation failure\n");
		return (1);
	}
	return (0);
}
