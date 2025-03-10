/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:22:47 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	check_texture_file(char *texture_path, t_plane *plane)
{
	int	fd;

	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Texture file '%s' not found.\n", texture_path);
		free(plane);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static char	*get_plane_texture(char **tokens, t_plane *plane)
{
	char	*texture_path;
	char	*extension;

	texture_path = NULL;
	if (tokens[4] && tokens[4][0] != '#')
	{
		if (ft_strcmp(tokens[4], "checkerboard") == 0)
		{
			plane->is_checkerboard = true;
			return (NULL);
		}
		extension = ft_strrchr(tokens[4], '.');
		if (extension && ft_strcmp(extension, ".xpm") == 0)
		{
			texture_path = tokens[4];
			check_texture_file(texture_path, plane);
		}
		else
		{
			printf("Error: Invalid file extension");
			free(plane);
			exit(EXIT_FAILURE);
		}
	}
	return (texture_path);
}

int	parse_plane(t_scene *scene, char **tokens)
{
	t_plane	*plane;
	char	*texture_path;

	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		printf("Error: Invalid plane definition\n");
		return (0);
	}
	plane = malloc(sizeof(t_plane));
	if (!plane)
		return (0);
	plane->point = parse_vector(tokens[1]);
	plane->normal = parse_vector(tokens[2]);
	plane->color = parse_color(tokens[3]);
	plane->is_checkerboard = false;
	texture_path = get_plane_texture(tokens, plane);
	add_plane(scene, plane, texture_path);
	return (1);
}
