#include "scene.h"

t_hyperboloid *create_hyperboloid(t_vec center, t_vec axis, double a, double b, double c, double height, t_color color)
{
    t_hyperboloid *hyperboloid = malloc(sizeof(t_hyperboloid));
    if (!hyperboloid)
        return (NULL);
    hyperboloid->center = center;
    hyperboloid->axis = axis;
    hyperboloid->a = a;
    hyperboloid->b = b;
    hyperboloid->c = c;
    hyperboloid->height = height;
    hyperboloid->color = color;
    return (hyperboloid);
}

int parse_hyperboloid(t_scene *scene, char **tokens)
{
    if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5] || !tokens[6] || !tokens[7]) {
        fprintf(stderr, "Error: Invalid hyperboloid definition\n");
        return (0);
    }

    t_vec center = parse_vector(tokens[1]);
    t_vec axis = parse_vector(tokens[2]);
    double a = atof(tokens[3]);
    double b = atof(tokens[4]);
    double c = atof(tokens[5]);
    double height = atof(tokens[6]);
    t_color color = parse_color(tokens[7]);

    t_hyperboloid *hyperboloid = create_hyperboloid(center, axis, a, b, c, height, color);
    if (!hyperboloid)
        return (0);

    t_object *object = malloc(sizeof(t_object));
    if (!object) {
        free_hyperboloid(hyperboloid);
        return (0);
    }
    object->type = HYPERBOLOID;
    object->data = hyperboloid;
    object->color = color;

    ft_lstadd_front(&scene->objects, ft_lstnew(object));

    return (1);
}

void free_hyperboloid(t_hyperboloid *hyperboloid)
{
    free(hyperboloid);
}