/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:50:48 by gigardin          #+#    #+#             */
/*   Updated: 2024/09/29 05:50:47 by gigardin         ###   ########.fr       */
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
	return false;
}

void	init_arr_int(int *arr)
{
	int i;

	i = 0;
	while (++i < 200)
	arr[i] = 0;
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		philos_end[200];
	int		i;

	data = (t_data *)arg;
	init_arr_int(philos_end);
	while (!data->stop_simulation && !check_full_end(philos_end, data->num_philosophers)) 
	{
		i = -1;
		while (++i < data->num_philosophers)
		{
			if (check_meals_eanten(data->philosophers[i].meals_eaten, data->num_meals))
				philos_end[i] = data->philosophers[i].id;
			else if (get_timestamp() - data->philosophers[i].last_meal
				> data->time_to_die)
			{
				print_status(data, data->philosophers[i].id, "died");
				data->stop_simulation = 1;
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}
