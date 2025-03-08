/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 04:56:32 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void	compute_camera_parameters(t_scene *scene)
{
	t_vec	up;

	up.x = 0;
	up.y = 1;
	up.z = 0;
	scene->aspect_ratio = (float)WIDTH / (float)HEIGHT;
	scene->fov_rad = scene->camera.fov * M_PI / 180.0f;
	scene->viewport_height = 2.0f * tan(scene->fov_rad / 2.0f);
	scene->viewport_width = scene->viewport_height * scene->aspect_ratio;
	scene->camera_direction = vector_normalize(
			scene->camera.ray.direction);
	scene->camera_right = vector_normalize(vector_cross(
				up, scene->camera_direction));
	scene->camera_up = vector_cross(
			scene->camera_direction, scene->camera_right);
	scene->pixel_size_x = scene->viewport_width / (float)WIDTH;
	scene->pixel_size_y = scene->viewport_height / (float)HEIGHT;
}

void	close_window(t_scene *scene)
{
	mlx_destroy_window(scene->mlx, scene->win);
	exit(0);
}

void	render_scene(t_scene *scene)
{
	int	x;
	int	y;

	if (!scene->mlx || !scene->win || !scene->image || !scene->image_data)
	{
		printf("Error: mlx, window, or image not initialized.\n");
		return ;
	}
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
			process_by_pixel(scene, x++, y);
		printf("\rRendering: %d%%", (100 * y) / HEIGHT);
		y++;
	}
	mlx_put_image_to_window(scene->mlx, scene->win, scene->image, 0, 0);
}

int	key_press(int keycode, t_scene *scene)
{
	if (keycode == 53 || keycode == 65307)
		close_window(scene);
	return (0);
}
