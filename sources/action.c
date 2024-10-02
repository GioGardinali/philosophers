/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:47:51 by gigardin          #+#    #+#             */
/*   Updated: 2024/10/02 20:00:16 by gigardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	check_meals_eanten(int meals_eaten, int num_meals, int stop)
{
	if (stop || (num_meals && meals_eaten >= num_meals))
	{
		return (true);
	}
	return (false);
}

// void	*philosopher_routine(void *arg)
// {
// 	t_philo	*philo;
// 	t_data	*data;
// 	pthread_mutex_t	*first;
// 	pthread_mutex_t	*second;

// 	philo = (t_philo *)arg;
// 	first = philo->left_fork;
// 	second = philo->right_fork;
// 	if (philo->id % 2 == 0)
// 	{
// 		first = philo->right_fork;
// 		second = philo->left_fork;
// 	}
// 	data = philo->data; // Corrigido para acessar a estrutura de dados correta
// 	while (!check_meals_eanten(philo->meals_eaten, data->num_meals, data->stop_simulation))
// 	{
// 		// Thinking
// 		print_status(data, philo->id, "is thinking");
// 		// Taking forks
// 		if (first)
// 		{
// 			pthread_mutex_lock(first);
// 			print_status(data, philo->id, "has taken a fork");
// 			if (second)
// 			{
// 				pthread_mutex_lock(second);
// 				print_status(data, philo->id, "has taken a fork");
// 				// Eating
// 				print_status(data, philo->id, "is eating");
// 				pthread_mutex_lock(&philo->m_last_meal);
// 				philo->last_meal = get_timestamp();
// 				pthread_mutex_unlock(&philo->m_last_meal);				
// 				usleep(data->time_to_eat * 1000);
// 				pthread_mutex_lock(&data->meal_check_lock);
// 				philo->meals_eaten++;
// 				pthread_mutex_unlock(&data->meal_check_lock);
// 				// Putting down forks
// 				pthread_mutex_unlock(second);
// 			}
// 			pthread_mutex_unlock(first);
// 		}
// 		// Sleeping
// 		print_status(data, philo->id, "is sleeping");
// 		usleep(data->time_to_sleep * 1000);
// 	}
// 	return (NULL);
// }

void	*philosopher_routine(void *arg)
{
	t_philo			*philo;
	t_data			*data;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	philo = (t_philo *)arg;
	first = philo->left_fork;
	second = philo->right_fork;
	if (philo->id % 2 == 0)
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	data = philo->data; // Corrigido para acessar a estrutura de dados correta
	while (!check_meals_eanten(philo->meals_eaten, data->num_meals, data->stop_simulation))
	{
		// Thinking
		print_status(data, philo->id, "is thinking");
		// Taking forks and eating
		take_forks_and_eat(philo, data, first, second);
		// Sleeping
		print_status(data, philo->id, "is sleeping");
		usleep(data->time_to_sleep * 1000);
	}
	return (NULL);
}

void	take_forks_and_eat(t_philo *philo, t_data *data, pthread_mutex_t *first, pthread_mutex_t *second)
{
	// Taking forks
	if (first)
	{
		pthread_mutex_lock(first);
		print_status(data, philo->id, "has taken a fork");
		if (second)
		{
			pthread_mutex_lock(second);
			print_status(data, philo->id, "has taken a fork");
			// Eating
			print_status(data, philo->id, "is eating");
			pthread_mutex_lock(&philo->m_last_meal);
			philo->last_meal = get_timestamp();
			pthread_mutex_unlock(&philo->m_last_meal);
			usleep(data->time_to_eat * 1000);
			pthread_mutex_lock(&data->meal_check_lock);
			philo->meals_eaten++;
			pthread_mutex_unlock(&data->meal_check_lock);
			// Putting down forks
			pthread_mutex_unlock(second);
		}
		pthread_mutex_unlock(first);
	}
}
