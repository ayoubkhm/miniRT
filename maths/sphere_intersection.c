/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_intersection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:00:03 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:00:04 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

double	choose_sphere(double t0, double t1)
{
	double	valeurs[2];
	double	min;
	int		i;

	valeurs[0] = t0;
	valeurs[1] = t1;
	min = valeurs[0];
	i = 1;
	while (i < 2)
	{
		if (valeurs[i] < min)
		{
			min = valeurs[i];
		}
		i++;
	}
	return (min);
}

void	fill_sphere_hit(t_ray r, t_sphere *s, double t, t_hit *h)
{
	t_vec	p;
	t_vec	np;
	double	phi;
	double	theta;

	p = vector_add(r.origin, scale_vec(r.direction, t));
	h->t = t;
	h->point = p;
	h->normal = vector_normalize(vector_sub(p, s->center));
	np = vector_normalize(vector_sub(p, s->center));
	phi = atan2(-np.z, np.x) + M_PI;
	theta = acos(-np.y);
	h->uv = (t_vec){phi / (2 * M_PI), 1 - theta / M_PI, 0};
	h->material.color = s->color;
	h->material.reflectivity = 0;
	h->material.refractive_index = 1;
}

bool	intersect_sphere(t_vec ro, t_vec rd, t_sphere *s, t_hit *h)
{
	t_ray	r;
	t_vec	oc;
	t_quad	q;
	double	ts[2];
	double	t;

	r = (t_ray){ro, rd};
	oc = vector_sub(ro, s->center);
	q.a = scalar_dot(r.direction, r.direction);
	q.b = 2 * scalar_dot(oc, r.direction);
	q.c = scalar_dot(oc, oc) - s->radius * s->radius;
	if (!solve_quadratic(q, &ts[0], &ts[1]))
		return (false);
	t = choose_sphere(ts[0], ts[1]);
	if (t < EPSILON)
		return (false);
	fill_sphere_hit(r, s, t, h);
	return (true);
}
