/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_i.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoo <aoo@student.42singapore.sg>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:38:09 by aoo               #+#    #+#             */
/*   Updated: 2025/03/16 23:37:20 by aoo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	stop_check(t_table *table)
{
	pthread_mutex_lock(&table->stop_lock);
	if (table->stop)
		return (pthread_mutex_unlock(&table->stop_lock), true);
	return (pthread_mutex_unlock(&table->stop_lock), false);
}

int	dc_print(t_philo *philo, char*str)
{
	if (!stop_check(philo->table))
	{
		pthread_mutex_lock(&philo->table->print);
		printf("%ld %d %s\n", current_time_ms(philo->start_time), \
				philo->id, str);
		pthread_mutex_unlock(&philo->table->print);
		return (1);
	}
	return (0);
}

void	eat(t_philo *philo)
{
	pthread_mutex_t	*f_fork;
	pthread_mutex_t	*s_fork;

	if (philo->id % 2)
	{
		f_fork = philo->right_fork;
		s_fork = philo->left_fork;
	}
	else
	{
		f_fork = philo->left_fork;
		s_fork = philo->right_fork;
	}
	pthread_mutex_lock(f_fork);
	dc_print(philo, "has taken a fork");
	pthread_mutex_lock(s_fork);
	dc_print(philo, "has taken a fork");
	pthread_mutex_lock(&philo->last_eat_lock);
	philo->last_eat = current_time_ms(0);
	philo->meal_count++;
	pthread_mutex_unlock(&philo->last_eat_lock);
	dc_print(philo, "is eating");
	usleep(philo->eat_time * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

bool	dead_check(t_table *table, t_philo *philo)
{
	if (current_time_ms(philo->last_eat) >= philo->die_time)
	{
		pthread_mutex_lock(&table->stop_lock);
		table->stop = 1;
		pthread_mutex_unlock(&table->stop_lock);
		pthread_mutex_lock(&table->print);
		printf("%ld %d died\n", current_time_ms(philo->start_time), philo->id);
		pthread_mutex_unlock(&table->print);
		return (true);
	}
	return (false);
}
