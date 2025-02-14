/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:20:17 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	check_cone_texture(char *texture_path, t_cone *cone)
{
	int	fd;

	printf("Checking cone texture: %s\n", texture_path);
	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Texture file '%s' not found.\n", texture_path);
		free(cone);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static char	*get_cone_texture(char **tokens, t_cone *cone)
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
			check_cone_texture(texture_path, cone);
		}
		else
		{
			printf("Error: Invalid file extension '%s'", extension);
			free(cone);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		printf("No texture specified for cone.\n");
	}
	return (texture_path);
}

static t_cone	*init_cone_fields(char **tokens)
{
	t_cone	*cone;

	cone = malloc(sizeof(t_cone));
	if (!cone)
		return (NULL);
	cone->vertex = parse_vector(tokens[1]);
	cone->axis = vector_normalize(parse_vector(tokens[2]));
	cone->angle = ft_atof(tokens[3]) * M_PI / 180.0;
	cone->height = ft_atof(tokens[4]);
	cone->color = parse_color(tokens[5]);
	cone->is_checkerboard = false;
	return (cone);
}

// Partie 2 : Gestion des informations de texture et du mode "checkerboard"
static char	*get_cone_texture_info(char **tokens, t_cone *cone)
{
	char	*texture_path;

	if (tokens[6])
	{
		if (ft_strcmp(tokens[6], "checkerboard") == 0)
		{
			cone->is_checkerboard = true;
			texture_path = NULL;
		}
		else
			texture_path = get_cone_texture(tokens, cone);
	}
	else
	{
		texture_path = NULL;
	}
	return (texture_path);
}

int	parse_cone(t_scene *scene, char **tokens)
{
	t_cone	*cone;
	char	*texture_path;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		return (0);
	cone = init_cone_fields(tokens);
	if (!cone)
		return (0);
	texture_path = get_cone_texture_info(tokens, cone);
	add_cone(scene, cone, texture_path);
	return (1);
}
