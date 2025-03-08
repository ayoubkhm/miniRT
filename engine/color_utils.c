/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 03:47:04 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 04:29:50 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color_d	normalize_color(t_color color)
{
	t_color_d	normalized;

	normalized.r = color.r / 255.0;
	normalized.g = color.g / 255.0;
	normalized.b = color.b / 255.0;
	return (normalized);
}

void	compute_light_factors(t_lighting_params params,
			double *local, double *extra)
{
	*local = params.ambient + params.diffuse;
	if (*local > 1.0)
		*local = 1.0;
	*extra = params.specular + params.reflection;
	if (*extra > 1.0)
		*extra = 1.0;
}

t_color_d	apply_lighting_factors(t_color_d base, double local, double extra)
{
	t_color_d	result;

	result.r = base.r * local + extra * 0.7;
	result.g = base.g * local + extra * 0.7;
	result.b = base.b * local + extra * 0.7;
	result.r = fmin(1.0, result.r);
	result.g = fmin(1.0, result.g);
	result.b = fmin(1.0, result.b);
	return (result);
}

t_color	convert_color_d_to_color(t_color_d cd)
{
	t_color	color;

	color.r = (int)(cd.r * 255);
	color.g = (int)(cd.g * 255);
	color.b = (int)(cd.b * 255);
	return (color);
}

t_color	combine_lighting(t_color base_color, t_lighting_params params)
{
	t_color_d	normalized;
	double		local;
	double		extra;
	t_color_d	final_d;
	t_color		final_color;

	normalized = normalize_color(base_color);
	compute_light_factors(params, &local, &extra);
	final_d = apply_lighting_factors(normalized, local, extra);
	final_color = convert_color_d_to_color(final_d);
	return (final_color);
}
