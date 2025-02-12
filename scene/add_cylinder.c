/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/12 14:02:53 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	assign_texture(t_scene *scene, t_object *obj,
		char *texture_path, t_cylinder *cylinder)
{
	if (texture_path)
	{
		obj->texture = mlx_xpm_file_to_image(scene->mlx, texture_path,
				&obj->tex_width, &obj->tex_height);
		if (!obj->texture)
		{
			fprintf(stderr, "Error\n");
			free(obj);
			free(cylinder);
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

static t_object	*create_cylinder_object(t_scene *scene, t_cylinder *cylinder,
		char *texture_path)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
	{
		free(cylinder);
		return (NULL);
	}
	obj->type = CYLINDER;
	obj->data = cylinder;
	obj->color = cylinder->color;
	assign_texture(scene, obj, texture_path, cylinder);
	return (obj);
}

void	add_cylinder(t_scene *scene, t_cylinder *cylinder, char *texture_path)
{
	t_list		*new_node;
	t_object	*object;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(cylinder);
		return ;
	}
	object = create_cylinder_object(scene, cylinder, texture_path);
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
