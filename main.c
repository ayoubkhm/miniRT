#include "includes/minirt.h"
#include "scene/scene.h"
#include "camera/camera.h"
#include "scene/objects.h"
#include "maths/mini_maths.h"
#include <mlx.h>

#define WIDTH 800
#define HEIGHT 600

void close_window(t_scene *scene) 
{
    mlx_destroy_window(scene->mlx, scene->win);
    exit(0);
}

t_vec compute_ray(int i, int j, t_cam camera)
{
    float aspect_ratio = (float)WIDTH / (float)HEIGHT;
    float fov_rad = camera.fov * M_PI / 180.0;
    float viewport_height = 2.0 * tan(fov_rad / 2.0);
    float viewport_width = viewport_height * aspect_ratio;
    
    t_vec up = {0, 1, 0}; 
    t_vec camera_direction = vector_normalize(camera.direction);
    
    t_vec camera_right = vector_normalize(vector_cross(camera_direction, up));
    t_vec camera_up = vector_cross(camera_right, camera_direction);
    
    float pixel_ndc_x = (i + 0.5) / (float)WIDTH;
    float pixel_ndc_y = (j + 0.5) / (float)HEIGHT;
    float pixel_screen_x = 2 * pixel_ndc_x - 1;
    float pixel_screen_y = 1 - 2 * pixel_ndc_y;
    
    float pixel_camera_x = pixel_screen_x * viewport_width / 2.0;
    float pixel_camera_y = pixel_screen_y * viewport_height / 2.0;
    
    t_vec ray_direction = vector_normalize(
        vector_add(
            vector_add(
                scale_vec(camera_right, pixel_camera_x),
                scale_vec(camera_up, pixel_camera_y)
            ),
            camera_direction
        )
    );
    
    return ray_direction;
}

void render_scene(t_scene *scene)
{
    int i = 0;
    int j = 0;
    int col;

    printf("Starting render...\n");

    if (!scene->mlx || !scene->win)
    {
        fprintf(stderr, "Error: mlx or window not initialized.\n");
        return;
    }
    while (j < HEIGHT)
    {
        i = 0;
        while (i < WIDTH)
        {
            t_vec ray_dir = compute_ray(i, j, scene->camera);
            t_color color = {255, 255, 255};
            double t_min = INFINITY;

            t_list *current = scene->objects;
            while (current)
            {
                t_object *object = (t_object *)current->content;
                double t = INFINITY;
                bool hit = false;
                double u = 0, v = 0;

                // Intersection en fonction du type d'objet
                if (object->type == PLANE)
                {
                    hit = intersect_plane(scene->camera.position, ray_dir, (t_plane *)object->data, &t);
                    if (hit)
                    {
                        t_vec hit_point = vector_add(scene->camera.position, scale_vec(ray_dir, t));
                        u = fabs(hit_point.x) - floor(fabs(hit_point.x));
                        v = fabs(hit_point.z) - floor(fabs(hit_point.z));
                    }
                }
                else if (object->type == CYLINDER)
                {
                    hit = intersect_cylinder(scene->camera.position, ray_dir, *(t_cylinder *)object->data, &t);
                }
                else if (object->type == SPHERE)
                {
                    hit = intersect_sphere(scene->camera.position, ray_dir, *(t_sphere *)object->data, &t);
                    if (hit) {
                        t_sphere *sphere = (t_sphere *)object->data;
                        t_vec hit_point = vector_add(scene->camera.position, scale_vec(ray_dir, t));
                        t_vec hit_dir = vector_normalize(vector_sub(hit_point, sphere->center));
                        u = 0.5 + atan2(hit_dir.z, hit_dir.x) / (2 * M_PI);
                        v = 0.5 - asin(hit_dir.y) / M_PI;
                    }
                }

                // Si une intersection est trouvée et qu'elle est plus proche, appliquer la texture
                if (hit && t < t_min && t > 0)
                {
                    t_min = t;

                    if (object->texture_data) { // Texture présente
                        int tex_x = (int)(u * object->tex_width) % object->tex_width;
                        int tex_y = (int)(v * object->tex_height) % object->tex_height;
                        char *pixel = object->texture_data + (tex_y * object->line_len + tex_x * (object->bpp / 8));

                        int texture_color = *(int *)pixel;
                        color.r = (texture_color >> 16) & 0xFF;
                        color.g = (texture_color >> 8) & 0xFF;
                        color.b = texture_color & 0xFF;
                    }
                    else
                        color = object->color;
                }
                current = current->next;
            }

            // Conversion couleur pour `mlx_pixel_put`
            col = color.b + (color.g << 8) + (color.r << 16);

            // Dessin du pixel
            if (scene->mlx && scene->win)
                mlx_pixel_put(scene->mlx, scene->win, i, j, col);
            i++;
        }
        j++;
    }

    printf("Render completed.\n");
}




int key_press(int keycode, t_scene *scene)
{
    if (keycode == 53)
        close_window(scene);
    else if (keycode == 87)
        scene->camera.position.z += 1;
    else if (keycode == 1)
        scene->camera.position.z -= 1;
    else if (keycode == 0)
        scene->camera.position.x -= 1;
    else if (keycode == 2)
        scene->camera.position.x += 1;
    render_scene(scene);
    return (0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <scene_file.rt>\n", argv[0]);
        return (EXIT_FAILURE);
    }
    //printf("Initialisation\n");
    t_scene scene;
    init_scene(&scene);
    if (!load_scene(&scene, argv[1]))
    {
        printf("Error.\n");
        free_scene(&scene);
        return (EXIT_FAILURE);
    }
    //printf("Scene loaded successfully:\n");
    render_scene(&scene);
    mlx_key_hook(scene.win, key_press, &scene);
    mlx_loop(scene.mlx);
    free_scene(&scene);
    return (EXIT_SUCCESS);
}
