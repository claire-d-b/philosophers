/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 16:22:18 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/13 16:24:20 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*start_routine(void *philo)
{
	t_philo	*phil;

	phil = (t_philo *)philo;
	philo_routine(philo);
	return (TRUE);
}

int	join_threads(t_philo *philo, int philo_number)
{
	int	ret;
	int	i;

	i = 0;
	ret = 0;
	while (i < philo_number)
	{
		ret = pthread_join(philo->data->threads[i], NULL);
		if (ret)
			return (print_error("Error in attempt to join thread\n", philo));
		i++;
	}
	return (TRUE);
}

int	start_threads(t_philo *philo, int philo_number)
{
	int	ret;
	int	i;

	i = 0;
	ret = 0;
	while (i < philo_number)
	{
		ret = pthread_create(&philo->data->threads[i], NULL, &start_routine, \
		(void *)&philo[i]);
		if (ret)
			return (print_error("Error in attempt to create thread\n", philo));
		usleep(100);
		i++;
	}
	return (TRUE);
}
