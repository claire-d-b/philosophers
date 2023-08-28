/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:39 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/27 12:00:22 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Returns the time since the program started

unsigned long	get_time(t_philo *philo)
{
	struct timeval	tv;

	pthread_mutex_lock(&philo->data->start_mutex);
	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", philo));
	unsigned long time = (tv.tv_sec * 1000000 + tv.tv_usec) - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->start_mutex);
	return (time);
}

// Returns the time since 1970

unsigned long	get_start_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", NULL));
	// one second = 1,000,000 usec
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}
