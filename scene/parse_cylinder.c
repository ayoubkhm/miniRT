/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:20:23 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	handle_texture_error(t_cylinder *cylinder, char *msg, char *str)
{
	fprintf(stderr, msg, str);
	free(cylinder);
	exit(EXIT_FAILURE);
}

char	*get_texture_path(char **tokens, t_cylinder *cylinder)
{
	char	*texture_path;
	char	*extension;
	int		fd;

	texture_path = NULL;
	if (!(tokens[6] && tokens[6][0] != '#'))
		return (NULL);
	printf("Checking texture path: %s\n", tokens[6]);
	extension = ft_strrchr(tokens[6], '.');
	if (!(extension && ft_strcmp(extension, ".xpm") == 0))
		handle_texture_error(cylinder,
			"Error: Invalid file extension '%s' (only .xpm supported)\n",
			extension);
	texture_path = tokens[6];
	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
		handle_texture_error(cylinder,
			"Error: Texture file \n", texture_path);
	close(fd);
	return (texture_path);
}

static	t_cylinder	*init_cylinder_fields(char **tokens)
{
	t_cylinder	*cylinder;

	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
		return (NULL);
	cylinder->base = parse_vector(tokens[1]);
	cylinder->axis = parse_vector(tokens[2]);
	cylinder->diameter = ft_atof(tokens[3]);
	cylinder->radius = cylinder->diameter / 2.0;
	cylinder->height = ft_atof(tokens[4]);
	cylinder->color = parse_color(tokens[5]);
	cylinder->is_checkerboard = false;
	return (cylinder);
}

static char	*get_cylinder_texture_info(char **tokens, t_cylinder *cylinder)
{
	char	*texture_path;

	if (tokens[6])
	{
		if (ft_strcmp(tokens[6], "checkerboard") == 0)
		{
			cylinder->is_checkerboard = true;
			texture_path = NULL;
		}
		else
			texture_path = get_texture_path(tokens, cylinder);
	}
	else
	{
		texture_path = NULL;
	}
	return (texture_path);
}

int	parse_cylinder(t_scene *scene, char **tokens)
{
	t_cylinder	*cylinder;
	char		*texture_path;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
	{
		printf("Error: Invalid cylinder definition\n");
		return (0);
	}
	cylinder = init_cylinder_fields(tokens);
	if (!cylinder)
		return (0);
	texture_path = get_cylinder_texture_info(tokens, cylinder);
	add_cylinder(scene, cylinder, texture_path);
	return (1);
}
