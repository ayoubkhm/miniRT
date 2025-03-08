/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid_intersection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:07:52 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:07:54 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

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
