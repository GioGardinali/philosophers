/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:39:28 by gigardin          #+#    #+#             */
/*   Updated: 2024/10/02 20:25:15 by gigardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	m_last_meal;
	long			last_meal;
	int				meals_eaten;
	struct s_data	*data; // Adicionado para acessar os dados globais
}	t_philo;

typedef struct s_data
{
	int				num_philosophers;
	long			start_threads;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_meals;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_check_lock;
	pthread_mutex_t mutex_death;
	t_philo			*philosophers;
	pthread_mutex_t	*forks;
	int				stop_simulation;
}	t_data;

long	get_timestamp(void);
bool	check_meals_eanten(int meals_eaten, int num_meals, int stop);
void	print_status(t_data *data, int id, const char *status);
void	take_forks_and_eat(t_philo *philo, t_data *data,
			pthread_mutex_t *first, pthread_mutex_t *second);
void	check_philosopher_death(t_data *data, int *philos_end);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);
int		init_mutexes_check(t_data *data);
void	init_data_values(t_data *data, int argc, char **argv);
void	init_philosophers(t_data *data);
int		init_data(t_data *data, int argc, char **argv);
void	create_threads(t_data *data);
void	cleanup(t_data *data);

/*utils.c*/
int		ft_putstr_fd(char *s, int fd);
int		ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
size_t	ft_strlen(const char *s);
void	print_error(const char *msg);
#endif
