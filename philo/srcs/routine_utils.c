/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:23 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/27 14:14:01 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	reasons_for_death(t_philo *philo, unsigned long alt_eat)
{
	if ((philo->data->all_eat >= philo->philo_number && \
	philo->nb_of_times_eat) || philo->philo_number == 1 \
	|| get_time(philo) >= philo->last_meal + philo->time_to_die \
	|| (get_time(philo) > philo->time_to_die && \
	philo->time_to_die < alt_eat))
		return (FALSE);
	return (TRUE);
}

int	stop_numerous(t_philo *philo, unsigned long die, unsigned long alt_eat)
{
	while (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->lm_mutex);
		pthread_mutex_lock(&philo->data->count_mutex);
		if (get_time(philo) >= philo->last_meal + die || \
		(philo->eat_count >= philo->nb_of_times_eat && philo->nb_of_times_eat) \
		|| philo->philo_number == 1 || (get_time(philo) > die && die < alt_eat))
		{
			pthread_mutex_lock(&philo->data->all_eat_mutex);
			if (reasons_for_death(philo, alt_eat))
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
		usleep(1000);
	}
	return (TRUE);
}

int	stop_alone(t_philo *philo, unsigned long die)
{
	while (get_time(philo) < die)
		usleep(1000);
	return (quit_routine(philo));
}

int	is_it_dead(t_philo *philo)
{
	if (philo->philo_number > 1)
		return (stop_numerous(philo, philo->time_to_die, \
		philo->time_to_eat * 2));
	else
		return (stop_alone(philo, philo->time_to_die));
}

void	record_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lm_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->data->lm_mutex);
}
