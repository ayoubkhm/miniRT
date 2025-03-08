/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:01:31 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 19:01:42 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

bool	solve_quadratic(t_quad q, double *t0, double *t1)
{
	double	discriminant;
	double	q_calc;

	discriminant = q.b * q.b - 4 * q.a * q.c;
	if (discriminant < 0)
	{
		return (false);
	}
	discriminant = sqrt(discriminant);
	if (q.b < 0)
	{
		q_calc = -0.5 * (q.b - discriminant);
	}
	else
	{
		q_calc = -0.5 * (q.b + discriminant);
	}
	*t0 = q_calc / q.a;
	*t1 = q.c / q_calc;
	if (*t0 > *t1)
	{
		ft_swap(t0, t1);
	}
	return (true);
}
