/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:06:11 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/10 00:08:51 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	parse_cylinder(t_scene *scene, char **tokens)
{
	t_cylinder	*cylinder;
	char		*texture_path;
	int			fd;
	char		*extension;

	texture_path = NULL;
	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
	{
		fprintf(stderr, "Error: Invalid cylinder definition\n");
		return (0);
	}
	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
		return (0);
	cylinder->base = parse_vector(tokens[1]);
	cylinder->axis = parse_vector(tokens[2]);
	cylinder->diameter = ft_atof(tokens[3]);
	cylinder->radius = ft_atof(tokens[3]) / 2.0;
	cylinder->height = ft_atof(tokens[4]);
	cylinder->color = parse_color(tokens[5]);
	if (tokens[6] && tokens[6][0] != '#')
	{
		printf("Checking texture path: %s\n", tokens[6]);

		extension = ft_strrchr(tokens[6], '.');
		if (extension && ft_strcmp(extension, ".xpm") == 0)
		{
			texture_path = tokens[6];
			fd = open(texture_path, O_RDONLY);
			if (fd == -1)
			{
				fprintf(stderr, "Error: Texture file '%s' not found.\n", texture_path);
				free(cylinder);
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		else
		{
			fprintf(stderr, "Error: Invalid file extension '%s' (only .xpm supported)\n", extension);
			free(cylinder);
			exit(EXIT_FAILURE);
		}
	}
	add_cylinder(scene, cylinder, texture_path);
	return (1);
}

void	add_cylinder(t_scene *scene, t_cylinder *cylinder, char *texture_path)
{
	t_list		*new_node;
	t_object	*object;

	new_node = malloc(sizeof(t_list));
	object = malloc(sizeof(t_object));
	if (!new_node || !object)
	{
		free(cylinder);
		return ;
	}
	object->type = CYLINDER;
	object->data = cylinder;
	object->color = cylinder->color;
	if (texture_path)
	{
		object->texture = mlx_xpm_file_to_image(scene->mlx, texture_path, &object->tex_width, &object->tex_height);
		if (!object->texture)
		{
			fprintf(stderr, "Error\n");
			free(object);
			free(cylinder);
			free(new_node);
			exit(EXIT_FAILURE);
		}
		object->texture_data = mlx_get_data_addr(object->texture, &object->bpp, &object->line_len, &object->endian);
	}
	else
	{
		object->texture = NULL;
		object->texture_data = NULL;
	}
	new_node->content = object;
	new_node->next = scene->objects;
	scene->objects = new_node;
}
