/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:08:41 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/13 16:22:10 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_structs_error(t_philo *philo, t_data *infos)
{
	free(infos->threads);
	free(infos->forks_mutex);
	free(infos);
	free(philo);
}

void	free_structs(t_philo *philo)
{
	free(philo->data->forks_mutex);
	free(philo->data->threads);
	free(philo->data);
	free(philo);
}

int	main(int ac, char **av)
{
	t_philo			*philo;
	t_data			*infos;

	philo = NULL;
	infos = NULL;
	if (ac < 5 || ac > 6)
		return (print_error("Error in arguments\n", NULL));
	else
	{
		init_structs(&infos, &philo, av);
		shared_data(infos, av);
		if (ft_atoi(av[1]) <= 0 || \
		create_forks_a_philo(0, infos, philo, av) == ERROR)
		{
			free_structs_error(philo, infos);
			return (print_error("Error in arguments\n", NULL));
		}
		destroy_mutexes(0, philo);
	}
	free_structs(philo);
	return (0);
}