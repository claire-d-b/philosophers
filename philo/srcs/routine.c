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

// Function that sets process data major infos i.e. if a philosopher has died or if all philosophers have eaten enough.

int	quit_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->count_mutex);
	if (!philo->nb_of_times_eat)
	{
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

// Function that increments of 1 the process data 'all_eat' when all philosopher have finished eating n times.

void	philo_eat(t_philo *philo)
{
	take_different_forks(philo);
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

// Prints the time when a philosopher starts sleeping

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex);
	print_msg(philo, "%lu %d is sleeping\n");
	pthread_mutex_unlock(&philo->data->mutex);
}

// Prints the time when a philosopher starts thinking

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex);
	print_msg(philo, "%lu %d is thinking\n");
	pthread_mutex_unlock(&philo->data->mutex);
}

// Function that is used as parameter of pthread_create.
// Is the routine of a philosopher / thread.

void	*philo_routine(t_philo *philo)
{	
	// main loop orchestrating philosophers activities
	pthread_mutex_lock(&philo->data->die_mutex);
	pthread_mutex_lock(&philo->data->end_mutex);
	while (!philo->data->died && !philo->data->end)
	{
		pthread_mutex_unlock(&philo->data->die_mutex);
		pthread_mutex_unlock(&philo->data->end_mutex);
		philo_eat(philo);
		usleep(philo->time_to_eat);
		release_different_forks(philo);
		philo_sleep(philo);
		usleep(philo->time_to_sleep);
		philo_think(philo);
		pthread_mutex_lock(&philo->data->die_mutex);
		pthread_mutex_lock(&philo->data->end_mutex);
	}
	pthread_mutex_unlock(&philo->data->die_mutex);
	pthread_mutex_unlock(&philo->data->end_mutex);
	return (NULL);
}
