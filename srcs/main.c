/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:00:18 by elopin            #+#    #+#             */
/*   Updated: 2025/05/18 18:17:52 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>

void	*ft_thread_death(void *arg)
{
	t_global	*glb;
	int	i;
	long	laps;

	i = -1;
	laps = 0;
	glb = (t_global *)arg;
	while(++i < glb->nbr_philo)
	{
		pthread_mutex_lock(&glb->mutex_meal);
		laps = get_time_in_ms() - glb->philo[i].last_meal;
		pthread_mutex_unlock(&glb->mutex_meal);
		if (laps >= glb->rules.time_to_die)
		{
			pthread_mutex_lock(&glb->death_mu);
			glb->death = 0;
			pthread_mutex_unlock(&glb->death_mu);
			printf("%ld %d died\n", get_time_in_ms() - glb->start,glb->philo[i].id);
			break;
		}
		if (i == glb->nbr_philo - 1)
			i = -1;
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
	pthread_join(glb.thread_death, NULL);
	return (0);
}

