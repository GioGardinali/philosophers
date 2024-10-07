/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:53:31 by gigardin          #+#    #+#             */
/*   Updated: 2024/10/02 20:17:19 by gigardin         ###   ########.fr       */
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
	if (!check_is_death(data))
	{
		pthread_mutex_lock(&data->print_lock);
		printf("%9ld %d %s\n", get_timestamp() - data->philosophers[id - 1].start_threads,
			id, status);
		pthread_mutex_unlock(&data->print_lock);
	}
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
			data->philosophers[i].right_fork = &data->forks[(i + 1)
				% data->num_philosophers];
		else
			data->philosophers[i].right_fork = NULL;
		data->philosophers[i].start_threads = get_timestamp();
		data->philosophers[i].last_meal = get_timestamp();
		data->philosophers[i].data = data; // Adicionado para acessar os dados globais
		pthread_create(&data->philosophers[i].thread, NULL,
			philosopher_routine, &data->philosophers[i]);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor_routine, data);
	i = 0;
	while (i < data->num_philosophers)
		pthread_join(data->philosophers[i++].thread, NULL);
	pthread_join(monitor_thread, NULL);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philosophers[i].meal_check_lock);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	free(data->forks);
	free(data->philosophers);
}
