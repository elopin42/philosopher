/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:31:12 by elopin            #+#    #+#             */
/*   Updated: 2025/05/18 18:27:37 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

long	get_time_in_ms(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

void	*ft_routine(void *arg)
{
	t_philo	*philo;
	int		i;
	int		y;
	long	start;
	int	a;

	a = -1;
	philo = (t_philo *)arg;
	start = get_time_in_ms();
	philo->last_meal = get_time_in_ms();
	while (42 && *philo->death)
	{
		if (philo->rules->loop > 0 && ++a > philo->rules->loop)
			break;
		i = 0;
		y = 0;
		printf("%ld %d sleep\n", get_time_in_ms() - philo->start,philo->id);
		usleep(philo->rules->time_to_sleep * 1000);
		printf("%ld %d think\n", get_time_in_ms() - philo->start,philo->id);
		while (*philo->death && (!i || !y))
		{
			if (*philo->death && !i && !pthread_mutex_lock(philo->left_fork)
				&& ++i)
				printf("%ld %d taken fork\n", get_time_in_ms() - philo->start,philo->id);
			if (*philo->death && !y && !pthread_mutex_lock(philo->right_fork)
				&& ++y)
				printf("%ld %d taken fork\n", get_time_in_ms() - philo->start ,philo->id);
			pthread_mutex_lock(&philo->mutex_meal);
			philo->last_meal = get_time_in_ms();
			pthread_mutex_unlock(&philo->mutex_meal);
		}
		printf("%ld %d eat\n", get_time_in_ms() - philo->start,philo->id);
		usleep(philo->rules->time_to_eat * 1000);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		start = get_time_in_ms();
	}
	return (NULL);
}

void	fake_routine(t_global *glb)
{
	printf("%ld %d think\n", get_time_in_ms() - glb->start,1);
	printf("%ld %d taken fork\n", get_time_in_ms() - glb->start,1);
	usleep(glb->rules.time_to_die * 1000);
	printf("%ld %d die", get_time_in_ms() - glb->start,1);
}
