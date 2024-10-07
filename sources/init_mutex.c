/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 20:15:24 by gigardin          #+#    #+#             */
/*   Updated: 2024/10/02 20:16:38 by gigardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	valid_arguments(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		print_error("Usage: ./philo [number_of_philosophers] "
			"[time_to_die] [time_to_eat] [time_to_sleep] "
			"[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	if (atoi(argv[1]) > 200)
	{
		print_error("Error: The number of philosophers must "
			"not exceed 200.\n");
		return (1);
	}
	return (0);
}

void	init_data_values(t_data *data, int argc, char **argv)
{
	data->num_philosophers = atoi(argv[1]);
	data->time_to_die = atol(argv[2]);
	data->time_to_eat = atol(argv[3]);
	data->time_to_sleep = atol(argv[4]);
	if (argc == 6)
		data->num_meals = atoi(argv[5]);
	else
		data->num_meals = 0;
	data->stop_simulation = 0;
}

int	init_mutexes_check(t_data *data)
{
	data->forks = malloc(data->num_philosophers * sizeof(pthread_mutex_t));
	data->philosophers = malloc(data->num_philosophers * sizeof(t_philo));
	if (!data->forks || !data->philosophers)
	{
		print_error("Memory allocation failed\n");
		return (1);
	}
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->mutex_death, NULL);
	return (0);
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philosophers[i].m_last_meal, NULL);
		pthread_mutex_init(&data->philosophers[i].meal_check_lock, NULL);
		i++;
	}
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (valid_arguments(argc, argv))
		return (1);
	init_data_values(data, argc, argv);
	if (data->num_philosophers <= 0 || data->time_to_die < 60
		|| data->time_to_eat < 60 || data->time_to_sleep < 60)
	{
		print_error("Invalid arguments\n");
		return (1);
	}
	if (init_mutexes_check(data))
		return (1);
	init_philosophers(data);
	return (0);
}
