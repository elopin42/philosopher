/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_libft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:33:42 by elopin            #+#    #+#             */
/*   Updated: 2025/06/02 22:51:51 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>

int	check_long(const char *ts)
{
	if (ts[0] == '-')
	{
		if (ft_strlen((char *)ts) > 11)
			return (0);
	}
	else if (ft_strlen((char *)ts) > 10)
		return (0);
	return (1);
}

void	pti_printf(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->print_mutex);
	pthread_mutex_lock(philo->death_mu);
	if (*philo->death || ft_strnstr(str, "died", ft_strlen(str)))
		printf(str, get_time_in_ms() - philo->start, philo->id + 1);
	pthread_mutex_unlock(philo->death_mu);
	pthread_mutex_unlock(philo->print_mutex);
}
int	ft_strnstr(const char *b, const char *li, size_t l)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*li)
		return (0);
	while (b[i])
	{
		j = 0;
		while (li[j] == b[i] && b[i] && i < l)
		{
			i++;
			j++;
		}
		if (!li[j])
			return (j);
		i -= j;
		i++;
	}
	return (0);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int	ret;
	int	sign;
	int	i;

	if (!str || str[0] == '\0' || !check_long(str))
		return (-1);
	i = 0;
	ret = 0;
	sign = 1;
	while (*str == '\t' || *str == '\f' || *str == '\v' || *str == '\r'
		|| *str == '\n' || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			return (-1);
		str++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
		ret = 10 * ret + (str[i++] - '0');
	if (str[i] != '\0')
		return (-1);
	return (ret * sign);
}

/*void	ft_usleep(long duration_ms, t_philo *philo)
{
	long	start;
	long	now;

	start = get_time_in_ms();
	while (1)
	{
		now = get_time_in_ms();
		if (now - start >= duration_ms)
			break ;
		if (!check_if_dead(philo))
			break ;
		usleep(500);
	}
}*/

void ft_usleep(long duration_ms, t_philo *philo)
{
    long start;
    long now;
    long remaining;

    start = get_time_in_ms();
    while (1)
    {
        now = get_time_in_ms();
        remaining = duration_ms - (now - start);
        
        if (remaining <= 0)
            break;
            
        if (!check_if_dead(philo))
            break;
            
        // Sleep plus court pour plus de précision
        if (remaining > 10)
            usleep(1000); // 1ms
        else
            usleep(100);  // 0.1ms pour la fin
    }
}
