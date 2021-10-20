/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:08:20 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/20 18:22:50 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_mutexes(t_philo *philo)
{
	if (pthread_mutex_init(&philo->data->mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->die_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->count_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->lm_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->end_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->time_cmp_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->time_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->alone_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->start_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	if (pthread_mutex_init(&philo->data->all_eat_mutex, NULL))
		return (print_error("Error in attempt to init mutex\n", philo));
	return (TRUE);
}

int	destroy_mutexes(int i, t_philo *philo)
{
	if (pthread_mutex_destroy(&philo->data->die_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->count_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->lm_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->end_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->time_cmp_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->time_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->alone_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->start_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	if (pthread_mutex_destroy(&philo->data->all_eat_mutex))
		return (print_error("Error in attempt to destroy mutex\n", philo));
	while (++i < philo->philo_number)
		if (pthread_mutex_destroy(&philo->data->forks_mutex[i]))
			return (print_error("Error in attempt to destroy mutex\n", philo));
	return (TRUE);
}

void	take_different_forks(t_philo *philo)
{
	if (philo->philo_number == 1)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
		take_forks(philo);
	}
	if (philo->id % 2 && philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);
		pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
		take_forks(philo);
		record_last_meal(philo);
	}
	else if (philo->philo_number > 1)
	{
		pthread_mutex_lock(&philo->data->forks_mutex[philo->left]);
		pthread_mutex_lock(&philo->data->forks_mutex[philo->right]);	
		take_forks(philo);
		record_last_meal(philo);	
	}
}

void	release_different_forks(t_philo *philo)
{
	if (philo->philo_number == 1)
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
	else if (philo->id % 2)
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->left]);
		pthread_mutex_unlock(&philo->data->forks_mutex[philo->right]);
	}
}
