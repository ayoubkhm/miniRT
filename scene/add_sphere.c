/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:20:01 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static t_object	*assign_sphere_tex(t_scene *scene, t_object *obj,
	t_sphere *sphere, char *texture_path)
{
	obj->texture = mlx_xpm_file_to_image(scene->mlx, texture_path,
			&obj->tex_width, &obj->tex_height);
	if (!obj->texture)
	{
		printf("Error\n");
		free(obj);
		free(sphere);
		return (NULL);
	}
	obj->texture_data = mlx_get_data_addr(obj->texture, &obj->bpp,
			&obj->line_len, &obj->endian);
	return (obj);
}

static t_object	*create_sphere_object(t_scene *scene,
	t_sphere *sphere, char *texture_path)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
	{
		free(sphere);
		return (NULL);
	}
	obj->type = SPHERE;
	obj->data = sphere;
	obj->color = sphere->color;
	obj->is_checkerboard = sphere->is_checkerboard;
	if (texture_path)
	{
		if (!assign_sphere_tex(scene, obj, sphere, texture_path))
			return (NULL);
	}
	else
	{
		obj->texture = NULL;
		obj->texture_data = NULL;
	}
	return (obj);
}

void	add_sphere(t_scene *scene, t_sphere *sphere, char *texture_path)
{
	t_list		*new_node;
	t_object	*object;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(sphere);
		return ;
	}
	object = create_sphere_object(scene, sphere, texture_path);
	if (!object)
	{
		free(new_node);
		exit(EXIT_FAILURE);
	}
	new_node->content = object;
	new_node->next = scene->objects;
	scene->objects = new_node;
	scene->object_count++;
}
