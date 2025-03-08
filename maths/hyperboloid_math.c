/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid_math.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:07:55 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:07:56 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

t_quad	compute_hyp_coeffs(t_ray l, double a, double c)
{
	t_quad	co;

	co.a = (l.direction.x * l.direction.x + l.direction.y * l.direction.y) / a
		- (l.direction.z * l.direction.z) / c;
	co.b = 2 * ((l.origin.x * l.direction.x + l.origin.y * l.direction.y) / a
			- (l.origin.z * l.direction.z) / c);
	co.c = (l.origin.x * l.origin.x + l.origin.y * l.origin.y) / a
		- (l.origin.z * l.origin.z) / c - 1;
	return (co);
}

bool	solve_hyp_quad(double A, double B, double C, double *t)
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
	{
		if (t1 > EPSILON)
			*t = t1;
		else
			*t = -1;
	}
	if (*t >= EPSILON)
		return (true);
	else
		return (false);
}

t_vec	compute_hyp_hit_point(t_hypint *I, t_vec lp)
{
	t_vec	wp;

	if (I->tr.ang != 0)
		wp = vector_add(I->hyp->base, rodrigues_rotate(
					lp, I->tr.rot, -I->tr.ang));
	else
		wp = vector_add(I->hyp->base, lp);
	return (wp);
}

t_vec	compute_hyp_hit_normal(t_hypint *I, t_vec lp)
{
	t_vec	ln;
	t_vec	norm;

	ln.x = 2 * lp.x / I->a;
	ln.y = 2 * lp.y / I->a;
	ln.z = -2 * lp.z / I->c;
	ln = vector_normalize(ln);
	if (I->tr.ang != 0)
		norm = vector_normalize(rodrigues_rotate(ln, I->tr.rot, -I->tr.ang));
	else
		norm = vector_normalize(ln);
	return (norm);
}

void	set_hyp_hit(t_hypint *I, double t, t_hit *h)
{
	t_vec	lp;
	t_vec	wp;
	t_vec	norm;
	t_vec	zero;

	lp = vector_add(I->local.origin, scale_vec(I->local.direction, t));
	wp = compute_hyp_hit_point(I, lp);
	h->t = t;
	h->point = wp;
	norm = compute_hyp_hit_normal(I, lp);
	h->normal = norm;
	h->material.color = I->hyp->color;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	h->uv = zero;
}
