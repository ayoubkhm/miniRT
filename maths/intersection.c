/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:02:45 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:07:10 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

bool	compute_plane_intersection(t_vec ray_origin, t_vec ray_dir,
		t_plane *plane, double *t_val)
{
	double	denom;

	denom = scalar_dot(plane->normal, ray_dir);
	if (fabs(denom) > EPSILON)
	{
		*t_val = scalar_dot(vector_sub(
					plane->point, ray_origin), plane->normal) / denom;
		if (*t_val > EPSILON)
			return (true);
	}
	return (false);
}

void	set_hit_info_plane(t_hit *hit, t_plane *plane,
			t_ray ray, double t_val)
{
	hit->t = t_val;
	hit->point = vector_add(ray.origin, scale_vec(ray.direction, t_val));
	hit->normal = plane->normal;
	hit->material.color = plane->color;
}

void	compute_plane_uv(t_plane *plane, t_hit *hit)
{
	double	u;
	double	v;

	u = (hit->point.x - plane->point.x) * 0.1;
	v = (hit->point.z - plane->point.z) * 0.1;
	u = u - floor(u);
	v = v - floor(v);
	hit->uv = (t_vec){u, v, 0};
}

bool	intersect_plane(t_vec ray_origin, t_vec ray_dir,
			t_plane *plane, t_hit *hit)
{
	double	t_val;
	t_ray	ray;

	ray = (t_ray){ray_origin, ray_dir};
	if (!compute_plane_intersection(ray_origin, ray_dir, plane, &t_val))
		return (false);
	set_hit_info_plane(hit, plane, ray, t_val);
	compute_plane_uv(plane, hit);
	return (true);
}

bool	intersect_object(t_ray ray, t_object *object, t_hit *hit)
{
	int	is_hit;

	is_hit = 0;
	if (object->type == SPHERE)
		is_hit = intersect_sphere(ray.origin, ray.direction,
				(t_sphere *)object->data, hit);
	else if (object->type == PLANE)
		is_hit = intersect_plane(ray.origin, ray.direction,
				(t_plane *)object->data, hit);
	else if (object->type == CYLINDER)
		is_hit = intersect_cylinder(ray.origin, ray.direction,
				(t_cylinder *)object->data, hit);
	else if (object->type == HYPERBOLOID)
		is_hit = intersect_hyperboloid(ray.origin, ray.direction,
				(t_hyperboloid *)object->data, hit);
	else if (object->type == PARABOLOID)
		is_hit = intersect_paraboloid(ray.origin, ray.direction,
				(t_paraboloid *)object->data, hit);
	else if (object->type == CONE)
		is_hit = intersect_cone(ray.origin, ray.direction,
				(t_cone *)object->data, hit);
	if (is_hit)
		hit->object = object;
	return (is_hit);
}
