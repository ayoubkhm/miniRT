/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_paraboloid.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:22:31 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	check_paraboloid_texture(char *texture_path,
	t_paraboloid *paraboloid)
{
	int	fd;

	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Texture file");
		free(paraboloid);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static char	*get_paraboloid_texture(char **tokens, t_paraboloid *paraboloid)
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
			check_paraboloid_texture(texture_path, paraboloid);
		}
		else
		{
			printf("Error: Invalid file extension");
			free(paraboloid);
			exit(EXIT_FAILURE);
		}
	}
	return (texture_path);
}

static t_paraboloid	*init_paraboloid_fields(char **tokens)
{
	t_paraboloid	*paraboloid;

	paraboloid = malloc(sizeof(t_paraboloid));
	if (!paraboloid)
		return (NULL);
	paraboloid->base = parse_vector(tokens[1]);
	paraboloid->axis = parse_vector(tokens[2]);
	paraboloid->p = ft_atof(tokens[3]);
	paraboloid->height = ft_atof(tokens[4]);
	paraboloid->color = parse_color(tokens[5]);
	paraboloid->is_checkerboard = false;
	return (paraboloid);
}

static char	*get_paraboloid_texture_info(char **tokens,
	t_paraboloid *paraboloid)
{
	char	*texture_path;

	if (tokens[6])
	{
		if (ft_strcmp(tokens[6], "checkerboard") == 0)
		{
			paraboloid->is_checkerboard = true;
			texture_path = NULL;
		}
		else
			texture_path = get_paraboloid_texture(tokens, paraboloid);
	}
	else
		texture_path = NULL;
	return (texture_path);
}

int	parse_paraboloid(t_scene *scene, char **tokens)
{
	t_paraboloid	*paraboloid;
	char			*texture_path;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
	{
		printf("Error: Invalid paraboloid definition\n");
		return (0);
	}
	paraboloid = init_paraboloid_fields(tokens);
	if (!paraboloid)
		return (0);
	texture_path = get_paraboloid_texture_info(tokens, paraboloid);
	add_paraboloid(scene, paraboloid, texture_path);
	return (1);
}
