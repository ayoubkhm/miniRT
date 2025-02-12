/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/12 14:36:01 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../lib/libft/libft.h"

static int	process_scene_line(char *line, t_scene *scene)
{
	char	*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t\n\r");
	if (trimmed_line[0] == '#' || trimmed_line[0] == '\0')
	{
		free(trimmed_line);
		return (1);
	}
	if (!parse_line(scene, trimmed_line))
	{
		fprintf(stderr, "Error parsing line: %s\n", trimmed_line);
		free(trimmed_line);
		return (0);
	}
	free(trimmed_line);
	return (1);
}

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	init_scene(t_scene *scene)
{
	scene->camera_defined = 0;
	scene->ambient_light_defined = 0;
	scene->lights = NULL;
	scene->objects = NULL;
	scene->object_count = 0;
	scene->light_count = 0;
	scene->mlx = mlx_init();
	if (!scene->mlx)
		return ((void)fprintf(stderr, "mlx_init failed\n"));
	scene->win = mlx_new_window(scene->mlx, WIDTH, HEIGHT, "MERVAN LE GOAT");
	scene->image = mlx_new_image(scene->mlx, WIDTH, HEIGHT);
	scene->image_data = mlx_get_data_addr(scene->image, &scene->bpp,
			&scene->line_len, &scene->endian);
	scene->move_forward = false;
	scene->move_backward = false;
	scene->move_left = false;
	scene->move_right = false;
	scene->rotate_left = false;
	scene->rotate_right = false;
	scene->rotate_up = false;
	scene->rotate_down = false;
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

	file = fopen(filename, "r");
	if (!file)
	{
		perror("Error opening file");
		return (0);
	}
	status = read_scene_file(file, scene);
	fclose(file);
	if (!scene->camera_defined || !scene->ambient_light_defined)
	{
		fprintf(stderr, "Error: Camera or ambient light not defined\n");
		return (0);
	}
	return (status);
}
