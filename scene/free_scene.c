#include "scene.h"

void free_scene(t_scene *scene)
{
    free_objects(scene->objects);
    free_lights(scene->lights);
}

void free_objects(t_list *objects)
{
    t_list *current;
    t_list *next;
    t_object *object;

    current = objects;
    while (current)
    {
        next = current->next;
        object = (t_object *)current->content;
        if (object->type == SPHERE)
            free(object->data);
        else if (object->type == PLANE)
            free(object->data);
        else if (object->type == CYLINDER)
            free(object->data);
        else if (object->type == HYPERBOLOID)
            free(object->data);

        free(object);
        free(current);
        current = next;
    }
}

void free_lights(t_list *lights)
{
    t_list *current;
    t_list *next;

    current = lights;
    while (current)
    {
        next = current->next;
        free(current->content);
        free(current);
        current = next;
    }
}
