/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:16:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:16:14 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include <math.h>

t_color	get_texture_color(t_object *obj, t_vec uv)
{
	t_color	color;
	int		tex_x;
	int		tex_y;
	int		index;

	if (!obj->texture_data)
		return (obj->color);
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);
	tex_x = (int)(uv.x * obj->tex_width);
	tex_y = (int)(uv.y * obj->tex_height);
	index = tex_y * obj->line_len + tex_x * (obj->bpp / 8);
	color.b = (unsigned char)obj->texture_data[index];
	color.g = (unsigned char)obj->texture_data[index + 1];
	color.r = (unsigned char)obj->texture_data[index + 2];
	return (color);
}
