#include "../includes/minirt.h"
#include <math.h>

#define EPSILON 1e-6

int trace_scene(t_scene *scene, t_ray ray, t_hit *hit)
{
    int found = 0;
    int i;
    double closest_t = DBL_MAX;
    t_hit temp_hit;
    t_list *obj = scene->objects;
    t_object *object;
    i = 0;
    
    while (i < scene->object_count)
    {
        object = (t_object *)obj->content;
        if (intersect_object(ray, object, &temp_hit))
        {
            if (temp_hit.t < closest_t && temp_hit.t > EPSILON)
            {
                closest_t = temp_hit.t;
                *hit = temp_hit;
                found = 1;
            }
        }
        obj = obj->next;
        i++;
    }
    return found;
}

void put_pixel(char *data, int x, int y, t_color color, int size_line)
{
    int index;

    index = y * size_line + x * 4;
    data[index] = color.b;
    data[index + 1] = color.g;
    data[index + 2] = color.r;
    data[index + 3] = 0;
}

void apply_gamma_correction(char *data, int width, int height, int size_line, double gamma)
{
    int x, y;
    double inv_gamma = 1.0 / gamma;
    
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            int index = y * size_line + x * 4;
            
            double b = (data[index] / 255.0);
            double g = (data[index + 1] / 255.0);
            double r = (data[index + 2] / 255.0);
            
            b = pow(b, inv_gamma);
            g = pow(g, inv_gamma);
            r = pow(r, inv_gamma);
            
            data[index]     = (unsigned char)(b * 255);
            data[index + 1] = (unsigned char)(g * 255);
            data[index + 2] = (unsigned char)(r * 255);
            // L'alpha reste inchangé.
        }
    }
}

// Dans render_scene (main.c par exemple)
void render_scene(t_scene *scene)
{
    int x, y;

    if (!scene->mlx || !scene->win || !scene->image || !scene->image_data)
    {
        fprintf(stderr, "Error: mlx, window, or image not initialized.\n");
        return;
    }

    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            process_by_pixel(scene, x, y);
        }
        // Affiche la progression par ligne (par exemple)
        printf("\rRendering: %d%%", (100 * y) / HEIGHT);
    }
    mlx_put_image_to_window(scene->mlx, scene->win, scene->image, 0, 0);
}

void print_scene_info(t_scene *scene)
{
    int index;
    t_list *temp;
    
    printf("=== CAMÉRA ===\n");
    printf("Origine: (%f, %f, %f)\n",
           scene->camera.ray.origin.x,
           scene->camera.ray.origin.y,
           scene->camera.ray.origin.z);
    printf("Direction: (%f, %f, %f)\n",
           scene->camera.ray.direction.x,
           scene->camera.ray.direction.y,
           scene->camera.ray.direction.z);
    printf("FOV: %f\n", scene->camera.fov);
    
    printf("=== LUMIÈRE AMBIANTE ===\n");
    printf("Ambient ratio: %f\n", scene->ambient_light.ratio);
    
    printf("=== LUMIÈRES ===\n");
    printf("Nombre de lumières: %d\n", scene->light_count);
    temp = scene->lights;
    index = 0;
    while (temp)
    {
        t_light *light = (t_light *)temp->content;
        if (!light)
            printf("Lumière %d: NULL\n", index);
        else
        {
            printf("Lumière %d:\n", index);
            printf("\tPosition: (%f, %f, %f)\n",
                   light->position.x,
                   light->position.y,
                   light->position.z);
            printf("\tCouleur: (R=%d, G=%d, B=%d)\n",
                   light->color.r, light->color.g, light->color.b);
            printf("\tRatio: %f\n", light->ratio);
        }
        temp = temp->next;
        index++;
    }
    
    printf("=== OBJETS ===\n");
    printf("Nombre d'objets: %d\n", scene->object_count);
    temp = scene->objects;
    index = 0;
    while (temp)
    {
        t_object *object = (t_object *)temp->content;
        if (!object)
            printf("Objet %d: NULL\n", index);
        else
        {
            printf("Objet %d:\n", index);
            printf("\tType: %d\n", object->type);
            // Vous pouvez ajouter ici un affichage plus détaillé selon le type d'objet.
        }
        temp = temp->next;
        index++;
    }
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Usage: %s <scene_file.rt>\n", av[0]);
        return (EXIT_FAILURE);
    }

    t_scene scene;
    init_scene(&scene);

    if (!load_scene(&scene, av[1]))
    {
        printf("Error.\n");
        free_scene(&scene);
        return (EXIT_FAILURE);
    }

    printf("Scene loaded successfully:\n");
    // <-- Appel nécessaire ici :
    compute_camera_parameters(&scene);
    
    render_scene(&scene);

    // Gestion des événements clavier :
    mlx_hook(scene.win, 2, 1L << 0, key_press, &scene);
    mlx_hook(scene.win, 3, 1L << 1, key_release, &scene);
    mlx_loop_hook(scene.mlx, (void *)update_camera_position, &scene);

    mlx_loop(scene.mlx);
    free_scene(&scene);
    return (EXIT_SUCCESS);
}