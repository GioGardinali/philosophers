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

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int comeu;
	int		i;

	comeu = 0;
	data = (t_data *)arg;
	while (!data->stop_simulation)
	{
		i = 0;
		while (i < data->num_philosophers)
		{
			if (data->num_meals && data->philosophers[i].meals_eaten)
			if (get_timestamp() - data->philosophers[i].last_meal
				> data->time_to_die)
			{
				print_status(data, data->philosophers[i].id, "died");
				data->stop_simulation = 1;
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
