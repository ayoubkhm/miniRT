/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 01:42:01 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 14:24:11 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//main charactere is ayoub LOL.c
// 

#include "../includes/minirt.h"
#include <math.h>

static int	process_object(t_ray ray, t_object *object,
	double *closest_t, t_hit *hit)
{
	t_hit	temp;

	ft_memset(&temp, 0, sizeof(t_hit));
	if (intersect_object(ray, object, &temp))
	{
		if (temp.t < *closest_t && temp.t > (EPSILON * 10.0))
		{
			*closest_t = temp.t;
			*hit = temp;
			return (1);
		}
	}
	return (0);
}

int	trace_scene(t_scene *scene, t_ray ray, t_hit *hit)
{
	int			found;
	int			i;
	double		closest_t;
	t_list		*obj;
	t_object	*object;

	found = 0;
	i = 0;
	closest_t = DBL_MAX;
	obj = scene->objects;
	while (i < scene->object_count)
	{
		object = (t_object *)obj->content;
		if (process_object(ray, object, &closest_t, hit))
			found = 1;
		obj = obj->next;
		i++;
	}
	return (found);
}

void	put_pixel(t_image *img, int x, int y, t_color color)
{
	int	index;

	index = y * img->size_line + x * 4;
	img->data[index] = color.b;
	img->data[index + 1] = color.g;
	img->data[index + 2] = color.r;
	img->data[index + 3] = 0;
}

int	main(int ac, char **av)
{
	t_scene	scene;

	if (ac != 2)
	{
		printf("Usage: %s <scene_file.rt>\n", av[0]);
		return (EXIT_FAILURE);
	}
	init_scene(&scene);
	scene.accumulated_translation = (t_vec){0, 0, 0};
	if (!load_scene(&scene, av[1]))
	{
		printf("Error.\n");
		free_scene(&scene);
		return (EXIT_FAILURE);
	}
	printf("Scene loaded successfully:\n");
	compute_camera_parameters(&scene);
	render_scene(&scene);
	mlx_hook(scene.win, 2, 1L << 0, key_press, &scene);
	mlx_loop(scene.mlx);
	free_scene(&scene);
	return (EXIT_SUCCESS);
}
