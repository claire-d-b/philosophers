/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clde-ber <clde-ber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 15:08:58 by clde-ber          #+#    #+#             */
/*   Updated: 2021/10/26 18:00:33 by clde-ber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Writes string in specified fd

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s || !fd)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

// Prints take forks message

void	take_forks(t_philo *philo)
{
	if (philo->philo_number == 1)
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, \
		"%lu %d has taken a fork\n");
		pthread_mutex_unlock(&philo->data->mutex);
	}
	else
	{
		pthread_mutex_lock(&philo->data->mutex);
		print_msg(philo, "%lu %d has taken a fork\n");
		print_msg(philo, "%lu %d has taken a fork\n");
		pthread_mutex_unlock(&philo->data->mutex);
	}
}

// Function that prints all messages

void	print_msg(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->die_mutex);
	pthread_mutex_lock(&philo->data->end_mutex);
	if (!philo->data->died && !philo->data->end)
	{
		printf(msg, get_time(philo) / 1000, philo->id);
	}
	pthread_mutex_unlock(&philo->data->die_mutex);
	pthread_mutex_unlock(&philo->data->end_mutex);
}

// Function that prints error on standard error

int	print_error(char *msg, t_philo *philo)
{
	if (philo)
		pthread_mutex_lock(&philo->data->mutex);
	ft_putstr_fd(msg, 2);
	if (philo)
		pthread_mutex_unlock(&philo->data->mutex);
	return (ERROR);
}
