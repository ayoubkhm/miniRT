/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 15:52:59 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../lib/libft/libft.h"
#include <fcntl.h>
#include <unistd.h>

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
	int		fd;
	int		status;
	FILE	*file;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return (0);
	}
	file = fdopen(fd, "r");
	if (!file)
	{
		perror("Error: fdopen failed");
		close(fd);
		return (0);
	}
	status = read_scene_file(file, scene);
	close(fd);
	if (!scene->camera_defined || !scene->ambient_light_defined)
	{
		printf("Error: Camera or ambient light not defined\n");
		return (0);
	}
	return (status);
}
