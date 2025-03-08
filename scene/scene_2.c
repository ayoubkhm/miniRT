/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 05:30:29 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../lib/libft/libft.h"

int	process_scene_line(char *line, t_scene *scene)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t\n\r");
	if (trimmed_line[0] == '#' || trimmed_line[0] == '\0')
	{
		free(trimmed_line);
		return (1);
	}
	if (!parse_line(scene, trimmed_line))
	{
		printf(stderr, "Error parsing line: %s\n", trimmed_line);
		free(trimmed_line);
		return (0);
	}
	free(trimmed_line);
	return (1);
}

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	init_scene_defaults(t_scene *scene)
{
	if (!scene)
	{
		printf("Error: scene pointer is NULL\n");
		return ;
	}
	scene->camera_defined = 0;
	scene->ambient_light_defined = 0;
	scene->lights = NULL;
	scene->objects = NULL;
	scene->object_count = 0;
	scene->light_count = 0;
}

void	init_graphics(t_scene *scene)
{
	scene->mlx = mlx_init();
	if (!scene->mlx)
	{
		printf("mlx_init failed\n");
		exit(1);
	}
	scene->win = mlx_new_window(scene->mlx, WIDTH, HEIGHT, "MERVAN LE GOAT");
	if (!scene->win)
	{
		printf("mlx_new_window failed\n");
		exit(1);
	}
	scene->image = mlx_new_image(scene->mlx, WIDTH, HEIGHT);
	if (!scene->image)
	{
		printf("mlx_new_image failed\n");
		exit(1);
	}
	scene->image_data = mlx_get_data_addr(scene->image, &scene->bpp,
			&scene->line_len, &scene->endian);
	if (!scene->image_data)
	{
		printf("mlx_get_data_addr failed\n");
		exit(1);
	}
}

void	init_controls_and_camera(t_scene *scene)
{
	scene->move_forward = false;
	scene->move_backward = false;
	scene->move_left = false;
	scene->move_right = false;
	scene->rotate_left = false;
	scene->rotate_right = false;
	scene->rotate_up = false;
	scene->rotate_down = false;
	scene->move_up = false;
	scene->move_down = false;
	scene->accumulated_translation = (t_vec){0, 0, 0};
	scene->last_key_press_time = 0;
	scene->last_key_release_time = 0;
	scene->key_active = false;
	scene->rendering_pending = false;
	scene->accumulated_rotation_y = 0;
	scene->accumulated_rotation_x = 0;
	scene->camera_direction = (t_vec){0, 0, -1};
	scene->camera_up = (t_vec){0, 1, 0};
	scene->camera_right = (t_vec){1, 0, 0};
	scene->viewport_width = 2.0f;
	scene->viewport_height = 2.0f;
	scene->pixel_size_x = scene->viewport_width / WIDTH;
	scene->pixel_size_y = scene->viewport_height / HEIGHT;
	scene->simple_render_mode = false;
}
