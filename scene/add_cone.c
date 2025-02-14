/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:19:13 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	assign_cone_texture(t_scene *scene, t_object *obj,
		char *texture_path, t_cone *cone)
{
	if (texture_path)
	{
		obj->texture = mlx_xpm_file_to_image(scene->mlx, texture_path,
				&obj->tex_width, &obj->tex_height);
		if (!obj->texture)
		{
			printf("Error loading texture file: %s\n", texture_path);
			free(obj);
			free(cone);
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

static t_object	*create_cone_object(t_scene *scene,
	t_cone *cone, char *texture_path)
{
	t_object	*obj;

	obj = malloc(sizeof(t_object));
	if (!obj)
	{
		free(cone);
		return (NULL);
	}
	obj->type = CONE;
	obj->data = cone;
	obj->color = cone->color;
	obj->is_checkerboard = cone->is_checkerboard;
	assign_cone_texture(scene, obj, texture_path, cone);
	return (obj);
}

void	add_cone(t_scene *scene, t_cone *cone, char *texture_path)
{
	t_list		*new_node;
	t_object	*object;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(cone);
		return ;
	}
	object = create_cone_object(scene, cone, texture_path);
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
