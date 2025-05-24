/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:46:40 by elopin            #+#    #+#             */
/*   Updated: 2025/05/20 12:10:24 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <pthread.h>

void	ft_clean_up(t_global *glb)
{
	int	i;

	i = -1;
	while(++i < glb->nbr_philo)
	{
		pthread_mutex_destroy(&glb->philo[i].mutex_meal);
		pthread_mutex_destroy(&glb->fork[i]);
    pthread_mutex_destroy(&glb->philo[i].mutex_a);
	}
	pthread_mutex_destroy(&glb->mutex_meal);
	pthread_mutex_destroy(&glb->death_mu);
	free(glb->fork);
	free(glb->philo);
	i = -1;
}
