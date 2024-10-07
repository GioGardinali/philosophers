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
#include <pthread.h>

bool	check_meals_eanten(int meals_eaten, int num_meals, t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_check_lock);
	if (num_meals && meals_eaten >= num_meals)
	{
		pthread_mutex_unlock(&philo->meal_check_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_check_lock);
	if (check_is_death(data))
		return (true);
	return (false);
}

bool	check_is_death(t_data *data)
{
	pthread_mutex_lock(&data->mutex_death);
	if (data->stop_simulation)
	{
		pthread_mutex_unlock(&data->mutex_death);
		return (true);
	}
	pthread_mutex_unlock(&data->mutex_death);
	return (false);
}

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
	while (!check_meals_eanten(philo->meals_eaten, data->num_meals, data, philo))
	{
		// Taking forks and eating
		take_forks_and_eat(philo, data, &first, &second);
		// Sleeping
		print_status(data, philo->id, "is sleeping");
		usleep(data->time_to_sleep * 1000);
		// Thinking
		print_status(data, philo->id, "is thinking");
	}
	return (NULL);
}

void	take_forks_and_eat(t_philo *philo, t_data *data, pthread_mutex_t **first, pthread_mutex_t **second)
{
	// Taking forks
	if (*first)
	{
		pthread_mutex_lock(*first);
		print_status(data, philo->id, "has taken a fork");
		if (*second)
		{
			pthread_mutex_lock(*second);
			print_status(data, philo->id, "has taken a fork");
			// Eating
			print_status(data, philo->id, "is eating");
			pthread_mutex_lock(&philo->m_last_meal);
			philo->last_meal = get_timestamp();
			pthread_mutex_unlock(&philo->m_last_meal);
			usleep(data->time_to_eat * 1000);
			pthread_mutex_lock(&philo->meal_check_lock);
			philo->meals_eaten++;
			pthread_mutex_unlock(&philo->meal_check_lock);
			// Putting down forks
			pthread_mutex_unlock(*second);
		}
		pthread_mutex_unlock(*first);
	}
}
