/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 13:58:35 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	check_sphere_texture(char *texture_path, t_sphere *sphere)
{
	int	fd;

	fd = open(texture_path, O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Texture file '%s' not found.\n", texture_path);
		free(sphere);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static char	*get_sphere_texture(char **tokens, t_sphere *sphere)
{
	char	*texture_path;
	char	*extension;

	texture_path = NULL;
	if (tokens[4] && tokens[4][0] != '#')
	{
		extension = ft_strrchr(tokens[4], '.');
		if (extension && ft_strcmp(extension, ".xpm") == 0)
		{
			texture_path = tokens[4];
			check_sphere_texture(texture_path, sphere);
		}
		else
		{
			printf("Error: Invalid file extension '%s'\n",
				extension);
			free(sphere);
			exit(EXIT_FAILURE);
		}
	}
	return (texture_path);
}

static t_sphere	*init_sphere_fields(char **tokens)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (NULL);
	sphere->center = parse_vector(tokens[1]);
	sphere->radius = ft_atof(tokens[2]) / 2.0;
	sphere->color = parse_color(tokens[3]);
	sphere->is_checkerboard = false;
	return (sphere);
}

static char	*get_sphere_texture_info(char **tokens, t_sphere *sphere)
{
	char	*texture_path;

	if (tokens[4])
	{
		if (ft_strcmp(tokens[4], "checkerboard") == 0)
		{
			sphere->is_checkerboard = true;
			texture_path = NULL;
		}
		else
		{
			texture_path = get_sphere_texture(tokens, sphere);
		}
	}
	else
		texture_path = NULL;
	return (texture_path);
}

int	parse_sphere(t_scene *scene, char **tokens)
{
	t_sphere	*sphere;
	char		*texture_path;

	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		printf("Error: Invalid sphere definition\n");
		return (0);
	}
	sphere = init_sphere_fields(tokens);
	if (!sphere)
		return (0);
	texture_path = get_sphere_texture_info(tokens, sphere);
	add_sphere(scene, sphere, texture_path);
	return (1);
}
