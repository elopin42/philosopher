/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:31:12 by elopin            #+#    #+#             */
/*   Updated: 2025/05/21 22:36:38 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void	choose_direction_unlock(t_philo *philo, int i, int y);

bool	check_if_dead(t_philo *philo)
{
	bool	is_alive;

	pthread_mutex_lock(&philo->mutex_meal);
	pthread_mutex_lock(philo->death_mu);
	is_alive = (*philo->death != 0);
	pthread_mutex_unlock(philo->death_mu);
	pthread_mutex_unlock(&philo->mutex_meal);
	return (is_alive);
}

long	get_time_in_ms(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

int	choose_direction(t_philo *philo, int *i, int *y)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	if (!*i && !pthread_mutex_lock(first))
	{
		pti_printf("%ld %d has taken a fork\n", philo);
		first = philo->left_fork;
		i++;
	}
	if (!*y && !pthread_mutex_lock(second))
	{
		pti_printf("%ld %d has taken a fork\n", philo);
		second = philo->right_fork;
		y++;
	}
	choose_direction_unlock(philo, *i, *y);
	return (1);
}

void	choose_direction_unlock(t_philo *philo, int i, int y)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	
  (void)i;
	(void)y;
	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	pti_printf("%ld %d is eating\n", philo);
	usleep(philo->rules->time_to_eat * 1000);
	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_meal);
  pthread_mutex_lock(&philo->mutex_a);
  ++philo->a;
  pthread_mutex_unlock(&philo->mutex_a);
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
}

void	*ft_routine(void *arg)
{
	t_philo	*philo;
	int		i;
	int		y;
	int		a;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_meal);
	while (check_if_dead(philo))
	{
		y = -1;
		i = -1;
		a = 0;
		if (philo->id % 2 == 0)
			usleep(2000);
		i = 0;
		y = 0;
		if (!a)
			a = choose_direction(philo, &i, &y);
		if (check_if_dead(philo))
		{
			pti_printf("%ld %d is sleeping\n", philo);
			usleep(philo->rules->time_to_sleep * 1000);
		}
		if (check_if_dead(philo))
			pti_printf("%ld %d is thinking\n", philo);
	}
	return (NULL);
}
