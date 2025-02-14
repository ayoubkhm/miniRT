/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 17:14:03 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

void print_scene_info(t_scene *scene)
{
	printf("=== Scene Info ===\n");
	printf("Camera Origin: (%f, %f, %f)\n",
		   scene->camera.ray.origin.x,
		   scene->camera.ray.origin.y,
		   scene->camera.ray.origin.z);
	printf("Camera Direction: (%f, %f, %f)\n",
		   scene->camera.ray.direction.x,
		   scene->camera.ray.direction.y,
		   scene->camera.ray.direction.z);
	printf("Movement Flags: up: %d, down: %d, forward: %d, backward: %d, left: %d, right: %d\n",
		   scene->move_up, scene->move_down,
		   scene->move_forward, scene->move_backward,
		   scene->move_left, scene->move_right);
	printf("Rotation Flags: rotate_left: %d, rotate_right: %d, rotate_up: %d, rotate_down: %d\n",
		   scene->rotate_left, scene->rotate_right,
		   scene->rotate_up, scene->rotate_down);
	printf("Accumulated Translation: (%f, %f, %f)\n",
		   scene->accumulated_translation.x,
		   scene->accumulated_translation.y,
		   scene->accumulated_translation.z);
	printf("===================\n");
}

void compute_camera_parameters(t_scene *scene)
{
	t_vec up = {0, 1, 0};

	scene->aspect_ratio = (float)WIDTH / (float)HEIGHT;
	scene->fov_rad = scene->camera.fov * M_PI / 180.0f;
	scene->viewport_height = 2.0f * tan(scene->fov_rad / 2.0f);
	scene->viewport_width = scene->viewport_height * scene->aspect_ratio;
	scene->camera_direction = vector_normalize(scene->camera.ray.direction);
	scene->camera_right = vector_normalize(vector_cross(up, scene->camera_direction));
	scene->camera_up = vector_cross(scene->camera_direction, scene->camera_right);
	scene->pixel_size_x = scene->viewport_width / (float)WIDTH;
	scene->pixel_size_y = scene->viewport_height / (float)HEIGHT;
}

static double get_current_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void close_window(t_scene *scene)
{
	mlx_destroy_window(scene->mlx, scene->win);
	exit(0);
}

void render_scene(t_scene *scene)
{
	int x;
	int y;

	if (!scene->mlx || !scene->win || !scene->image || !scene->image_data)
	{
		printf("Error: mlx, window, or image not initialized.\n");
		return;
	}
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			process_by_pixel(scene, x, y);
			x++;
		}
		printf("\rRendering: %d%%", (100 * y) / HEIGHT);
		y++;
	}
	mlx_put_image_to_window(scene->mlx, scene->win, scene->image, 0, 0);
}

int movement_keys_active(t_scene *scene)
{
	return (scene->move_forward || scene->move_backward ||
			scene->move_left || scene->move_right ||
			scene->rotate_left || scene->rotate_right ||
			scene->rotate_up || scene->rotate_down ||
			scene->move_up || scene->move_down);
}

void update_camera_motion(t_scene *scene)
{
	if (scene->move_forward || scene->move_backward ||
		scene->move_left || scene->move_right ||
		scene->rotate_left || scene->rotate_right ||
		scene->move_up || scene->move_down)
	{
		return;
	}
	scene->camera.ray.origin = vector_add(scene->camera.ray.origin,
		scene->accumulated_translation);
	scene->accumulated_translation = (t_vec){0, 0, 0};
	compute_camera_parameters(scene);
	render_scene(scene);
}

int update_render_if_idle(t_scene *scene)
{
	double current_time = get_current_time();

	if (scene->rendering_pending &&
		(current_time - scene->last_key_release_time >= 0.5))
	{
		scene->camera.ray.origin = vector_add(scene->camera.ray.origin,
			scene->accumulated_translation);
		scene->accumulated_translation = (t_vec){0, 0, 0};
		if (fabs(scene->accumulated_rotation_y) > 0.0001)
		{
			scene->camera.ray.direction = rotate_vec(
				scene->camera.ray.direction,
				(t_vec){0, 1, 0},
				scene->accumulated_rotation_y);
			scene->accumulated_rotation_y = 0;
		}
		if (fabs(scene->accumulated_rotation_x) > 0.0001)
		{
			scene->camera.ray.direction = rotate_vec(
				scene->camera.ray.direction,
				scene->camera_right,
				scene->accumulated_rotation_x);
			scene->accumulated_rotation_x = 0;
		}
		compute_camera_parameters(scene);
		render_scene(scene);
		print_scene_info(scene);
		scene->rendering_pending = false;
	}
	return (0);
}

