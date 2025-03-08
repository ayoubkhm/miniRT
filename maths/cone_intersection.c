/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:00:40 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 19:00:57 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

bool	compute_cone_lat_t(t_ray r, t_cone *c, double *t)
{
	double	c2;
	t_quad	q;
	double	ts[2];

	c2 = cos(c->angle) * cos(c->angle);
	q.a = scalar_dot(r.direction, c->axis) * scalar_dot(r.direction, c->axis)
		- c2 * scalar_dot(r.direction, r.direction);
	q.b = 2 * (scalar_dot(r.direction, c->axis)
			* scalar_dot(vector_sub(r.origin, c->vertex), c->axis)
			- c2 * scalar_dot(vector_sub(r.origin, c->vertex), r.direction));
	q.c = scalar_dot(vector_sub(r.origin, c->vertex), c->axis)
		* scalar_dot(vector_sub(r.origin, c->vertex), c->axis)
		- c2 * scalar_dot(vector_sub(r.origin, c->vertex),
			vector_sub(r.origin, c->vertex));
	if (!solve_quadratic(q, &ts[0], &ts[1]))
		return (false);
	if (ts[0] > EPSILON)
		*t = ts[0];
	else
		*t = ts[1];
	if (*t < EPSILON)
		return (false);
	return (true);
}

bool	intersect_cone_lat(t_ray r, t_cone *c, t_hit *h)
{
	double	t;
	t_vec	d;

	if (!compute_cone_lat_t(r, c, &t))
		return (false);
	d = vector_sub(vector_add(r.origin, scale_vec(r.direction, t)), c->vertex);
	if (scalar_dot(d, c->axis) < 0 || scalar_dot(d, c->axis) > c->height)
		return (false);
	h->t = t;
	h->point = vector_add(r.origin, scale_vec(r.direction, t));
	h->normal = vector_normalize(vector_sub(h->point, vector_add(c->vertex,
					scale_vec(c->axis, scalar_dot(d, c->axis)))));
	h->material.color = c->color;
	return (true);
}

bool	intersect_cone_base(t_ray r, t_cone *c, t_hit *h)
{
	t_vec	bc;
	double	denom;
	double	t;
	t_vec	p;

	bc = vector_add(c->vertex, scale_vec(c->axis, c->height));
	denom = scalar_dot(r.direction, c->axis);
	if (fabs(denom) < EPSILON)
		return (false);
	t = scalar_dot(vector_sub(bc, r.origin), c->axis) / denom;
	if (t < EPSILON)
		return (false);
	p = vector_add(r.origin, scale_vec(r.direction, t));
	if (vector_length(vector_sub(p, bc)) > c->height * tan(c->angle))
		return (false);
	h->t = t;
	h->point = p;
	h->normal = scale_vec(vector_normalize(c->axis), -1);
	h->material.color = c->color;
	return (true);
}

bool	intersect_cone(t_vec ro, t_vec rd, t_cone *c, t_hit *h)
{
	t_ray	r;
	t_hit	hl;
	t_hit	hb;
	bool	lat;
	bool	bas;

	r = (t_ray){ro, rd};
	lat = intersect_cone_lat(r, c, &hl);
	bas = intersect_cone_base(r, c, &hb);
	if (!lat && !bas)
		return (false);
	if (lat && bas)
	{
		if (hl.t < hb.t)
			*h = hl;
		else
			*h = hb;
	}
	else if (lat)
		*h = hl;
	else
		*h = hb;
	init_hit(h);
	return (true);
}
