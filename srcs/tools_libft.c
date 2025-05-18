/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_libft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopin <elopin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:33:42 by elopin            #+#    #+#             */
/*   Updated: 2025/05/17 15:10:35 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	ft_atoi(const char *str)
{
	int	ret;
	int	sign;
	int	i;

	i = 0;
	ret = 0;
	sign = 1;
	while (*str == '\t' || *str == '\f' || *str == '\v' || *str == '\r'
		|| *str == '\n' || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
		ret = 10 * ret + (str[i++] - '0');
	return (ret * sign);
}
