/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/12 14:12:53 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	assign_plane_texture(t_scene *scene, t_object *obj,
		t_plane *plane, char *texture_path)
{
	obj->texture = mlx_xpm_file_to_image(scene->mlx, texture_path,
			&obj->tex_width, &obj->tex_height);
	if (!obj->texture)
	{
		fprintf(stderr, "Error loading texture file: %s\n", texture_path);
		free(obj);
		free(plane);
		exit(EXIT_FAILURE);
	}
	obj->texture_data = mlx_get_data_addr(obj->texture, &obj->bpp,
			&obj->line_len, &obj->endian);
}

static t_object	*create_plane_object(t_scene *scene, t_plane *plane,
		char *texture_path)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
	{
		free(plane);
		return (NULL);
	}
	obj->type = PLANE;
	obj->data = plane;
	obj->color = plane->color;
	if (texture_path)
		assign_plane_texture(scene, obj, plane, texture_path);
	else
	{
		obj->texture = NULL;
		obj->texture_data = NULL;
	}
	return (obj);
}

void	add_plane(t_scene *scene, t_plane *plane, char *texture_path)
{
	t_list		*new_node;
	t_object	*object;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(plane);
		return ;
	}
	object = create_plane_object(scene, plane, texture_path);
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
