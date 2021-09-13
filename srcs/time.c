/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 16:14:58 by clde-ber          #+#    #+#             */
/*   Updated: 2021/09/13 10:28:45 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	get_time(t_philo *philo)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", philo));
	return ((tv.tv_sec * 1000000 + tv.tv_usec) - philo->data->start_time);
}

unsigned long	get_start_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (print_error("Error in getting time\n", NULL));
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void	wait_action(t_philo *philo, unsigned long time)
{
	pthread_mutex_lock(&philo->data->time_cmp_mutex);
	philo->time_cmp += time;
	pthread_mutex_unlock(&philo->data->time_cmp_mutex);
	while (1)
	{
		pthread_mutex_lock(&philo->data->cumul_time_mutex);
		philo->cumul_time = get_time(philo);
		pthread_mutex_unlock(&philo->data->cumul_time_mutex);
		pthread_mutex_lock(&philo->data->time_cmp_mutex);
		pthread_mutex_lock(&philo->data->cumul_time_mutex);
		if (philo->cumul_time >= philo->time_cmp)
		{
			pthread_mutex_unlock(&philo->data->time_cmp_mutex);
			pthread_mutex_unlock(&philo->data->cumul_time_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->time_cmp_mutex);
		pthread_mutex_unlock(&philo->data->cumul_time_mutex);
		usleep(1);
	}
}
