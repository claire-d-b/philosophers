/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:09:13 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/21 15:15:54 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	quit_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->count_mutex);
	if ((philo->eat_count < philo->nb_of_times_eat && philo->nb_of_times_eat) \
	|| !philo->nb_of_times_eat)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu milliseconds : philosopher %d died\n");
		pthread_mutex_unlock(&philo->data->mutex);
		pthread_mutex_lock(&philo->data->die_mutex);
		philo->data->died = 1;
		pthread_mutex_unlock(&philo->data->die_mutex);
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
		print_msg(philo, "%lu milliseconds : philosopher %d is eating\n");
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
		print_msg(philo, "%lu milliseconds : philosopher %d is sleeping\n");
		pthread_mutex_unlock(&philo->data->mutex);
	}
}

void	philo_think(t_philo *philo)
{
	int	adjust;

	adjust = 0;
	pthread_mutex_lock(&philo->data->lm_mutex);
	pthread_mutex_lock(&philo->data->count_mutex);
	adjust = (philo->last_meal / 1000 - ((philo->time_to_eat + \
	philo->time_to_sleep) * (philo->eat_count - 1)));
	pthread_mutex_unlock(&philo->data->lm_mutex);
	pthread_mutex_unlock(&philo->data->count_mutex);
	if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu milliseconds : philosopher %d is thinking\n");
		pthread_mutex_unlock(&philo->data->mutex);
		if (philo->id % 2 && philo->philo_number % 2 && adjust > 0)
		{
			philo->diff = 0;
			wait_action(philo, adjust);
		}
	}
}

void	*philo_routine(t_philo *philo)
{	
	if (philo->id % 2)
		philo_think(philo);
	while (1)
	{
		pthread_mutex_lock(&philo->data->die_mutex);
		pthread_mutex_lock(&philo->data->end_mutex);
		if (philo->data->died || philo->data->end)
		{
			pthread_mutex_unlock(&philo->data->die_mutex);
			pthread_mutex_unlock(&philo->data->end_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->die_mutex);
		pthread_mutex_unlock(&philo->data->end_mutex);
		philo_eat(philo);
		philo->diff = 0;
		wait_action(philo, philo->time_to_eat * 1000);
		release_different_forks(philo);
		philo_sleep(philo);
		philo->diff = 0;
		wait_action(philo, philo->time_to_sleep * 1000);
		philo_think(philo);
	}
	return (NULL);
}
