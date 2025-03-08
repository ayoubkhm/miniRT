/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_inter2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:02:10 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 19:02:47 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

bool	valid_cylinder_candidate(t_ray ray, t_cylinder *cylinder,
			t_vec axis, double t_candidate)
{
	double	projection;
	t_vec	hit_point;

	if (t_candidate < EPSILON)
	{
		return (false);
	}
	hit_point = vector_add(ray.origin, scale_vec(ray.direction, t_candidate));
	projection = scalar_dot(vector_sub(hit_point, cylinder->base), axis);
	return (projection >= 0 && projection <= cylinder->height);
}

double	cylinder_lateral_intersection(t_vec ray_origin, t_vec ray_dir,
			t_cylinder *cylinder, t_vec axis)
{
	t_vec	delta_p;
	t_vec	v;
	double	t0;
	double	t1;
	double	t_val;

	delta_p = vector_sub(ray_origin, cylinder->base);
	v = vector_sub(ray_dir, scale_vec(axis, scalar_dot(ray_dir, axis)));
	delta_p = vector_sub(delta_p, scale_vec(axis, scalar_dot(delta_p, axis)));
	if (!solve_quadratic((t_quad){scalar_dot(v, v),
			2.0 * scalar_dot(v, delta_p), scalar_dot(delta_p, delta_p)
			- (cylinder->radius * cylinder->radius)}, &t0, &t1))
	{
		return (INFINITY);
	}
	t_val = INFINITY;
	if (valid_cylinder_candidate((t_ray){ray_origin, ray_dir},
		cylinder, axis, t0))
		t_val = t0;
	if (valid_cylinder_candidate((t_ray){ray_origin, ray_dir},
		cylinder, axis, t1) && t1 < t_val)
		t_val = t1;
	return (t_val);
}

double	cylinder_cap_intersection(t_ray ray, t_vec cap_center,
			double radius, t_vec axis)
{
	double	denom;
	double	t_candidate;
	double	dist;
	t_vec	hit_point;

	denom = scalar_dot(ray.direction, axis);
	if (fabs(denom) < EPSILON)
	{
		return (INFINITY);
	}
	t_candidate = scalar_dot(vector_sub(cap_center, ray.origin), axis) / denom;
	if (t_candidate < EPSILON)
	{
		return (INFINITY);
	}
	hit_point = vector_add(ray.origin, scale_vec(ray.direction, t_candidate));
	dist = vector_length(vector_sub(hit_point, cap_center));
	if (dist <= radius)
	{
		return (t_candidate);
	}
	return (INFINITY);
}

int	choose_cylinder_candidate(double t_lateral, double t_cap_bottom,
			double t_cap_top, double *t_val)
{
	double	min_val;

	min_val = fmin(t_lateral, fmin(t_cap_bottom, t_cap_top));
	*t_val = min_val;
	if (min_val == INFINITY)
	{
		return (-1);
	}
	if (fabs(min_val - t_lateral) < EPSILON)
	{
		return (0);
	}
	else if (fabs(min_val - t_cap_bottom) < EPSILON)
	{
		return (1);
	}
	else
	{
		return (2);
	}
}
