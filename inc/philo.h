/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:07:58 by elopin            #+#    #+#             */
/*   Updated: 2025/05/18 18:17:23 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int	loop;
}					t_rules;

typedef struct s_philo
{
	int				id;
	int				state;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*rules;
	int				*death;
	long start;
	long last_meal;
	pthread_mutex_t mutex_meal;
}					t_philo;

typedef struct s_global
{
	t_philo			*philo;
	pthread_mutex_t	*fork;
	t_rules			rules;
	pthread_t		thread_death;
	int				death;
	pthread_mutex_t	mutex_meal;
	int				nbr_philo;
	pthread_mutex_t death_mu;
	long start;
}					t_global;

long	get_time_in_ms(void);
bool				ft_init_before_play(t_global *glb, int ac, char **av);
int					ft_atoi(const char *str);
void				*ft_routine(void *arg);
void				fake_routine(t_global *glb);
#endif
