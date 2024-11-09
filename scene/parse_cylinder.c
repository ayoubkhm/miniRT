#include "scene.h"

int parse_cylinder(t_scene *scene, char **tokens)
{
    t_cylinder *cylinder;

    if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5]) {
        fprintf(stderr, "Error: Invalid cylinder definition\n");
        return (0);
    }

    cylinder = malloc(sizeof(t_cylinder));
    if (!cylinder)
        return (0);

    cylinder->base = parse_vector(tokens[1]);
    cylinder->axis = parse_vector(tokens[2]);
    cylinder->diameter = ft_atof(tokens[3]);
    cylinder->radius = ft_atof(tokens[3]) / 2.0;
    cylinder->height = ft_atof(tokens[4]);
    cylinder->color = parse_color(tokens[5]);


    // printf("Cylinder defined:\n");
    // printf("  Base: {%.2f, %.2f, %.2f}\n", cylinder->base.x, cylinder->base.y, cylinder->base.z);
    // printf("  Axis: {%.2f, %.2f, %.2f}\n", cylinder->axis.x, cylinder->axis.y, cylinder->axis.z);
    // printf("  Radius: %.2f\n", cylinder->radius);
    // printf("  Height: %.2f\n", cylinder->height);
    // printf("  Color: {R:%d, G:%d, B:%d}\n", cylinder->color.r, cylinder->color.g, cylinder->color.b);

    add_cylinder(scene, cylinder);

    return (1);
}

void add_cylinder(t_scene *scene, t_cylinder *cylinder)
{
    t_list   *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object) {
        return;
    }

    object->type = CYLINDER;
    object->data = cylinder;
    object->color = cylinder->color;

    new_node->content = object;
    new_node->next = scene->objects;
    scene->objects = new_node;
}