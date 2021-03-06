/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:13 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/27 14:06:22 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	quit_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->count_mutex);
	if ((philo->eat_count < philo->nb_of_times_eat && philo->nb_of_times_eat) \
	|| !philo->nb_of_times_eat)
	{
		pthread_mutex_lock(&philo->data->die_mutex);
		philo->data->died = 1;
		pthread_mutex_unlock(&philo->data->die_mutex);
		pthread_mutex_lock(&philo->data->mutex);
		printf("%lu %d died\n", get_time(philo) / 1000, philo->id);
		pthread_mutex_unlock(&philo->data->mutex);
	}
	else
	{
		pthread_mutex_lock(&philo->data->end_mutex);
		philo->data->end = 1;
		pthread_mutex_unlock(&philo->data->end_mutex);
	}
	pthread_mutex_unlock(&philo->data->count_mutex);
	return (FALSE);
}

void	philo_eat(t_philo *philo)
{
	take_different_forks(philo);
	if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu %d is eating\n");
		pthread_mutex_unlock(&philo->data->mutex);
		pthread_mutex_lock(&philo->data->count_mutex);
		philo->eat_count++;
		if (philo->eat_count == philo->nb_of_times_eat)
		{
			pthread_mutex_lock(&philo->data->all_eat_mutex);
			philo->data->all_eat++;
			pthread_mutex_unlock(&philo->data->all_eat_mutex);
		}
		pthread_mutex_unlock(&philo->data->count_mutex);
	}
}

void	philo_sleep(t_philo *philo)
{
	if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu %d is sleeping\n");
		pthread_mutex_unlock(&philo->data->mutex);
	}
}

void	philo_think(t_philo *philo, int boolean)
{
	int	adjust;

	adjust = 0;
	pthread_mutex_lock(&philo->data->lm_mutex);
	if (philo->last_meal)
		adjust = get_time(philo) - philo->last_meal - \
		philo->time_to_eat - philo->time_to_sleep;
	if (!philo->last_meal)
		adjust = get_time(philo) - philo->last_meal;
	pthread_mutex_unlock(&philo->data->lm_mutex);
	if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu %d is thinking\n");
		pthread_mutex_unlock(&philo->data->mutex);
		if ((philo->philo_number % 2 || boolean == 1) && adjust > 0)
		{
			philo->diff = 0;
			wait_action(philo, adjust);
		}
	}
}

void	*philo_routine(t_philo *philo)
{	
	if (philo->id % 2)
		philo_think(philo, 1);
	pthread_mutex_lock(&philo->data->die_mutex);
	pthread_mutex_lock(&philo->data->end_mutex);
	while (!philo->data->died & !philo->data->end)
	{
		pthread_mutex_unlock(&philo->data->die_mutex);
		pthread_mutex_unlock(&philo->data->end_mutex);
		philo_eat(philo);
		philo->diff = 0;
		wait_action(philo, philo->time_to_eat);
		release_different_forks(philo);
		philo_sleep(philo);
		philo->diff = 0;
		wait_action(philo, philo->time_to_sleep);
		philo_think(philo, 0);
		pthread_mutex_lock(&philo->data->die_mutex);
		pthread_mutex_lock(&philo->data->end_mutex);
	}
	pthread_mutex_unlock(&philo->data->die_mutex);
	pthread_mutex_unlock(&philo->data->end_mutex);
	return (NULL);
}
