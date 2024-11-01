#include "scene.h"

void add_light(t_scene *scene, t_light *light)
{
    t_list *new_node = malloc(sizeof(t_list));

    if (!new_node) {
        // Gérer l'erreur d'allocation
        return;
    }

    new_node->content = light;
    new_node->next = scene->lights;
    scene->lights = new_node;
}