#include "../includes/minirt.h"

void	light_processing(t_scene *scene, t_tools *t)
{
	t_light				*light;
	t_list				*lig;
	double				shadow;
	t_lighting_params	params;

	lig = scene->lights;
	while (lig != NULL)
	{
		light = lig->content;
		shadow = compute_shadow(scene, t->hit, light);
		t->diffuse += compute_diffuse_single(scene, t->hit, light) * shadow;
		t->specular += compute_specular_single(scene,
				t->hit, t->ray, light) * shadow;
		lig = lig->next;
	}
	params.ambient = scene->ambient_light.ratio;
	params.diffuse = t->diffuse;
	params.specular = t->specular;
	params.reflection = 0.0;
	t->final_color = combine_lighting(t->base_color, params);
}

t_color	get_background_color(t_scene *scene, t_ray ray)
{
	t_color	color;
	double	t_value;

	(void)scene;
	t_value = 0.5 * (ray.direction.y + 1.0);
	color.r = (int)((1.0 - t_value) * 255 + t_value * 128);
	color.g = (int)((1.0 - t_value) * 255 + t_value * 178);
	color.b = (int)((1.0 - t_value) * 255 + t_value * 255);
	if (color.r > 255)
		color.r = 255;
	if (color.g > 255)
		color.g = 255;
	if (color.b > 255)
		color.b = 255;
	return (color);
}

void	process_by_pixel(t_scene *scene, int x, int y)
{
	t_ray_calc	calc;
	t_vec		ray_dir;
	t_ray		ray;
	t_color		color;

	calc.ndc_x = ((float)x + 0.5f) / (float)WIDTH;
	calc.ndc_y = ((float)y + 0.5f) / (float)HEIGHT;
	calc.screen_x = 2.0f * calc.ndc_x - 1.0f;
	calc.screen_y = 1.0f - 2.0f * calc.ndc_y;
	calc.cam_x = calc.screen_x * scene->viewport_width / 2.0f;
	calc.cam_y = calc.screen_y * scene->viewport_height / 2.0f;
	ray_dir = vector_normalize(vector_add(vector_add(
					scale_vec(scene->camera_right, calc.cam_x),
					scale_vec(scene->camera_up, calc.cam_y)),
				scene->camera_direction));
	ray.origin = scene->camera.ray.origin;
	ray.direction = ray_dir;
	color = trace_ray(scene, ray, MAX_DEPTH);
	put_pixel(scene->image_data, x, y, color, scene->line_len);
}
