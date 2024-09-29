/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:47:51 by gigardin          #+#    #+#             */
/*   Updated: 2024/09/29 03:48:11 by gigardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int		all_ate_enough;
	int		i;

	philo = (t_philo *)arg;
	data = philo->data; // Corrigido para acessar a estrutura de dados correta
	all_ate_enough = 1;
	i = 0;
	while (!data->stop_simulation)
	{
		// Thinking
		print_status(data, philo->id, "is thinking");
		usleep(data->time_to_sleep * 1000);
		// Taking forks
		pthread_mutex_lock(philo->left_fork);
		print_status(data, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(data, philo->id, "has taken a fork");
		// Eating
		print_status(data, philo->id, "is eating");
		philo->last_meal = get_timestamp();
		usleep(data->time_to_eat * 1000);
		pthread_mutex_lock(&data->meal_check_lock);
		philo->meals_eaten++;
		if (data->num_meals != -1 && philo->meals_eaten >= data->num_meals)
		{
			while (i < data->num_philosophers)
			{
				if (data->philosophers[i].meals_eaten < data->num_meals)
				{
					all_ate_enough = 0;
					break ;
				}
				i++;
			}
			if (all_ate_enough)
			{
				data->stop_simulation = 1;
			}
		}
		pthread_mutex_unlock(&data->meal_check_lock);
		// Putting down forks
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		// Sleeping
		print_status(data, philo->id, "is sleeping");
		usleep(data->time_to_sleep * 1000);
	}
	return (NULL);
}
