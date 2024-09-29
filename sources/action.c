/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:47:51 by gigardin          #+#    #+#             */
/*   Updated: 2024/09/29 05:25:27 by gigardin         ###   ########.fr       */
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
		// Taking forks
		if (philo->left_fork)
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(data, philo->id, "has taken a fork");
			if (philo->right_fork)
			{
				pthread_mutex_lock(philo->right_fork);
				print_status(data, philo->id, "has taken a fork");
				// Eating
				print_status(data, philo->id, "is eating");
				philo->last_meal = get_timestamp();
				usleep(data->time_to_eat * 1000);
				pthread_mutex_lock(&data->meal_check_lock);
				philo->meals_eaten++;
				pthread_mutex_unlock(&data->meal_check_lock);
				// Putting down forks
				pthread_mutex_unlock(philo->right_fork);
			}
			pthread_mutex_unlock(philo->left_fork);
		}
		// Sleeping
		print_status(data, philo->id, "is sleeping");
		usleep(data->time_to_sleep * 1000);
	}
	return (NULL);
}
