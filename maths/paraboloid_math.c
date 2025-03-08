/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid_math.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:01:37 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:01:38 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

t_parabcoeff	compute_parab_coeff(t_ray l, double p)
{
	t_parabcoeff	cp;

	cp.A = l.direction.x * l.direction.x + l.direction.y * l.direction.y;
	cp.B = 2 * (l.origin.x * l.direction.x
			+ l.origin.y * l.direction.y) - 2 * p * l.direction.z;
	cp.C = l.origin.x * l.origin.x
		+ l.origin.y * l.origin.y - 2 * p * l.origin.z;
	return (cp);
}

bool	solve_parab_quad(double A, double B, double C, double *t)
{
	double	disc;
	double	sd;
	double	t0;
	double	t1;

	disc = B * B - 4 * A * C;
	if (disc < 0)
		return (false);
	sd = sqrt(disc);
	t0 = (-B - sd) / (2 * A);
	t1 = (-B + sd) / (2 * A);
	if (t0 > EPSILON)
		*t = t0;
	else
		*t = t1;
	if (*t >= EPSILON)
		return (true);
	else
		return (false);
}

void	compute_parab_hit_geometry(t_parabint *I, double t,
		t_vec *point, t_vec *normal)
{
	t_vec	lp;
	t_vec	wp;
	t_vec	ln;

	lp = vector_add(I->r.origin, scale_vec(I->r.direction, t));
	if (I->tr.ang != 0)
		wp = vector_add(I->parab->base,
				rodrigues_rotate(lp, I->tr.rot, -I->tr.ang));
	else
		wp = vector_add(I->parab->base, lp);
	*point = wp;
	ln.x = 2 * lp.x;
	ln.y = 2 * lp.y;
	ln.z = -2 * I->p;
	ln = vector_normalize(ln);
	if (I->tr.ang != 0)
		*normal = vector_normalize(rodrigues_rotate(ln, I->tr.rot, -I->tr.ang));
	else
		*normal = vector_normalize(ln);
}

void	set_parab_hit(t_parabint *I, double t, t_hit *h)
{
	t_vec	point;
	t_vec	normal;
	t_vec	zero;

	compute_parab_hit_geometry(I, t, &point, &normal);
	h->t = t;
	h->point = point;
	h->normal = normal;
	h->material.color = I->parab->color;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	h->uv = zero;
	h->material.reflectivity = 0;
	h->material.transparency = 0;
	h->material.refractive_index = 1;
}

bool	intersect_paraboloid(t_vec ro, t_vec rd, t_paraboloid *p, t_hit *hit)
{
	t_ray			lr;
	t_trans			tr;
	t_parabcoeff	cp;
	double			t;
	t_parabint		info;

	transform_parab_ray((t_ray){ro, rd}, p, &lr, &tr);
	cp = compute_parab_coeff(lr, p->p);
	if (!solve_parab_quad(cp.A, cp.B, cp.C, &t))
		return (false);
	info = (t_parabint){lr, p, p->p, tr};
	set_parab_hit(&info, t, hit);
	return (true);
}
