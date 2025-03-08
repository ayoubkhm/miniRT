/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:31:55 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 04:32:02 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec	reflect(t_vec I, t_vec N)
{
	double	dot_value;
	t_vec	result;

	dot_value = scalar_dot(I, N);
	result = vector_sub(I, scale_vec(N, 2.0 * dot_value));
	return (result);
}

int	refract(t_vec I, t_vec N, double eta, t_vec *refracted)
{
	double	cosi;
	double	cost2;

	cosi = -scalar_dot(I, N);
	cost2 = 1.0 - eta * eta * (1.0 - cosi * cosi);
	if (cost2 < 0)
		return (0);
	*refracted = vector_normalize(vector_add(scale_vec(I, eta),
				scale_vec(N, eta * cosi - sqrt(cost2))));
	return (1);
}
