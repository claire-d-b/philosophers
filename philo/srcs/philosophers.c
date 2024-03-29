/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:08:41 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/30 08:56:55 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Defines the number of times each philosopher must eat if set, the programs should finish when this number is reached for all philosophers

void	record_nb_of_times_eat(t_philo *philo, char **av, char *str, int *ret)
{
	free(str);
	if (av[5])
	{
		str = ft_itoa(ft_atoi(av[5]));
		philo->nb_of_times_eat = ft_atoi(av[5]);
		if (!is_number(av[5]) || ft_atoi(av[5]) < 0 || \
		ft_strcmp(str, av[5]))
		{
			*ret = ERROR;
			free(str);
			return ;
		}
		if (ft_atoi(av[5]) == 0)
			*ret = FALSE;
		free(str);
	}
}

// Free memory functions to avoid leaks

void	free_structs_no_routine(t_philo *philo, t_data *infos)
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

// Main process

int	main(int ac, char **av)
{
	t_philo			*philo;
	t_data			*infos;
	int				ret;

	philo = NULL;
	infos = NULL;
	if (ac < 5 || ac > 6 || ft_atoi(av[1]) <= 0)
		return (print_error("Error in arguments\n", NULL));
	if (init_structs(&infos, &philo, av) || shared_data(infos, av) == ERROR)
		return (print_error("Malloc error\n", NULL));
	ret = create_forks_a_philo(-1, infos, philo, av);
	if (ret != TRUE)
		free_structs_no_routine(philo, infos);
	if (ret == ERROR)
		return (print_error("Error in arguments\n", NULL));
	else if (ret == FALSE)
		return (FALSE);
	init_mutexes(philo);
	start_threads(philo, philo->philo_number);
	while (is_it_dead(philo) == TRUE) usleep(1000);
	join_threads(philo, philo->philo_number);
	destroy_mutexes(-1, philo);
	free_structs(philo);
	return (0);
}
