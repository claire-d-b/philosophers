/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:23 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/13 16:09:20 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_it_dead(t_philo *philo)
{
	while (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->lm_mutex);
		if (get_time(philo) > philo->last_meal + philo->time_to_die * 1000 || \
		(philo->eat_count >= philo->nb_of_times_eat && philo->nb_of_times_eat) \
		|| philo->philo_number == 1)
		{
			pthread_mutex_unlock(&philo->data->lm_mutex);
			return (quit_routine(philo));
		}
		pthread_mutex_unlock(&philo->data->lm_mutex);
		usleep(100);
	}
	if (philo->philo_number == 1)
	{
		while (get_time(philo) < philo->time_to_die * 1000 && philo->philo_number == 1)
			usleep(100);
		return (quit_routine(philo));
	}
	return (TRUE);
}

void	record_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lm_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->data->lm_mutex);
}
