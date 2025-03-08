/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 05:30:02 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../lib/libft/libft.h"

void	init_scene(t_scene *scene)
{
	if (!scene)
	{
		printf("Error: scene pointer is NULL\n");
		return ;
	}
	init_scene_defaults(scene);
	init_graphics(scene);
	init_controls_and_camera(scene);
}

static int	read_scene_file(FILE *file, t_scene *scene)
{
	char	*line;
	size_t	len;
	ssize_t	read_value;
	int		status;

	line = NULL;
	len = 0;
	status = 1;
	read_value = getline(&line, &len, file);
	while (read_value != -1)
	{
		if (!process_scene_line(line, scene))
		{
			status = 0;
			break ;
		}
		read_value = getline(&line, &len, file);
	}
	free(line);
	return (status);
}

int	load_scene(t_scene *scene, const char *filename)
{
	FILE	*file;
	int		status;

	file = open(filename, "r");
	if (!file)
	{
		perror("Error opening file");
		return (0);
	}
	status = read_scene_file(file, scene);
	close(file);
	if (!scene->camera_defined || !scene->ambient_light_defined)
	{
		printf(stderr, "Error: Camera or ambient light not defined\n");
		return (0);
	}
	return (status);
}
