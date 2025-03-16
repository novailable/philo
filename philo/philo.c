/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoo <aoo@student.42singapore.sg>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:43:46 by aoo               #+#    #+#             */
/*   Updated: 2025/03/16 23:38:57 by aoo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_table(char *philo_count, t_table *table)
{
	int	i;

	ft_atoi_vali(philo_count, &table->philo_count);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->philo_count);
	if (!table->forks)
		(printf("forks malloc failed\n"), exit(1));
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_init(&table->forks[i++], NULL))
			malloc_failed(table, "forks mutex failed\n");
	}
	table->philos = (t_philo *)malloc(sizeof(t_philo) * table->philo_count);
	if (!table->philos)
		malloc_failed(table, "philos malloc failed\n");
	table->stop = 0;
	if (pthread_mutex_init(&table->stop_lock, NULL))
		malloc_failed(table, "table stop mutex failed\n");
	if (pthread_mutex_init(&table->print, NULL))
		malloc_failed(table, "philo print mutex failed\n");
}

void	create_philos(char **argv, t_table *table, long start_time)
{
	int	i;
	int	max_meal;

	max_meal = -1;
	i = 0;
	if (argv[5])
		ft_atoi_vali(argv[5], &max_meal);
	while (i < table->philo_count)
	{
		table->philos[i].id = i + 1;
		ft_atoi_vali(argv[2], &table->philos[i].die_time);
		ft_atoi_vali(argv[3], &table->philos[i].eat_time);
		ft_atoi_vali(argv[4], &table->philos[i].sleep_time);
		table->philos[i].last_eat = start_time;
		table->philos[i].start_time = start_time;
		table->philos[i].meal_count = 0;
		table->philos[i].max_meal = max_meal;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->\
		forks[(i + 1) % table->philo_count];
		table->philos[i].table = table;
		if (pthread_mutex_init(&table->philos[i++].last_eat_lock, NULL))
			malloc_failed(table, "philo last_eat mutex failed\n");
	}
}

void	*philo_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (stop_check(philo->table) || \
		(philo->max_meal != -1 && philo->meal_count >= philo->max_meal))
			return (NULL);
		if (philo->table->philo_count == 1)
			return (dc_print(philo, "has taken a fork"), \
					usleep(philo->die_time * 1000), NULL);
		eat(philo);
		if (stop_check(philo->table))
			return (NULL);
		if (philo->max_meal != -1 && philo->meal_count >= philo->max_meal)
			return (NULL);
		if (!dc_print(philo, "is sleeping"))
			return (NULL);
		usleep(philo->sleep_time * 1000);
		if (!dc_print(philo, "is thinking"))
			return (NULL);
		usleep(5000);
	}
}

void	*check_end(void *arg)
{
	int		i;
	int		full;
	t_philo	*philo;
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		i = 0;
		full = 0;
		while (i < table->philo_count)
		{
			philo = &table->philos[i];
			pthread_mutex_lock(&philo->last_eat_lock);
			if (dead_check(table, philo))
				return (pthread_mutex_unlock(&philo->last_eat_lock), NULL);
			if (philo->max_meal != -1 && philo->meal_count >= philo->max_meal)
				full++;
			pthread_mutex_unlock(&philo->last_eat_lock);
			i++;
		}
		if (full == table->philo_count)
			return (NULL);
	}
}

int	main(int argc, char **argv)
{
	t_table		table;
	pthread_t	*philos_life;
	pthread_t	monitor;
	int			i;

	if (!input_vali(argc, argv))
		exit(1);
	create_table(argv[1], &table);
	create_philos(argv, &table, current_time_ms(0));
	philos_life = (pthread_t *)malloc(sizeof(pthread_t) * table.philo_count);
	if (!philos_life)
		malloc_failed(&table, "philo life malloc failed\n");
	i = -1;
	while (++i < table.philo_count)
		if (pthread_create(&philos_life[i], NULL, philo_life, &table.philos[i]))
			(free(philos_life), malloc_failed(&table, \
								"philo life thread creation failed\n"));
	if (pthread_create(&monitor, NULL, check_end, &table))
		(free(philos_life), malloc_failed(&table, "monitor thread failed\n"));
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < table.philo_count)
		pthread_join(philos_life[i], NULL);
	return (cleanup(&table), free(philos_life), 0);
}
