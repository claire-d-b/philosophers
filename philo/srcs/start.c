/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 16:22:18 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/28 09:36:29 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Pivot function creating a t_philo pointer from a void* to be set as parameter to the philo routine function

void	*start_routine(void *philo)
{
	t_philo	*phil;

	phil = (t_philo *)philo;
	philo_routine(phil);
	return (TRUE);
}

// The pthread_join() function provides a simple mechanism allowing an application to wait for a thread to terminate.
// After the thread terminates, the application may then choose to clean up resources that were used by the thread.

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

// Launching the threads in order :)

int	start_threads(t_philo *philo, int philo_number)
{
	int	ret;
	int	i;

	i = 0;
	ret = 0;
	while (i < philo_number)
	{
		ret = pthread_create(&philo->data->threads[i], NULL, start_routine, \
		(void *)&philo[i]);
		if (ret)
			return (print_error("Error in attempt to create thread\n", philo));
		i++;
	}
	return (TRUE);
}