int key_press(int keycode, t_scene *scene)
{
	if (!scene->key_active)
	{
		scene->key_active = true;
		scene->last_key_press_time = get_current_time();
	}
	if (keycode == 13 || keycode == 119)
		scene->move_forward = true;
	else if (keycode == 1 || keycode == 115)
		scene->move_backward = true;
	else if (keycode == 0 || keycode == 97)
		scene->move_right = true;
	else if (keycode == 2 || keycode == 100)
		scene->move_left = true;
	else if (keycode == 65361)
		scene->rotate_left = true;
	else if (keycode == 65363)
		scene->rotate_right = true;
	else if (keycode == 65362)
		scene->rotate_up = true;
	else if (keycode == 65364)
		scene->rotate_down = true;
	else if (keycode == 114)
		scene->move_up = true;
	else if (keycode == 102)
		scene->move_down = true;
	else if (keycode == 53 || keycode == 65307)
		close_window(scene);
	return (0);
}

int key_release(int keycode, t_scene *scene)
{
	double delta = get_current_time() - scene->last_key_press_time;
	printf("Key released: %d\n", keycode);
	printf("Elapsed time since key press: %f sec (movement offset: %f)\n",
		   delta, MOVE_SPEED * delta);

	if (keycode == 13 || keycode == 119)
	{
		scene->move_forward = false;
		scene->accumulated_translation = vector_add(
			scene->accumulated_translation,
			scale_vec(scene->camera.ray.direction, MOVE_SPEED * delta));
	}
	else if (keycode == 1 || keycode == 115)
	{
		scene->move_backward = false;
		scene->accumulated_translation = vector_sub(
			scene->accumulated_translation,
			scale_vec(scene->camera.ray.direction, MOVE_SPEED * delta));
	}
	else if (keycode == 0 || keycode == 97)
	{
		scene->move_right = false;
		{
			t_vec right = vector_normalize(
				vector_cross(scene->camera.ray.direction, (t_vec){0, 1, 0}));
			scene->accumulated_translation = vector_add(
				scene->accumulated_translation,
				scale_vec(right, MOVE_SPEED * delta));
		}
	}
	else if (keycode == 2 || keycode == 100)
	{
		scene->move_left = false;
		{
			t_vec left = vector_normalize(
				vector_cross((t_vec){0, 1, 0}, scene->camera.ray.direction));
			scene->accumulated_translation = vector_add(
				scene->accumulated_translation,
				scale_vec(left, MOVE_SPEED * delta));
		}
	}
	else if (keycode == 114)
	{
		scene->move_up = false;
		scene->accumulated_translation = vector_add(
			scene->accumulated_translation, (t_vec){0, MOVE_SPEED * delta, 0});
	}
	else if (keycode == 102)
	{
		scene->move_down = false;
		scene->accumulated_translation = vector_sub(
			scene->accumulated_translation, (t_vec){0, MOVE_SPEED * delta, 0});
	}
	else if (keycode == 65361)
	{
		scene->rotate_left = false;
		scene->accumulated_rotation_y -= ROTATE_SPEED * delta;
	}
	else if (keycode == 65363)
	{
		scene->rotate_right = false;
		scene->accumulated_rotation_y += ROTATE_SPEED * delta;
	}
	else if (keycode == 65362)
	{
		scene->rotate_up = false;
		scene->accumulated_rotation_x -= ROTATE_SPEED * delta;
	}
	else if (keycode == 65364)
	{
		scene->rotate_down = false;
		scene->accumulated_rotation_x += ROTATE_SPEED * delta;
	}
	scene->last_key_release_time = get_current_time();
	scene->rendering_pending = true;
	if (!movement_keys_active(scene))
		scene->key_active = false;
	return (0);
}
