/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 10:02:12 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/01 10:02:37 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	skip_whitespace(const char *str, int *i)
{
	while (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n')
		(*i)++;
}

static int	parse_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-')
	{
		sign = -1;
		(*i)++;
	}
	else if (str[*i] == '+')
	{
		sign = 1;
		(*i)++;
	}
	return (sign);
}

static double	parse_integer(const char *str, int *i)
{
	double	result;

	result = 0.0;
	while (str[*i] && str[*i] >= '0' && str[*i] <= '9')
	{
		result = result * 10 + (str[*i] - '0');
		(*i)++;
	}
	return (result);
}

static double	parse_fraction(const char *str, int *i)
{
	double	fraction;
	double	divisor;

	fraction = 0.0;
	divisor = 10.0;
	if (str[*i] == '.')
	{
		(*i)++;
		while (str[*i] && str[*i] >= '0' && str[*i] <= '9')
		{
			fraction += (str[*i] - '0') / divisor;
			divisor *= 10.0;
			(*i)++;
		}
	}
	return (fraction);
}

double	ft_atof(const char *str)
{
	int		i;
	double	integer_part;
	double	fraction_part;
	int		sign;

	i = 0;
	skip_whitespace(str, &i);
	sign = parse_sign(str, &i);
	integer_part = parse_integer(str, &i);
	fraction_part = parse_fraction(str, &i);
	return (sign * (integer_part + fraction_part));
}
