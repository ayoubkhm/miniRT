#include "../includes/minirt.h"

int parse_hyperboloid(t_scene *scene, char **tokens)
{
	t_hyperboloid *hyperboloid;
	char *texture_path;
	char *extension;
	int fd;

	if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
	{
		fprintf(stderr, "Error: Invalid hyperboloid definition\n");
		return (0);
	}
	hyperboloid = malloc(sizeof(t_hyperboloid));
	if (!hyperboloid)
		return (0);
	hyperboloid->base = parse_vector(tokens[1]);
	hyperboloid->axis = parse_vector(tokens[2]);
	hyperboloid->diameter = ft_atof(tokens[3]);
	hyperboloid->radius = ft_atof(tokens[3]) / 2.0;
	hyperboloid->height = ft_atof(tokens[4]);
	hyperboloid->color = parse_color(tokens[5]);
	texture_path = NULL;
	if (tokens[6] && tokens[6][0] != '#')
	{
		//printf("Checking texture path: %s\n", tokens[6]);
		extension = ft_strrchr(tokens[6], '.');
		if (extension && ft_strcmp(extension, ".xpm") == 0)
		{
			texture_path = tokens[6];
			fd = open(texture_path, O_RDONLY);
			if (fd == -1)
			{
				fprintf(stderr, "Error: Texture file '%s' not found.\n", texture_path);
				free(hyperboloid);
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		else
		{
			fprintf(stderr, "Error: Invalid file extension '%s' (only .xpm supported)\n", extension);
			free(hyperboloid);
			exit(EXIT_FAILURE);
		}
	}
	add_hyperboloid(scene, hyperboloid, texture_path);
	return (1);
}

void add_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid, char *texture_path)
{
	t_list *new_node = malloc(sizeof(t_list));
	t_object *object = malloc(sizeof(t_object));

	if (!new_node || !object)
	{
		free(hyperboloid);
		return;
	}
	object->type = HYPERBOLOID;
	object->data = hyperboloid;
	object->color = hyperboloid->color;
	if (texture_path)
	{
		object->texture = mlx_xpm_file_to_image(scene->mlx, texture_path, &object->tex_width, &object->tex_height);
		if (!object->texture)
		{
			fprintf(stderr, "Error\n");
			free(object);
			free(hyperboloid);
			free(new_node);
			exit(EXIT_FAILURE);
		}
		object->texture_data = mlx_get_data_addr(object->texture, &object->bpp, &object->line_len, &object->endian);
	}
	else
	{
		object->texture = NULL;
		object->texture_data = NULL;
	}
	new_node->content = object;
	new_node->next = scene->objects;
	scene->objects = new_node;
}
