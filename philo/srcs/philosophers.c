/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:08:41 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/26 18:08:56 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	ret = is_it_dead(philo);
	join_threads(philo, philo->philo_number);
	destroy_mutexes(-1, philo);
	free_structs(philo);
	return (0);
}
