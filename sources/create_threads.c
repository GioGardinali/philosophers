/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:53:31 by gigardin          #+#    #+#             */
/*   Updated: 2024/09/30 21:05:25 by gigardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_data *data, int id, const char *status)
{
	pthread_mutex_lock(&data->mutex_death);
	pthread_mutex_lock(&data->print_lock);
	if (!data->stop_simulation)
	{
		printf("%9ld %d %s\n", get_timestamp() - data->start_threads, id, status);
		
	}
	pthread_mutex_unlock(&data->print_lock);
	pthread_mutex_unlock(&data->mutex_death);
}

int	init_data(t_data *data, int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc < 5 || argc > 6)
	{
		print_error("Usage: ./philo [number_of_philosophers] "
			"[time_to_die time_to_eat] [time_to_sleep] "
			"[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	data->num_philosophers = atoi(argv[1]);
	if (data->num_philosophers > 200)
	{
		print_error("Error: The number of philosophers must "
			"not exceed 200.\n");
		return (1);
	}
	data->time_to_die = atol(argv[2]);
	data->time_to_eat = atol(argv[3]);
	data->time_to_sleep = atol(argv[4]);
	if (argc == 6)
	{
		data->num_meals = atoi(argv[5]);
	}
	else
	{
		data->num_meals = 0;
	}
	if (data->num_philosophers <= 0 || data->time_to_die < 60
		|| data->time_to_eat < 60 || data->time_to_sleep < 60)
		{
		print_error("Invalid arguments\n");
		return (1);
	}
	data->forks = malloc(data->num_philosophers * sizeof(pthread_mutex_t));
	data->philosophers = malloc(data->num_philosophers * sizeof(t_philo));
	data->stop_simulation = 0;
	data->start_threads = 0;
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_check_lock, NULL); // Inicialização adicionada
	pthread_mutex_init(&data->mutex_death, NULL);
	while (i < data->num_philosophers)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philosophers[i].m_last_meal, NULL);
		i++;
	}
	return (0);
}

void	create_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < data->num_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = &data->forks[i];
		if (data->num_philosophers > 1)
			data->philosophers[i].right_fork = &data->forks[(i + 1)	% data->num_philosophers];
		else
			data->philosophers[i].right_fork = NULL;
		data->start_threads = get_timestamp();
		data->philosophers[i].last_meal = get_timestamp();
		data->philosophers[i].data = data; // Adicionado para acessar os dados globais
		pthread_create(&data->philosophers[i].thread, NULL,
			philosopher_routine, &data->philosophers[i]);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor_routine, data);
	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_check_lock);
	free(data->forks);
	free(data->philosophers);
}
