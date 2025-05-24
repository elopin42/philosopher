/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:01:59 by elopin            #+#    #+#             */
/*   Updated: 2025/05/21 19:09:50 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>

bool	ft_create_struct_for_philo(t_global *glb)
{
	int	i;

	i = -1;
	glb->philo = malloc(sizeof(t_philo) * glb->rules.nb_philo);
	if (!glb->philo)
		return (false);
	glb->fork = malloc(sizeof(pthread_mutex_t) * glb->rules.nb_philo);
	if (!glb->fork)
		return (false);
	while (++i < glb->rules.nb_philo)
	{
		pthread_mutex_init(&glb->fork[i], NULL);
		pthread_mutex_init(&glb->philo[i].mutex_meal, NULL);
		glb->philo[i].id = i;
		glb->philo[i].state = 0;
		glb->philo[i].left_fork = &glb->fork[i];
		glb->philo[i].right_fork = &glb->fork[(i + 1) % glb->rules.nb_philo];
		glb->philo[i].rules = &glb->rules;
		glb->philo[i].death = &glb->death;
		glb->philo[i].last_meal = glb->start;
		glb->philo[i].start = glb->start;
		glb->philo[i].print_mutex = &glb->print_mutex;
		glb->philo[i].death_mu = &glb->death_mu;
		glb->philo[i].a = -1;
	}
	glb->nbr_philo = i;
	return (true);
}

bool	ft_init_before_play(t_global *glb, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (printf("argument\n"), false);
	glb->rules.nb_philo = ft_atoi(av[1]);
	glb->rules.time_to_die = ft_atoi(av[2]);
	glb->rules.time_to_eat = ft_atoi(av[3]);
	glb->rules.time_to_sleep = ft_atoi(av[4]);
	glb->death = 1;
	glb->start = get_time_in_ms();
	if (ac == 6)
		glb->rules.loop = ft_atoi(av[5]);
	else
		glb->rules.loop = -1;
	if (glb->rules.nb_philo <= 0 || glb->rules.time_to_eat < 0
		|| glb->rules.time_to_die < 0 || glb->rules.time_to_sleep < 0
		|| (ac == 6 && glb->rules.loop < 0))
		return (printf("bad_value\n"), false);
	pthread_mutex_init(&glb->mutex_meal, NULL);
	pthread_mutex_init(&glb->death_mu, NULL);
	pthread_mutex_init(&glb->print_mutex, NULL);
	ft_create_struct_for_philo(glb);
	return (true);
}
