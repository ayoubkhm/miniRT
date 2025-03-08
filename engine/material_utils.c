/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:29:56 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 04:30:40 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	get_checkerboard_color(t_vec uv, int scale,
		t_color color1, t_color color2)
{
	int	u;
	int	v;

	u = (int)(uv.x * scale);
	v = (int)(uv.y * scale);
	if ((u + v) % 2 == 0)
		return (color1);
	else
		return (color2);
}

void	normalize_material(t_hit *hit)
{
	if (hit->material.reflectivity < 0.0 || hit->material.reflectivity > 1.0)
		hit->material.reflectivity = 0.0;
	if (hit->material.transparency < 0.0 || hit->material.transparency > 1.0)
		hit->material.transparency = 0.0;
	if (hit->material.refractive_index <= 0.0)
		hit->material.refractive_index = 1.0;
}

void	compute_base_color(t_tools *t)
{
	t->base_color = t->hit.material.color;
	if (t->hit.object->is_checkerboard)
	{
		t->base_color = get_checkerboard_color(t->hit.uv, 10,
				(t_color){255, 255, 255}, (t_color){0, 0, 0});
	}
	else if (t->hit.object->texture_data)
	{
		t->base_color = get_texture_color(t->hit.object, t->hit.uv);
	}
}
