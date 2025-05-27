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

/*int choose_direction(t_philo *philo, int *i, int *y)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;

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

	while (check_if_dead(philo) && (!*i || !*y))
	{
		if (!*i && !pthread_mutex_lock(first))
		{
			pti_printf("%ld %d has taken a fork\n", philo);
			(*i)++;
		}
		if (!*y && !pthread_mutex_lock(second))
		{
			pti_printf("%ld %d has taken a fork\n", philo);
			(*y)++;
		}
		usleep(50); // éviter CPU 100% en cas de blocage
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
	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_meal);
	pti_printf("%ld %d is eating\n", philo);
	usleep(philo->rules->time_to_eat * 1000);
  pthread_mutex_lock(&philo->mutex_a);
  ++philo->a;
  if (philo->a == philo->rules->loop)
  {
	  pthread_mutex_lock(&philo->rules->glb_ptr->mutex_done);
	  philo->rules->glb_ptr->nbr_done++;
	  pthread_mutex_unlock(&philo->rules->glb_ptr->mutex_done);
  }
  pthread_mutex_unlock(&philo->mutex_a);
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
}*/
void	eat(t_philo *philo)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;

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

	pthread_mutex_lock(first);
	pti_printf("%ld %d has taken a fork\n", philo);
	pthread_mutex_lock(second);
	pti_printf("%ld %d has taken a fork\n", philo);

	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_meal);

	pti_printf("%ld %d is eating\n", philo);
	usleep(philo->rules->time_to_eat * 1000);

	pthread_mutex_lock(&philo->mutex_a);
	philo->a++;
	if (philo->a == philo->rules->loop - 1)
	{
		pthread_mutex_lock(&philo->rules->glb_ptr->mutex_done);
		philo->rules->glb_ptr->nbr_done++;
		pthread_mutex_unlock(&philo->rules->glb_ptr->mutex_done);
	}
	pthread_mutex_unlock(&philo->mutex_a);

	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
}

void	*ft_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->mutex_meal);

	if (philo->id % 2 == 0)
		usleep(philo->rules->time_to_eat * 500);

	while (check_if_dead(philo))
	{
    if (philo->rules->loop > 0)
	  {
		  pthread_mutex_lock(&philo->mutex_a);
		  if (philo->a >= philo->rules->loop - 1)
		  {
			  pthread_mutex_unlock(&philo->mutex_a);
			  break;
		  }
		  pthread_mutex_unlock(&philo->mutex_a);
	  }
		eat(philo);
		if (!check_if_dead(philo)) break;
		pti_printf("%ld %d is sleeping\n", philo);
		usleep(philo->rules->time_to_sleep * 1000);
		if (!check_if_dead(philo)) break;
		pti_printf("%ld %d is thinking\n", philo);
	}
	return (NULL);
}

