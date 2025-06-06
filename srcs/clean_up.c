/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:46:40 by elopin            #+#    #+#             */
/*   Updated: 2025/06/02 23:20:38 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>

void	ft_clean_up(t_global *glb)
{
	int	i;

	i = -1;
	while (++i < glb->nbr_philo)
	{
		pthread_mutex_destroy(&glb->philo[i].mutex_meal);
		pthread_mutex_destroy(&glb->fork[i]);
		pthread_mutex_destroy(&glb->philo[i].mutex_a);
	}
	pthread_mutex_destroy(&glb->mutex_meal);
	pthread_mutex_destroy(&glb->death_mu);
	pthread_mutex_destroy(&glb->print_mutex);
	pthread_mutex_destroy(&glb->mutex_done);
	free(glb->fork);
	free(glb->philo);
	i = -1;
}

void	ft_usleep(long duration_ms, t_philo *philo)
{
	long	start;
	long	now;
	long	remaining;

	start = get_time_in_ms();
	while (1)
	{
		now = get_time_in_ms();
		remaining = duration_ms - (now - start);
		if (remaining <= 0)
			break ;
		if (!check_if_dead(philo))
			break ;
		if (remaining > 10)
			usleep(1000);
		else
			usleep(100);
	}
}

bool	check_if_dead(t_philo *philo)
{
	bool	is_alive;

	pthread_mutex_lock(philo->death_mu);
	is_alive = (*philo->death != 0);
	pthread_mutex_unlock(philo->death_mu);
	return (is_alive);
}

long	get_time_in_ms(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}
