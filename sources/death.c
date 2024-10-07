/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:50:48 by gigardin          #+#    #+#             */
/*   Updated: 2024/10/02 20:26:00 by gigardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	check_full_end(int *philos_end, int num_philosophers)
{
	int	i;

	i = -1;
	while (philos_end[++i] != 0)
		;
	if (i >= (num_philosophers))
		return (true);
	return (false);
}

void	init_arr_int(int *arr)
{
	int	i;

	i = 0;
	while (++i < 200)
	arr[i] = 0;
}

void	check_philosopher_death(t_data *data, int *philos_end)
{
	int	i;

	i = -1;
	while (++i < data->num_philosophers)
	{
		if (check_meals_eanten(data->philosophers[i].meals_eaten,
				data->num_meals, data, &data->philosophers[i]))
			philos_end[i] = data->philosophers[i].id;
		pthread_mutex_lock(&data->philosophers[i].m_last_meal);
		if (!check_is_death(data) && (get_timestamp() - data->philosophers[i].last_meal
			> data->time_to_die))
		{
			pthread_mutex_unlock(&data->philosophers[i].m_last_meal);
			pthread_mutex_lock(&data->mutex_death);
			pthread_mutex_lock(&data->print_lock);
			printf("%9ld %d died\n", get_timestamp() - data->philosophers[i].start_threads,
				data->philosophers[i].id);
			data->stop_simulation = 1;
			pthread_mutex_unlock(&data->print_lock);
			pthread_mutex_unlock(&data->mutex_death);
			return ;
		}
		pthread_mutex_unlock(&data->philosophers[i].m_last_meal);
	}
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		philos_end[200];

	data = (t_data *)arg;
	init_arr_int(philos_end);
	while (!check_is_death(data) && !check_full_end(philos_end,
			data->num_philosophers))
	{
		check_philosopher_death(data, philos_end);
		usleep(1000);
	}
	return (NULL);
}
