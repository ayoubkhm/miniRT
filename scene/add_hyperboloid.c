/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_hyperboloid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 00:12:16 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	assign_hyperboloid_texture(t_scene *scene, t_object *obj,
		char *texture_path, t_hyperboloid *hyperboloid)
{
	if (texture_path)
	{
		obj->texture = mlx_xpm_file_to_image(scene->mlx, texture_path,
				&obj->tex_width, &obj->tex_height);
		if (!obj->texture)
		{
			printf("Error loading texture file: %s\n", texture_path);
			free(obj);
			free(hyperboloid);
			exit(EXIT_FAILURE);
		}
		obj->texture_data = mlx_get_data_addr(obj->texture, &obj->bpp,
				&obj->line_len, &obj->endian);
	}
	else
	{
		obj->texture = NULL;
		obj->texture_data = NULL;
	}
}

static t_object	*create_hyperboloid_object(t_scene *scene,
	t_hyperboloid *hyperboloid, char *texture_path)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
	{
		free(hyperboloid);
		return (NULL);
	}
	obj->type = HYPERBOLOID;
	obj->data = hyperboloid;
	obj->color = hyperboloid->color;
	obj->is_checkerboard = hyperboloid->is_checkerboard;
	assign_hyperboloid_texture(scene, obj, texture_path, hyperboloid);
	return (obj);
}

void	add_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid,
		char *texture_path)
{
	t_list		*new_node;
	t_object	*object;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(hyperboloid);
		return ;
	}
	object = create_hyperboloid_object(scene, hyperboloid, texture_path);
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
