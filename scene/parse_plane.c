#include "../includes/minirt.h"

int parse_plane(t_scene *scene, char **tokens)
{
    t_plane *plane;
    char *texture_path;
    char *extension;
    int fd;

    texture_path = NULL;
    if (!tokens[1] || !tokens[2] || !tokens[3])
    {
        fprintf(stderr, "Error: Invalid plane definition\n");
        return (0);
    }

    plane = malloc(sizeof(t_plane));
    if (!plane)
        return (0);

    plane->point = parse_vector(tokens[1]);
    plane->normal = parse_vector(tokens[2]);
    plane->color = parse_color(tokens[3]);
    if (tokens[4] && tokens[4][0] != '#')
    {
        //printf("Checking texture path: %s\n", tokens[4]);
        extension = ft_strrchr(tokens[4], '.');
        if (extension && ft_strcmp(extension, ".xpm") == 0)
        {
            texture_path = tokens[4];
            fd = open(texture_path, O_RDONLY);
            if (fd == -1) {
                fprintf(stderr, "Error: Texture file '%s' not found.\n", texture_path);
                free(plane);
                exit(EXIT_FAILURE);
            }
            close(fd);
        }
        else
        {
            fprintf(stderr, "Error: Invalid file extension '%s' (only .xpm supported)\n", extension);
            free(plane);
            exit(EXIT_FAILURE);
        }
    }
    else if (tokens[4] && tokens[4][0] == '#')
    {
        //printf("No texture path provided or it's a comment. Continuing without texture.\n");
    }

    add_plane(scene, plane, texture_path);
    return (1);
}

void add_plane(t_scene *scene, t_plane *plane, char *texture_path)
{
    t_list *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object)
    {
        free(plane);
        return;
    }
    object->type = PLANE;
    object->data = plane;
    object->color = plane->color;
    if (texture_path)
    {
        object->texture = mlx_xpm_file_to_image(scene->mlx, texture_path, &object->tex_width, &object->tex_height);
        if (!object->texture) {
            fprintf(stderr, "Error loading texture file: %s\n", texture_path);
            free(object);
            free(plane);
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
