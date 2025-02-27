//main.c

#include "../includes/minirt.h"
#include <math.h>

// int trace_scene(t_scene *scene, t_ray ray, t_hit *hit)
// {
//     int found = 0;
//     int i;
//     double closest_t = DBL_MAX;
//     t_hit temp_hit;
//     t_list *obj = scene->objects;
//     t_object *object;
    
//     i = 0;
//     while (i < scene->object_count)
//     {
//         ft_memset(&temp_hit, 0, sizeof(t_hit));
//         object = (t_object *)obj->content;
//         if (intersect_object(ray, object, &temp_hit))
//         {
//             if (temp_hit.t < closest_t && temp_hit.t > EPSILON)
//             {
//                 closest_t = temp_hit.t;
//                 *hit = temp_hit;
//                 found = 1;
//             }
//         }
//         obj = obj->next;
//         i++;
//     }
//     return found;
// }

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
        ft_memset(&temp_hit, 0, sizeof(t_hit));
        object = (t_object *)obj->content;
        if (intersect_object(ray, object, &temp_hit))
        {
            // Correction de l'auto-intersection
            if (temp_hit.t < closest_t && temp_hit.t > (EPSILON * 10.0)) 
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


int main(int ac, char **av)
{
    t_scene scene;

    if (ac != 2)
    {
        printf("Usage: %s <scene_file.rt>\n", av[0]);
        return (EXIT_FAILURE);
    }
    init_scene(&scene);
    scene.accumulated_translation = (t_vec){0, 0, 0};
    scene.key_active = false;
    scene.rendering_pending = false;
    scene.last_key_press_time = 0;
    scene.last_key_release_time = 0;

    if (!load_scene(&scene, av[1]))
    {
        printf("Error.\n");
        free_scene(&scene);
        return (EXIT_FAILURE);
    }
    printf("Scene loaded successfully:\n");
    compute_camera_parameters(&scene);
    render_scene(&scene);
    mlx_hook(scene.win, 2, 1L << 0, key_press, &scene);
    mlx_hook(scene.win, 3, 1L << 1, key_release, &scene);
    mlx_loop_hook(scene.mlx, update_render_if_idle, &scene);
    mlx_loop(scene.mlx);
    free_scene(&scene);
    return (EXIT_SUCCESS);
}
