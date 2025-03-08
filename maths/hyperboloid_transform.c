/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid_transform.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:07:58 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:07:59 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

void	compute_hyp_transform_params(t_hyperboloid *h,
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

void	transform_hyp_ray(t_ray r, t_hyperboloid *h, t_ray *l, t_trans *tr)
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
