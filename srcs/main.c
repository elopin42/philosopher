/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:00:18 by elopin            #+#    #+#             */
/*   Updated: 2025/06/02 23:15:52 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>
#include <stdio.h>

void	fake_routine(t_global *glb)
{
	printf("%ld %d has taken a fork\n", get_time_in_ms() - glb->start, 1);
	usleep(glb->rules.time_to_die * 1000);
	printf("%ld %d died\n", get_time_in_ms() - glb->start, 1);
}

static int	check_death_or_done(t_global *glb, int i)
{
	long	laps;

	pthread_mutex_lock(&glb->philo[i].mutex_meal);
	laps = get_time_in_ms() - glb->philo[i].last_meal;
	pthread_mutex_unlock(&glb->philo[i].mutex_meal);
	if (laps >= glb->rules.time_to_die)
	{
		pthread_mutex_lock(&glb->death_mu);
		glb->death = 0;
		pthread_mutex_unlock(&glb->death_mu);
		pti_printf("%ld %d died\n", &glb->philo[i]);
		return (1);
	}
	pthread_mutex_lock(&glb->mutex_done);
	if (glb->nbr_done == glb->nbr_philo)
	{
		pthread_mutex_lock(&glb->death_mu);
		glb->death = 0;
		pthread_mutex_unlock(&glb->death_mu);
		pthread_mutex_unlock(&glb->mutex_done);
		printf("%d loop\n", glb->rules.loop);
		return (1);
	}
	pthread_mutex_unlock(&glb->mutex_done);
	return (0);
}

void	*ft_thread_death(void *arg)
{
	t_global	*glb;
	int			i;

	glb = (t_global *)arg;
	while (1)
	{
		i = -1;
		while (++i < glb->nbr_philo)
		{
			if (check_death_or_done(glb, i))
				return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}

static void	handle_threads(t_global *glb)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(pthread_t) * glb->rules.nb_philo);
	if (!threads)
		return ;
	pthread_create(&glb->thread_death, NULL, ft_thread_death, glb);
	glb->start = get_time_in_ms();
	i = -1;
	while (++i < glb->rules.nb_philo)
	{
		pthread_create(&threads[i], NULL, ft_routine, &glb->philo[i]);
		if (i % 2 == 0)
			usleep(200);
		else
			usleep(150);
	}
	i = -1;
	while (++i < glb->rules.nb_philo)
		pthread_join(threads[i], NULL);
	pthread_join(glb->thread_death, NULL);
	free(threads);
}

int	main(int ac, char **av)
{
	t_global	glb;

	if (!ft_init_before_play(&glb, ac, av))
		return (printf("Error\n"), 1);
	if (glb.nbr_philo == 1)
		return (fake_routine(&glb), ft_clean_up(&glb), 0);
	handle_threads(&glb);
	ft_clean_up(&glb);
	return (0);
}
