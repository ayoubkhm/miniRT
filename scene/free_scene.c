#include "scene.h"

void free_scene(t_scene *scene)
{
    free_objects(scene->objects);
    free_lights(scene->lights);
    // Si vous avez alloué d'autres éléments, libérez-les ici
}

void free_objects(t_list *objects)
{
    t_list *current;
    t_list *next;
    t_object *object;

    current = objects;
    while (current) {
        next = current->next;
        object = (t_object *)current->content;

        // Libérer les données spécifiques de l'objet
        if (object->type == SPHERE)
            free(object->data); // Libère t_sphere
        else if (object->type == PLANE)
            free(object->data); // Libère t_plane
        else if (object->type == CYLINDER)
            free(object->data); // Libère t_cylinder
        else if (object->type == HYPERBOLOID)
            free(object->data); // Libère t_cylinder

        free(object);   // Libère t_object
        free(current);  // Libère le nœud de la liste
        current = next;
    }
}

void free_lights(t_list *lights)
{
    t_list *current;
    t_list *next;

    current = lights;
    while (current) {
        next = current->next;
        free(current->content); // Libère t_light
        free(current);          // Libère le nœud de la liste
        current = next;
    }
}
