/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:31:46 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:32:26 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "../lib/libft/libft.h"

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_color_d
{
	double	r;
	double	g;
	double	b;
}	t_color_d;

typedef struct s_ray
{
	t_vec	origin;
	t_vec	direction;
}	t_ray;

typedef struct s_material
{
	t_color	color;
	double	reflectivity;
	double	transparency;
	double	refractive_index;
	double	specular_exponent;
}	t_material;

struct					s_object;
typedef struct s_object	t_object;

typedef struct s_hit
{
	double		t;
	t_vec		point;
	t_vec		normal;
	t_material	material;
	t_vec		uv;
	t_object	*object;
}	t_hit;

typedef struct s_tools
{
	t_ray	ray;
	t_hit	hit;
	int		hit_found;
	double	ambient;
	double	diffuse;
	double	specular;
	double	reflection;
	t_color	base_color;
	t_color	final_color;
}	t_tools;

typedef struct s_trace
{
	t_tools	t;
	t_color	reflect_color;
	t_color	refract_color;
	t_ray	reflect_ray;
	t_ray	refract_ray;
	double	eta;
}	t_trace;

typedef struct s_lighting_params
{
	double	ambient;
	double	diffuse;
	double	specular;
	double	reflection;
}	t_lighting_params;

typedef struct s_recursive_params
{
	double	reflectivity;
	double	transparency;
}	t_recursive_params;

typedef struct s_ray_calc
{
	float	ndc_x;
	float	ndc_y;
	float	screen_x;
	float	screen_y;
	float	cam_x;
	float	cam_y;
}	t_ray_calc;

typedef struct s_image
{
	char	*data;
	int		size_line;
}	t_image;

typedef struct s_rot
{
	t_vec	axis;
	t_vec	lo;
	t_vec	ld;
	double	angle;
}	t_rot;

typedef struct s_quad
{
	double	a;
	double	b;
	double	c;
}	t_quad;

typedef struct s_intinfo
{
	double	t;
	int		cand;
}	t_intinfo;

#endif
