/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_tracing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:30:59 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 05:32:52 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	compute_reflection(t_scene *scene, t_trace *trace, t_ray ray, int depth)
{
	t_color	reflect_color;

	reflect_color = (t_color){0, 0, 0};
	if (trace->t.hit.material.reflectivity > 0.0)
	{
		trace->reflect_ray.origin = vector_add(trace->t.hit.point,
				scale_vec(trace->t.hit.normal, 0.0001));
		trace->reflect_ray.direction
			= reflect(ray.direction, trace->t.hit.normal);
		reflect_color = trace_ray(scene, trace->reflect_ray, depth - 1);
	}
	return (reflect_color);
}

t_color	compute_refraction(t_scene *scene, t_trace *trace,
		t_ray ray, int depth)
{
	t_color	refract_color;

	refract_color = (t_color){0, 0, 0};
	if (trace->t.hit.material.transparency > 0.0)
	{
		if (scalar_dot(ray.direction, trace->t.hit.normal) < 0)
		{
			trace->eta = 1.0 / trace->t.hit.material.refractive_index;
		}
		else
		{
			trace->t.hit.normal = scale_vec(trace->t.hit.normal, -1);
			trace->eta = trace->t.hit.material.refractive_index;
		}
		if (refract(ray.direction, trace->t.hit.normal, trace->eta,
				&trace->refract_ray.direction))
		{
			trace->refract_ray.origin = vector_add(trace->t.hit.point,
					scale_vec(trace->t.hit.normal, 0.001));
			refract_color = trace_ray(scene, trace->refract_ray, depth - 1);
		}
	}
	return (refract_color);
}

t_color	trace_ray(t_scene *scene, t_ray ray, int depth)
{
	t_trace				trace;
	t_recursive_params	rec_params;

	ft_memset(&trace.t, 0, sizeof(t_tools));
	trace.reflect_color = (t_color){0, 0, 0};
	trace.refract_color = (t_color){0, 0, 0};
	if (depth > 0 && trace_scene(scene, ray, &trace.t.hit))
	{
		normalize_material(&trace.t.hit);
		trace.t.ray = ray;
		compute_base_color(&trace.t);
		light_processing(scene, &trace.t);
		rec_params.reflectivity = trace.t.hit.material.reflectivity;
		rec_params.transparency = trace.t.hit.material.transparency;
		trace.reflect_color = compute_reflection(scene, &trace, ray, depth);
		trace.refract_color = compute_refraction(scene, &trace, ray, depth);
		return (combine_recursive_lighting(trace.t.final_color,
				trace.reflect_color, trace.refract_color, rec_params));
	}
	else
	{
		return (get_background_color(scene, ray));
	}
}
