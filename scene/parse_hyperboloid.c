/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hyperboloid.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:54:20 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:22:13 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	check_hyperboloid_texture(char *texture_path,
	t_hyperboloid *hyperboloid)
{
	int	fd;

	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Texture file '%s' not found.\n", texture_path);
		free(hyperboloid);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static char	*get_hyperboloid_texture(char **tokens, t_hyperboloid *hyperboloid)
{
	char	*texture_path;
	char	*extension;

	texture_path = NULL;
	if (tokens[6] && tokens[6][0] != '#')
	{
		extension = ft_strrchr(tokens[6], '.');
		if (extension && ft_strcmp(extension, ".xpm") == 0)
		{
			texture_path = tokens[6];
			check_hyperboloid_texture(texture_path, hyperboloid);
		}
		else
		{
			printf("Error: Invalid file extension");
			free(hyperboloid);
			exit(EXIT_FAILURE);
		}
	}
	return (texture_path);
}

static t_hyperboloid	*init_hyperboloid_fields(char **tokens)
{
	t_hyperboloid	*hyperboloid;

	hyperboloid = malloc(sizeof(t_hyperboloid));
	if (!hyperboloid)
		return (NULL);
	hyperboloid->base = parse_vector(tokens[1]);
	hyperboloid->axis = parse_vector(tokens[2]);
	hyperboloid->diameter = ft_atof(tokens[3]);
	hyperboloid->radius = ft_atof(tokens[3]) / 2.0;
	hyperboloid->height = ft_atof(tokens[4]);
	hyperboloid->color = parse_color(tokens[5]);
	hyperboloid->is_checkerboard = false;
	return (hyperboloid);
}

static char	*get_hyperboloid_texture_info(char **tokens,
	t_hyperboloid *hyperboloid)
{
	char	*texture_path;

	if (tokens[6])
	{
		if (ft_strcmp(tokens[6], "checkerboard") == 0)
		{
			hyperboloid->is_checkerboard = true;
			texture_path = NULL;
		}
		else
		{
			texture_path = get_hyperboloid_texture(tokens, hyperboloid);
		}
	}
	else
	{
		texture_path = NULL;
	}
	return (texture_path);
}

int	parse_hyperboloid(t_scene *scene, char **tokens)
{
	t_hyperboloid	*hyperboloid;
	char			*texture_path;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
	{
		printf("Error: Invalid hyperboloid definition\n");
		return (0);
	}
	hyperboloid = init_hyperboloid_fields(tokens);
	if (!hyperboloid)
		return (0);
	texture_path = get_hyperboloid_texture_info(tokens, hyperboloid);
	add_hyperboloid(scene, hyperboloid, texture_path);
	return (1);
}
