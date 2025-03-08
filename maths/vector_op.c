/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:59:56 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 19:59:58 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mini_maths.h"
#define EPSILON 1e-6

t_vec	vector_sub(t_vec a, t_vec b)
{
	return ((t_vec){a.x - b.x, a.y - b.y, a.z - b.z});
}

t_vec	vector_normalize(t_vec v)
{
	double	mag;

	mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (mag < EPSILON)
		return ((t_vec){0, 0, 0});
	return ((t_vec){v.x / mag, v.y / mag, v.z / mag});
}

double	scalar_dot(t_vec a, t_vec b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec	vector_add(t_vec a, t_vec b)
{
	return ((t_vec){a.x + b.x, a.y + b.y, a.z + b.z});
}

t_vec	scale_vec(t_vec a, double k)
{
	return ((t_vec){k * a.x, k * a.y, k * a.z});
}
