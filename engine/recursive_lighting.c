#include "../includes/minirt.h"

t_color	compute_recursive_lighting_sum(t_color local_color,
    t_color reflect_color, t_color refract_color, t_recursive_params params)
{
double	local_weight;
t_color	result;

local_weight = 1.0 - params.reflectivity - params.transparency;
if (local_weight < 0)
    local_weight = 0.0;
result.r = (int)(local_color.r * local_weight + reflect_color.r
        * params.reflectivity + refract_color.r * params.transparency);
result.g = (int)(local_color.g * local_weight + reflect_color.g
        * params.reflectivity + refract_color.g * params.transparency);
result.b = (int)(local_color.b * local_weight + reflect_color.b
        * params.reflectivity + refract_color.b * params.transparency);
return (result);
}

t_color	clamp_color(t_color color)
{
	if (color.r > 255)
		color.r = 255;
	if (color.g > 255)
		color.g = 255;
	if (color.b > 255)
		color.b = 255;
	return (color);
}

t_color	combine_recursive_lighting(t_color local_color, t_color reflect_color,
			t_color refract_color, t_recursive_params params)
{
	t_color	final_color;

	final_color = compute_recursive_lighting_sum(local_color,
			reflect_color, refract_color, params);
	final_color = clamp_color(final_color);
	return (final_color);
}