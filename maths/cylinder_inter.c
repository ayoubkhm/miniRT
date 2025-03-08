/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_inter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:59:47 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 19:01:55 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

void	fill_cyl_hit(t_ray r, t_cylinder *c, t_intinfo *info, t_hit *h)
{
	double	p;
	t_vec	ax;
	t_vec	pt;

	h->t = info->t;
	ax = vector_normalize(c->axis);
	pt = vector_add(r.origin, scale_vec(r.direction, info->t));
	h->point = pt;
	if (info->cand == 0)
	{
		p = scalar_dot(vector_sub(pt, c->base), ax);
		h->normal = vector_normalize(vector_sub(
					pt, vector_add(c->base, scale_vec(ax, p))));
	}
	else if (info->cand == 1)
		h->normal = vector_normalize(scale_vec(ax, -1));
	else
		h->normal = ax;
	h->material.color = c->color;
	h->uv = (t_vec){0, 0, 0};
	h->material.reflectivity = 0;
	h->material.transparency = 0;
	h->material.refractive_index = 1;
}

bool	intersect_cylinder(t_vec ro, t_vec rd, t_cylinder *c, t_hit *h)
{
	double		val[4];
	t_vec		ax;
	int			cand;
	t_intinfo	info;
	t_ray		ray;

	ray = (t_ray){ro, rd};
	ax = vector_normalize(c->axis);
	val[0] = cylinder_lateral_intersection(ro, rd, c, ax);
	val[1] = cylinder_cap_intersection(ray, c->base, c->radius, ax);
	val[2] = cylinder_cap_intersection(ray, vector_add(
				c->base, scale_vec(ax, c->height)), c->radius, ax);
	cand = choose_cylinder_candidate(val[0], val[1], val[2], &val[3]);
	if (cand < 0)
	{
		return (false);
	}
	info = (t_intinfo){val[3], cand};
	fill_cyl_hit(ray, c, &info, h);
	return (true);
}
