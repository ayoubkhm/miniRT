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

double	ft_atof(const char *str)
{
	double	result;
	double	fraction;
	int		sign;
	int		i;

	result = 0.0;
	fraction = 0.0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '-') ? -1 : 1;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	if (str[i] == '.')
	{
		i++;
		double divisor = 10;
		while (str[i] && str[i] >= '0' && str[i] <= '9')
		{
			fraction += (str[i++] - '0') / divisor;
			divisor *= 10;
		}
	}
	return sign * (result + fraction);
}
