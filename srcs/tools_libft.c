/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_libft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:33:42 by elopin            #+#    #+#             */
/*   Updated: 2025/05/21 13:15:24 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>

void	pti_printf(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->print_mutex);
	if (check_if_dead(philo))
		printf(str, get_time_in_ms() - philo->start, philo->id + 1);
	else if (ft_strnstr(str, "died", ft_strlen(str)))
		printf(str, get_time_in_ms() - philo->start, philo->id + 1);
	pthread_mutex_unlock(philo->print_mutex);
}
int		ft_strnstr(const char	*b, const char	*li, size_t	l)
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

int ft_strlen(char *str)
{
	int	i;
	
	i = 0;
	while(str[i])
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	int	ret;
	int	sign;
	int	i;

	if (!str || str[0] == '\0')
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
