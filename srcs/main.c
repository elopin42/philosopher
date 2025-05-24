/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:00:18 by elopin            #+#    #+#             */
/*   Updated: 2025/05/21 19:10:10 by elopin           ###   ########.fr       */
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

void	*ft_thread_death(void *arg)
{
	t_global	*glb;
	long		laps;
	int			i;
  int a_val;

  a_val = 0;
	glb = (t_global *)arg;
	while (1)
	{
		i = -1;
		while (++i < glb->nbr_philo)
		{
			pthread_mutex_lock(&glb->philo[i].mutex_meal);
			laps = (glb->philo[i].last_meal - get_time_in_ms()) * -1;
			pthread_mutex_unlock(&glb->philo[i].mutex_meal);
			//printf("laps --> %ld, time --> %d\n", laps, glb->rules.time_to_die);
			if (laps >= glb->rules.time_to_die)
			{
				pthread_mutex_lock(&glb->death_mu);
				glb->death = 0;
				pthread_mutex_unlock(&glb->death_mu);
				pti_printf("%ld %d died\n", glb->philo);
				return (NULL);
			}
      pthread_mutex_lock(&glb->philo[i].mutex_a);
      a_val = glb->philo[i].a;
      pthread_mutex_unlock(&glb->philo[i].mutex_a);
			if (glb->rules.loop > 0 && a_val >= glb->rules.loop)
			{
				pthread_mutex_lock(&glb->death_mu);
				glb->death = 0;
				pthread_mutex_unlock(&glb->death_mu);
				printf("%d loop\n", glb->rules.loop);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_global	glb;
	int			i;
	pthread_t	*threads;

	i = -1;
	if (!ft_init_before_play(&glb, ac, av))
		return (printf("Error\n"), 1);
	if (glb.nbr_philo == 1)
		return (fake_routine(&glb), 0);
	threads = malloc(sizeof(pthread_t) * glb.rules.nb_philo);
	if (!threads)
		return (1);
	pthread_create(&glb.thread_death, NULL, ft_thread_death, &glb);
	glb.start = get_time_in_ms();
	while (++i < glb.rules.nb_philo)
		pthread_create(&threads[i], NULL, ft_routine, &glb.philo[i]);
	i = -1;
	while(++i < glb.rules.nb_philo)
	{
		pthread_join(threads[i], NULL);
		//printf("threads[%d]\n", i);
	}
	pthread_join(glb.thread_death, NULL);
	free(threads);
	ft_clean_up(&glb);
	return (0);
}
