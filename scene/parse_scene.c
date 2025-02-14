/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 16:23:43 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static int	handle_identifier(t_scene *scene, char **tokens)
{
	if (!ft_strcmp(tokens[0], "A"))
		return (parse_ambient_light(scene, tokens));
	else if (!ft_strcmp(tokens[0], "C"))
		return (parse_camera(scene, tokens));
	else if (!ft_strcmp(tokens[0], "L"))
		return (parse_light(scene, tokens));
	else if (!ft_strcmp(tokens[0], "sp"))
		return (parse_sphere(scene, tokens));
	else if (!ft_strcmp(tokens[0], "pl"))
		return (parse_plane(scene, tokens));
	else if (!ft_strcmp(tokens[0], "cy"))
		return (parse_cylinder(scene, tokens));
	else if (!ft_strcmp(tokens[0], "hy"))
		return (parse_hyperboloid(scene, tokens));
	else if (!ft_strcmp(tokens[0], "pa"))
		return (parse_paraboloid(scene, tokens));
	else if (!ft_strcmp(tokens[0], "co"))
		return (parse_cone(scene, tokens));
	else
	{
		printf("Unknown identifier: %s\n", tokens[0]);
		return (0);
	}
}

int	parse_line(t_scene *scene, char *line)
{
	char	**tokens;
	int		ret;

	tokens = ft_split(line, ' ');
	if (!tokens[0])
	{
		free_tokens(tokens);
		return (0);
	}
	ret = handle_identifier(scene, tokens);
	free_tokens(tokens);
	return (ret);
}

int	parse_ambient_light(t_scene *scene, char **tokens)
{
	if (scene->ambient_light_defined)
	{
		printf("Error: Ambient light already defined\n");
		return (0);
	}
	if (!tokens[1] || !tokens[2])
	{
		printf("Error: Invalid ambient light definition\n");
		return (0);
	}
	scene->ambient_light.ratio = ft_atof(tokens[1]);
	scene->ambient_light.color = parse_color(tokens[2]);
	if (scene->ambient_light.ratio < 0.0
		|| scene->ambient_light.ratio > 1.0)
	{
		printf("Error: Ambient light ratio out of range\n");
		return (0);
	}
	scene->ambient_light_defined = 1;
	return (1);
}

int	parse_camera(t_scene *scene, char **tokens)
{
	if (scene->camera_defined)
	{
		printf("Error: Camera already defined\n");
		return (0);
	}
	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		printf("Error: Invalid camera definition\n");
		return (0);
	}
	scene->camera.ray.origin = parse_vector(tokens[1]);
	scene->camera.ray.direction = parse_vector(tokens[2]);
	scene->camera.fov = ft_atof(tokens[3]);
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
	{
		printf("Error: Camera FOV out of range\n");
		return (0);
	}
	scene->camera_defined = 1;
	return (1);
}

int	parse_light(t_scene *scene, char **tokens)
{
	t_light	*light;

	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		printf("Error: Invalid light definition\n");
		return (0);
	}
	light = malloc(sizeof(t_light));
	if (!light)
		return (0);
	light->position = parse_vector(tokens[1]);
	light->ratio = ft_atof(tokens[2]);
	light->color = parse_color(tokens[3]);
	if (light->ratio < 0.0 || light->ratio > 1.0)
	{
		printf("Error: Light ratio out of range\n");
		free(light);
		return (0);
	}
	add_light(scene, light);
	return (1);
}
