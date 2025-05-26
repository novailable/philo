/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoo <aoo@student.42singapore.sg>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:37:47 by aoo               #+#    #+#             */
/*   Updated: 2025/05/26 22:45:13 by aoo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	pthread_mutex_t	last_eat_lock;
	long			last_eat;
	int				meal_count;
	int				max_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			start_time;
	t_table			*table;
}	t_philo;

struct s_table
{
	int				philo_count;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	t_philo			*philos;
	pthread_mutex_t	stop_lock;
	int				stop;
};

bool	ft_atoi_vali(const char *nptr, int *result);
bool	input_vali(int argc, char **argv);
long	current_time_ms(long start_time);
bool	stop_check(t_table *table);
int		dc_print(t_philo *philo, char *str);
void	cleanup(t_table *table);
void	malloc_failed(t_table *table, char *msg);
bool	dead_check(t_table *table, t_philo *philo);
void	*check_end(void *arg);
void	eat(t_philo *philo);
#endif
