#include "scene.h"

int parse_hyperboloid(t_scene *scene, char **tokens)
{
    t_hyperboloid *hyperboloid;

    // Vérifier que tous les tokens nécessaires sont présents
    if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5]) {
        fprintf(stderr, "Error: Invalid hyperboloid definition\n");
        return (0);
    }

    // Allouer de la mémoire pour l'hyperboloïde
    hyperboloid = malloc(sizeof(t_hyperboloid));
    if (!hyperboloid)
        return (0);

    // Parser les valeurs des tokens
    hyperboloid->base = parse_vector(tokens[1]);
    hyperboloid->axis = parse_vector(tokens[2]);
    hyperboloid->diameter = ft_atof(tokens[3]);
    hyperboloid->radius = ft_atof(tokens[3]) / 2.0;
    hyperboloid->height = ft_atof(tokens[4]);
    hyperboloid->color = parse_color(tokens[5]);

    // Affichage pour vérification
    printf("Hyperboloid defined:\n");
    printf("  Base: {%.2f, %.2f, %.2f}\n", hyperboloid->base.x, hyperboloid->base.y, hyperboloid->base.z);
    printf("  Axis: {%.2f, %.2f, %.2f}\n", hyperboloid->axis.x, hyperboloid->axis.y, hyperboloid->axis.z);
    printf("  Radius: %.2f\n", hyperboloid->radius);
    printf("  Height: %.2f\n", hyperboloid->height);
    printf("  Color: {R:%d, G:%d, B:%d}\n", hyperboloid->color.r, hyperboloid->color.g, hyperboloid->color.b);

    // Ajouter l'hyperboloïde à la scène
    add_hyperboloid(scene, hyperboloid);

    return (1);
}

void add_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid)
{
    t_list *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object) {
        return;
    }

    object->type = HYPERBOLOID;
    object->data = hyperboloid;
    object->color = hyperboloid->color; // Copie la couleur

    new_node->content = object;
    new_node->next = scene->objects;
    scene->objects = new_node;
}
