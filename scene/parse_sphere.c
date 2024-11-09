#include "scene.h"
#include <mlx.h>
#include <string.h>
#include <fcntl.h>

int parse_sphere(t_scene *scene, char **tokens)
{
    t_sphere *sphere;

    if (!tokens[1] || !tokens[2] || !tokens[3])
    {
        fprintf(stderr, "Error: Invalid sphere definition\n");
        return (0);
    }

    sphere = malloc(sizeof(t_sphere));
    if (!sphere)
        return (0);

    sphere->center = parse_vector(tokens[1]);
    sphere->radius = ft_atof(tokens[2]) / 2.0;
    sphere->color = parse_color(tokens[3]);

    char *texture_path = NULL;
    if (tokens[4] && tokens[4][0] != '#')
    {
        printf("Checking texture path: %s\n", tokens[4]);

        char *extension = strrchr(tokens[4], '.');
        if (extension && strcmp(extension, ".xpm") == 0)
        {
            texture_path = tokens[4];
            int fd = open(texture_path, O_RDONLY);
            if (fd == -1) {
                fprintf(stderr, "Error: Texture file '%s' not found.\n", texture_path);
                free(sphere);
                exit(EXIT_FAILURE);
            }
            close(fd);  // Ferme le fichier après vérification
        }
        else
        {
            fprintf(stderr, "Error: Invalid file extension '%s' (only .xpm supported)\n", extension);
            free(sphere);
            exit(EXIT_FAILURE);
        }
    }
    else if (tokens[4] && tokens[4][0] == '#')
    {
        //printf("No texture path provided or it's a comment. Continuing without texture.\n");
    }

    add_sphere(scene, sphere, texture_path);
    return (1);
}

void add_sphere(t_scene *scene, t_sphere *sphere, char *texture_path)
{
    t_list *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object)
    {
        free(sphere);
        return;
    }

    object->type = SPHERE;
    object->data = sphere;
    object->color = sphere->color;

    if (texture_path) 
    {
        object->texture = mlx_xpm_file_to_image(scene->mlx, texture_path, &object->tex_width, &object->tex_height);
        if (!object->texture) {
            fprintf(stderr, "Error\n");  // Affiche "Error" si la texture est invalide
            free(object);
            free(sphere);
            free(new_node);
            exit(EXIT_FAILURE);  // Arrête le programme
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
