/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 18:03:23 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 18:25:50 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

static void	fill_cyl_hit(t_ray r, t_cylinder *c, t_intinfo *info, t_hit *h)
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

static double	choose_sphere(double t0, double t1)
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

static void	fill_sphere_hit(t_ray r, t_sphere *s, double t, t_hit *h)
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

t_vec	rodrigues_rotate(t_vec v, t_vec k, double ang)
{
	t_vec	t1;
	t_vec	t2;
	t_vec	t3;

	t1 = scale_vec(v, cos(ang));
	t2 = scale_vec(vector_cross(k, v), sin(ang));
	t3 = scale_vec(k, scalar_dot(k, v) * (1 - cos(ang)));
	return (vector_add(vector_add(t1, t2), t3));
}

static void	compute_hyp_transform_params(t_hyperboloid *h,
		t_vec *rot, double *ang)
{
	t_vec	k;
	t_vec	d;
	double	dot;

	k = vector_normalize(h->axis);
	d.x = 0;
	d.y = 0;
	d.z = 1;
	dot = scalar_dot(k, d);
	if (fabs(dot - 1) < EPSILON)
	{
		*rot = (t_vec){0, 0, 0};
		*ang = 0;
	}
	else if (fabs(dot + 1) < EPSILON)
	{
		*rot = (t_vec){1, 0, 0};
		*ang = M_PI;
	}
	else
	{
		*rot = vector_normalize(vector_cross(k, d));
		*ang = acos(dot);
	}
}

static void	transform_hyp_ray(t_ray r, t_hyperboloid *h, t_ray *l, t_trans *tr)
{
	t_vec	diff;

	compute_hyp_transform_params(h, &tr->rot, &tr->ang);
	diff = vector_sub(r.origin, h->base);
	if (tr->ang == 0)
	{
		l->origin = diff;
		l->direction = r.direction;
	}
	else
	{
		l->origin = rodrigues_rotate(diff, tr->rot, tr->ang);
		l->direction = rodrigues_rotate(r.direction, tr->rot, tr->ang);
	}
}

static t_quad	compute_hyp_coeffs(t_ray l, double a, double c)
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

static bool	solve_hyp_quad(double A, double B, double C, double *t)
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

static t_vec	compute_hyp_hit_point(t_hypint *I, t_vec lp)
{
	t_vec	wp;

	if (I->tr.ang != 0)
		wp = vector_add(I->hyp->base, rodrigues_rotate(
					lp, I->tr.rot, -I->tr.ang));
	else
		wp = vector_add(I->hyp->base, lp);
	return (wp);
}

static t_vec	compute_hyp_hit_normal(t_hypint *I, t_vec lp)
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

static void	set_hyp_hit(t_hypint *I, double t, t_hit *h)
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

bool	intersect_hyperboloid(t_vec ro, t_vec rd, t_hyperboloid *h, t_hit *hit)
{
	t_ray		lr;
	t_trans		tr;
	t_quad		co;
	double		t;
	t_hypint	info;

	transform_hyp_ray((t_ray){ro, rd}, h, &lr, &tr);
	co = compute_hyp_coeffs(lr, h->radius * h->radius,
			(h->height / 2.0) * (h->height / 2.0));
	if (!solve_hyp_quad(co.a, co.b, co.c, &t))
		return (false);
	info = (t_hypint){lr, h, h->radius * h->radius,
		(h->height / 2.0) * (h->height / 2.0), tr};
	set_hyp_hit(&info, t, hit);
	return (true);
}

static void	compute_parab_transform_params(t_paraboloid *p,
		t_vec *rot, double *ang)
{
	t_vec	k;
	t_vec	d;
	double	dot;

	k = vector_normalize(p->axis);
	d.x = 0;
	d.y = 0;
	d.z = 1;
	dot = scalar_dot(k, d);
	if (fabs(dot - 1) < EPSILON)
	{
		*rot = (t_vec){0, 0, 0};
		*ang = 0;
	}
	else if (fabs(dot + 1) < EPSILON)
	{
		*rot = (t_vec){1, 0, 0};
		*ang = M_PI;
	}
	else
	{
		*rot = vector_normalize(vector_cross(k, d));
		*ang = acos(dot);
	}
}

static void	transform_parab_ray(t_ray r, t_paraboloid *p, t_ray *l, t_trans *tr)
{
	t_vec	diff;

	compute_parab_transform_params(p, &tr->rot, &tr->ang);
	diff = vector_sub(r.origin, p->base);
	if (tr->ang == 0)
	{
		l->origin = diff;
		l->direction = r.direction;
	}
	else
	{
		l->origin = rodrigues_rotate(diff, tr->rot, tr->ang);
		l->direction = rodrigues_rotate(r.direction, tr->rot, tr->ang);
	}
}

static t_parabcoeff	compute_parab_coeff(t_ray l, double p)
{
	t_parabcoeff	cp;

	cp.A = l.direction.x * l.direction.x + l.direction.y * l.direction.y;
	cp.B = 2 * (l.origin.x * l.direction.x
			+ l.origin.y * l.direction.y) - 2 * p * l.direction.z;
	cp.C = l.origin.x * l.origin.x
		+ l.origin.y * l.origin.y - 2 * p * l.origin.z;
	return (cp);
}

static bool	solve_parab_quad(double A, double B, double C, double *t)
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

static void	compute_parab_hit_geometry(t_parabint *I, double t,
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

static void	set_parab_hit(t_parabint *I, double t, t_hit *h)
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

static bool	compute_cone_lat_t(t_ray r, t_cone *c, double *t)
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

static bool	intersect_cone_lat(t_ray r, t_cone *c, t_hit *h)
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

static bool	intersect_cone_base(t_ray r, t_cone *c, t_hit *h)
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

static void	init_hit(t_hit *h)
{
	h->uv = (t_vec){0, 0, 0};
	h->material.reflectivity = 0;
	h->material.transparency = 0;
	h->material.refractive_index = 1;
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
