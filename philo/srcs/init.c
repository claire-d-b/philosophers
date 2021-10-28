/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:08:04 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/28 09:47:24 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_structs(t_data **infos, t_philo **philo, char **args)
{
	if (ft_atoi(args[1]) < 0)
		return (ERROR);
	*infos = malloc(sizeof(t_data));
	if (!*infos)
		return (ERROR);
	memset(*infos, 0, sizeof(t_data));
	(*infos)->forks_mutex = malloc(sizeof(pthread_mutex_t) * ft_atoi(args[1]));
	if (!((*infos)->forks_mutex))
	{
		free(*infos);
		return (ERROR);
	}
	memset((*infos)->forks_mutex, 0, sizeof(pthread_mutex_t) * \
	ft_atoi(args[1]));
	*philo = malloc(sizeof(t_philo) * ft_atoi(args[1]));
	if (!*philo)
	{
		free((*infos)->forks_mutex);
		free(*infos);
		return (ERROR);
	}
	memset(*philo, 0, sizeof(t_philo) * ft_atoi(args[1]));
	return (TRUE);
}

int	shared_data(t_data *infos, char **av)
{
	if (ft_atoi(av[1]) < 0)
		return (ERROR);
	infos->start_time = get_start_time();
	infos->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
	if (!infos->threads)
		return (ERROR);
	return (TRUE);
}

void	link_philos(t_philo *philo, int i, char **av)
{
	philo->philo_number = ft_atoi(av[1]);
	philo->time_to_die = ft_atoi(av[2]) * 1000;
	philo->time_to_eat = ft_atoi(av[3]) * 1000;
	philo->time_to_sleep = ft_atoi(av[4]) * 1000;
	philo->id = i + 1;
	philo->right = (i + 1) % philo->philo_number;
	philo->left = i;
}

int	init_philo(t_philo *philo, char **av)
{
	char	*str;
	int		ret;

	ret = TRUE;
	str = ft_itoa(ft_atoi(av[1]));
	if (!is_number(av[1]) || ft_atoi(av[1]) < 0 || ft_strcmp(str, av[1]))
		ret = ERROR;
	if (ft_atoi(av[1]) == 0)
		ret = FALSE;
	free(str);
	str = ft_itoa(ft_atoi(av[2]));
	if (!is_number(av[2]) || ft_atoi(av[2]) < 0 || ft_strcmp(str, av[2]))
		ret = ERROR;
	free(str);
	str = ft_itoa(ft_atoi(av[3]));
	if (!is_number(av[3]) || ft_atoi(av[3]) < 0 || ft_strcmp(str, av[3]))
		ret = ERROR;
	free(str);
	str = ft_itoa(ft_atoi(av[4]));
	if (!is_number(av[4]) || ft_atoi(av[4]) < 0 || ft_strcmp(str, av[4]))
		ret = ERROR;
	record_nb_of_times_eat(philo, av, str, &ret);
	if (ret)
		return (ret);
	return (TRUE);
}

int	create_forks_a_philo(int i, t_data *infos, t_philo *philo, \
char **av)
{
	int	ret;

	ret = 0;
	while (++i < ft_atoi(av[1]))
	{
		memset(&philo[i], 0, sizeof(t_philo));
		ret = init_philo(&philo[i], av);
		if (ret != TRUE)
		{
			while (--i > 0)
			{
				if (pthread_mutex_destroy(&philo->data->forks_mutex[i]))
					return (print_error("Error in attempt to destroy mutex\n", \
					philo));
			}
			return (ret);
		}
		philo[i].data = infos;
		link_philos(&philo[i], i, av);
		if (pthread_mutex_init(&philo->data->forks_mutex[i], NULL))
			return (print_error("Error in attempt to init mutex\n", philo));
	}
	return (TRUE);
}
