/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:23 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/21 12:15:31 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	reasons_for_death(t_philo *philo)
{
	if ((philo->data->all_eat >= philo->philo_number && \
	philo->nb_of_times_eat) || philo->philo_number == 1 \
	|| get_time(philo) >= philo->last_meal + philo->time_to_die * 1000 \
	|| (get_time(philo) > philo->time_to_die * 1000 && \
	philo->time_to_die * 1000 < philo->time_to_eat * 2000))
		return (FALSE);
	return (TRUE);
}

int	stop_numerous(t_philo *philo)
{
	while (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->lm_mutex);
		pthread_mutex_lock(&philo->data->count_mutex);
		if (get_time(philo) >= philo->last_meal + philo->time_to_die * 1000 || \
		(philo->eat_count >= philo->nb_of_times_eat && philo->nb_of_times_eat) \
		|| philo->philo_number == 1 || (get_time(philo) > philo->time_to_die * \
		1000 && philo->time_to_die * 1000 < philo->time_to_eat * 2000))
		{
			pthread_mutex_lock(&philo->data->all_eat_mutex);
			if (reasons_for_death(philo))
			{
				pthread_mutex_unlock(&philo->data->lm_mutex);
				pthread_mutex_unlock(&philo->data->count_mutex);
				pthread_mutex_unlock(&philo->data->all_eat_mutex);
				return (quit_routine(philo));
			}
			pthread_mutex_unlock(&philo->data->all_eat_mutex);
		}
		pthread_mutex_unlock(&philo->data->count_mutex);
		pthread_mutex_unlock(&philo->data->lm_mutex);
		usleep(100);
	}
	return (TRUE);
}

int	stop_alone(t_philo *philo)
{
	while (get_time(philo) < philo->time_to_die * 1000)
		usleep(100);
	return (quit_routine(philo));
}

int	is_it_dead(t_philo *philo)
{
	if (philo->philo_number > 1)
		return (stop_numerous(philo));
	else
		return (stop_alone(philo));
}

void	record_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lm_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->data->lm_mutex);
}
