/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ii.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoo <aoo@student.42singapore.sg>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:39:25 by aoo               #+#    #+#             */
/*   Updated: 2025/03/16 23:37:11 by aoo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_time_ms(long start_time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) - start_time);
}

bool	input_vali(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc < 5 || argc > 6 || !ft_atoi_vali(argv[1], &i) || \
		!ft_atoi_vali(argv[2], &i) || !ft_atoi_vali(argv[3], &i) || \
		!ft_atoi_vali(argv[4], &i) || \
		(argc == 6 && !ft_atoi_vali(argv[5], &i)))
	{
		return (printf("Theses are the required arguments : \n"\
				"\tphilosopher count (integer)\n"\
				"\ttime to die (milliseconds)\n\ttime to eat (milliseconds)\n"\
				"\ttime to sleep (milliseconds)\n"\
				"\tnumber of times each philosopher must eat (optional)\n"\
				"\tall input must be positive\nInvalid input!\n"), false);
	}
	return (true);
}

void	cleanup(t_table *table)
{
	int	i;

	i = 0;
	if (table)
	{
		while (i < table->philo_count)
			pthread_mutex_destroy(&table->forks[i++]);
		i = 0;
		if (table->philos)
		{
			while (i < table->philo_count)
				pthread_mutex_destroy(&table->philos[i++].last_eat_lock);
			free(table->philos);
		}
		pthread_mutex_destroy(&table->stop_lock);
		pthread_mutex_destroy(&table->print);
		free(table->forks);
	}
}

void	malloc_failed(t_table *table, char *msg)
{
	(printf("%s\n", msg), cleanup(table), exit(1));
}
