/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:31:12 by elopin            #+#    #+#             */
/*   Updated: 2025/06/05 16:30:13 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void	update_meal_count(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_a);
	philo->a++;
	if (philo->a == philo->rules->loop)
	{
		pthread_mutex_lock(&philo->rules->glb_ptr->mutex_done);
		philo->rules->glb_ptr->nbr_done++;
		pthread_mutex_unlock(&philo->rules->glb_ptr->mutex_done);
	}
	pthread_mutex_unlock(&philo->mutex_a);
}

static void	lock_forks_and_eat(t_philo *philo, pthread_mutex_t *first,
		pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	if (!check_if_dead(philo))
	{
		pthread_mutex_unlock(first);
		return ;
	}
	pti_printf("%ld %d has taken a fork\n", philo);
	pthread_mutex_lock(second);
	if (!check_if_dead(philo))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return ;
	}
	pti_printf("%ld %d has taken a fork\n", philo);
	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_meal);
	pti_printf("%ld %d is eating\n", philo);
	ft_usleep(philo->rules->time_to_eat, philo);
	update_meal_count(philo);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

void	eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (!check_if_dead(philo))
		return ;
	if (philo->id % 2 == 0)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	lock_forks_and_eat(philo, first, second);
}

static int	check_loop_condition(t_philo *philo)
{
	if (philo->rules->loop > 0)
	{
		pthread_mutex_lock(&philo->mutex_a);
		if (philo->a >= philo->rules->loop)
		{
			pthread_mutex_unlock(&philo->mutex_a);
			return (0);
		}
		pthread_mutex_unlock(&philo->mutex_a);
	}
	return (1);
}

void	*ft_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_meal);
	if (philo->id % 2 == 0)
		ft_usleep(philo->rules->time_to_eat / 2, philo);
	while (check_if_dead(philo))
	{
		if (!check_loop_condition(philo))
			break ;
		eat(philo);
		if (!check_if_dead(philo))
			return (NULL);
		pti_printf("%ld %d is sleeping\n", philo);
		ft_usleep(philo->rules->time_to_sleep, philo);
		if (!check_if_dead(philo))
			return (NULL);
		pti_printf("%ld %d is thinking\n", philo);
		if (philo->rules->nb_philo % 2 == 1)
			ft_usleep(1, philo);
	}
	return (NULL);
}
