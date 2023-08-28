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

// Function that checks if number_of_times_eat set, whether all philosophers have eaten enough times

int	all_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->all_eat_mutex);
	if (philo->data->all_eat == philo->philo_number && \
	philo->nb_of_times_eat) {
		pthread_mutex_unlock(&philo->data->all_eat_mutex);
		return (FALSE);
	}
	pthread_mutex_unlock(&philo->data->all_eat_mutex);
	return (TRUE);
}

// Fucntions that prints if a philosopher dies, and calls a function to set process data major info regarding
// whether 1) someone died or 2) all philosophers have eaten enough.
// Those booleans are later evaluated by each thread in a routine loop, and threads exits when one of both is equal to 1.

int	stop_numerous(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lm_mutex);
	pthread_mutex_lock(&philo->data->count_mutex);
	if (get_time(philo) - philo->last_meal > philo->time_to_die || philo->philo_number == 1)
	{
		pthread_mutex_unlock(&philo->data->lm_mutex);
		pthread_mutex_unlock(&philo->data->count_mutex);
		pthread_mutex_lock(&philo->data->mutex);
		printf("%lu %d died\n", get_time(philo) / 1000, philo->id);
		pthread_mutex_unlock(&philo->data->mutex);
		return (quit_routine(philo));
	}
	if (all_eat(philo)) {
		pthread_mutex_unlock(&philo->data->lm_mutex);
		pthread_mutex_unlock(&philo->data->count_mutex);
		return (quit_routine(philo));
	}
	pthread_mutex_unlock(&philo->data->count_mutex);
	pthread_mutex_unlock(&philo->data->lm_mutex);
	return (TRUE);
}

// If there is more than one philosopher, a fucntion that checks if the program should terminate is called.
// If there is only one philosopher, the program exits since there is only one fork.

int	is_it_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->count_mutex);
	if (philo->philo_number > 1)
	{	
		pthread_mutex_unlock(&philo->data->count_mutex);
		return (stop_numerous(philo));
	}
	else {
		pthread_mutex_unlock(&philo->data->count_mutex);
		return (FALSE);
	}
}

// Records the time - since the program started - when a philosophers starts eating.

void	record_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->lm_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->data->lm_mutex);
}
