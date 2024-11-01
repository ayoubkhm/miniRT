#include "scene.h"

int parse_sphere(t_scene *scene, char **tokens)
{
    t_sphere *sphere;

    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        fprintf(stderr, "Error: Invalid sphere definition\n");
        return (0);
    }

    sphere = malloc(sizeof(t_sphere));
    if (!sphere)
        return (0);

    sphere->center = parse_vector(tokens[1]);
    sphere->radius = ft_atof(tokens[2]) / 2.0; // Diviser par 2 si le diamètre est donné
    sphere->color = parse_color(tokens[3]);

    add_sphere(scene, sphere);
    return (1);
}

void add_sphere(t_scene *scene, t_sphere *sphere)
{
    t_list *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object) {
        // Gérer l'erreur d'allocation
        return;
    }

    object->type = SPHERE;
    object->data = sphere;

    new_node->content = object;
    new_node->next = scene->objects;
    scene->objects = new_node;
}

int parse_plane(t_scene *scene, char **tokens)
{
    t_plane *plane;

    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        fprintf(stderr, "Error: Invalid plane definition\n");
        return (0);
    }

    plane = malloc(sizeof(t_plane));
    if (!plane)
        return (0);

    plane->point = parse_vector(tokens[1]);
    plane->normal = parse_vector(tokens[2]);
    plane->color = parse_color(tokens[3]);

    add_plane(scene, plane);
    return (1);
}

void add_plane(t_scene *scene, t_plane *plane)
{
    t_list   *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object) {
        // Gérer l'erreur d'allocation
        return;
    }

    object->type = PLANE;
    object->data = plane;

    new_node->content = object;
    new_node->next = scene->objects;
    scene->objects = new_node;
}

