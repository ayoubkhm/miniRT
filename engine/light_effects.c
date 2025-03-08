/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_effects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:06:46 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 04:06:51 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#define SHADOW_SAMPLES 50
#define RANDOM_OFFSET_RANGE 0.2

static t_vec	random_offset(double range)
{
	t_vec	off;

	off.x = (((double)rand() / RAND_MAX) - 0.5) * range;
	off.y = (((double)rand() / RAND_MAX) - 0.5) * range;
	off.z = (((double)rand() / RAND_MAX) - 0.5) * range;
	return (off);
}

static int	is_shadowed(t_scene *scene, t_ray ray, double light_distance)
{
	t_list		*obj;
	int			count;
	t_object	*object;
	t_hit		hit;

	obj = scene->objects;
	count = scene->object_count;
	while (count > 0)
	{
		object = obj->content;
		if (intersect_object(ray, object, &hit) && hit.t
			> EPSILON && hit.t < light_distance)
		{
			return (1);
		}
		obj = obj->next;
		count = count - 1;
	}
	return (0);
}

static int	shadow_sample(t_scene *scene, t_hit hit, t_light *light)
{
	t_ray	ray;
	t_vec	offset;
	t_vec	light_pos;
	double	distance;

	ray.origin = vector_add(hit.point, scale_vec(hit.normal, EPSILON));
	offset = random_offset(RANDOM_OFFSET_RANGE);
	light_pos = vector_add(light->position, offset);
	ray.direction = vector_normalize(vector_sub(light_pos, hit.point));
	distance = vector_length(vector_sub(light_pos, hit.point));
	return (is_shadowed(scene, ray, distance));
}

double	compute_shadow(t_scene *scene, t_hit hit, t_light *light)
{
	int		s;
	int		hits;
	double	intensity;

	s = 0;
	hits = 0;
	while (s < SHADOW_SAMPLES)
	{
		hits = hits + shadow_sample(scene, hit, light);
		s = s + 1;
	}
	intensity = 1.0 - ((double)hits / SHADOW_SAMPLES);
	return (fmax(intensity, 0.2));
}

double	compute_diffuse_single(t_scene *scene, t_hit hit, t_light *light)
{
	t_vec	light_dir;
	double	diff;

	(void)scene;
	light_dir = vector_normalize(vector_sub(light->position, hit.point));
	diff = scalar_dot(hit.normal, light_dir);
	if (diff < 0.1)
	{
		diff = 0;
	}
	else
	{
		diff = diff * light->ratio;
	}
	return (diff);
}

double	compute_specular_single(t_scene *scene, t_hit hit,
			t_ray ray, t_light *light)
{
	t_vec	light_dir;
	t_vec	view_dir;
	t_vec	reflect_dir;
	double	spec;
	double	exponent;

	light_dir = vector_normalize(vector_sub(light->position, hit.point));
	view_dir = vector_normalize(scale_vec(ray.direction, -1));
	reflect_dir = reflect(scale_vec(light_dir, -1), hit.normal);
	spec = scalar_dot(view_dir, reflect_dir);
	(void)scene;
	if (spec > 0)
	{
		exponent = hit.material.specular_exponent;
		if (exponent <= 0)
		{
			exponent = 10.0;
		}
		spec = pow(spec, exponent) * light->ratio;
	}
	else
	{
		spec = 0;
	}
	return (spec);
}
