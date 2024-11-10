#include "includes/minirt.h"
#define CHECKER_SIZE 2.5

t_vec rotate_vec(t_vec vec, t_vec axis, double angle)
{
    t_vec rotated;
    double cos_theta = cos(angle);
    double sin_theta = sin(angle);
    
    rotated.x = (cos_theta + (1 - cos_theta) * axis.x * axis.x) * vec.x +
                ((1 - cos_theta) * axis.x * axis.y - axis.z * sin_theta) * vec.y +
                ((1 - cos_theta) * axis.x * axis.z + axis.y * sin_theta) * vec.z;

    rotated.y = ((1 - cos_theta) * axis.y * axis.x + axis.z * sin_theta) * vec.x +
                (cos_theta + (1 - cos_theta) * axis.y * axis.y) * vec.y +
                ((1 - cos_theta) * axis.y * axis.z - axis.x * sin_theta) * vec.z;

    rotated.z = ((1 - cos_theta) * axis.z * axis.x - axis.y * sin_theta) * vec.x +
                ((1 - cos_theta) * axis.z * axis.y + axis.x * sin_theta) * vec.y +
                (cos_theta + (1 - cos_theta) * axis.z * axis.z) * vec.z;

    return vector_normalize(rotated);
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

t_color apply_checkerboard(t_vec point, t_color color1, t_color color2)
{
    int x_check = (int)(floor(point.x / CHECKER_SIZE)) % 2;
    int y_check = (int)(floor(point.y / CHECKER_SIZE)) % 2;
    int z_check = (int)(floor(point.z / CHECKER_SIZE)) % 2;
    
    if ((x_check + y_check + z_check) % 2 == 0)
        return color1;
    else
        return color2;
}

void render_scene(t_scene *scene)
{
    int i = 0, j = 0;
    t_vec ray_dir;
    t_color color; // Blanc par défaut
    t_vec   n = (t_vec){0, 0, 0};
    t_vec   l = (t_vec){0, 0, 0};
    double t_min;
    double t;
    t_object *object;
    bool hit;


    // Affiche la position de la caméra avant chaque rendu
    // printf("Camera position: {x: %.2f, y: %.2f, z: %.2f}\n",
        //    scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);

    if (!scene->mlx || !scene->win || !scene->image || !scene->image_data)
    {
        fprintf(stderr, "Error: mlx, window, or image not initialized.\n");
        return;
    }


    while (j < HEIGHT)
    {
        i = 0;
        while (i < WIDTH)
        {
            ray_dir = compute_ray(i, j, scene->camera);
            color = (t_color){255, 255, 255}; // Blanc par défaut
            t_min = INFINITY;

            t_list *current = scene->objects;
            while (current)
            {
                object = (t_object *)current->content;
                t = INFINITY;
                hit = false;
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
                    hit = intersect_cylinder(scene->camera.position, ray_dir, *(t_cylinder *)object->data, &t);
                else if (object->type == SPHERE)
                {
                    hit = intersect_sphere(scene->camera.position, ray_dir, *(t_sphere *)object->data, &t);
                    if (hit)
                    {
                        t_sphere *sphere = (t_sphere *)object->data;
                        n = vector_sub(scale_vec(scene->camera.position, t), sphere->center);
                        // n = scale_vec(sphere->center, t);
                        t_light light = *(t_light *)(scene->lights->content);
                        l = vector_sub(light.position, n);
                        n  = vector_normalize(n);
                        l = vector_normalize(l);
                        t_vec hit_point = vector_add(scene->camera.position, scale_vec(ray_dir, t));
                        t_vec hit_dir = vector_normalize(vector_sub(hit_point, sphere->center));
                        u = 0.5 + atan2(hit_dir.z, hit_dir.x) / (2 * M_PI);
                        v = 0.5 - asin(hit_dir.y) / M_PI;
                    }
                }
                else if (object->type == HYPERBOLOID)
                    hit = intersect_hyperboloid(scene->camera.position, ray_dir, (t_hyperboloid *)object->data, &t);

                // Détermination de la couleur en fonction de la texture ou du damier
                if (hit && t < t_min && t > 0)
                {
                    t_min = t;

                    if (object->texture_data)
                    {
                        int tex_x = (int)(u * object->tex_width) % object->tex_width;
                        int tex_y = (int)(v * object->tex_height) % object->tex_height;
                        char *pixel = object->texture_data + (tex_y * object->line_len + tex_x * (object->bpp / 8));
                        int texture_color = *(int *)pixel;
                        color.r = (texture_color >> 16) & 0xFF;
                        color.g = (texture_color >> 8) & 0xFF;
                        color.b = texture_color & 0xFF;
                    }
                    else if (object->flag_checkerboard)
                    {
                        t_vec hit_point = vector_add(scene->camera.position, scale_vec(ray_dir, t));
                        int checker_x = (int)(floor(hit_point.x / CHECKER_SIZE)) % 2;
                        int checker_y = (int)(floor(hit_point.y / CHECKER_SIZE)) % 2;
                        int checker_z = (int)(floor(hit_point.z / CHECKER_SIZE)) % 2;

                        color = ((checker_x + checker_y + checker_z) % 2 == 0) ? object->color : (t_color){0, 0, 0};
                    }
                    else
                    {
                        if (object->type == SPHERE)
                        {
                            print_position_or_vect(l, "Direction avec la lumiere");
                            print_position_or_vect(n, "Normal avec la sphere");
                            color = (t_color){object->color.r * fmax(scalar_dot(n, l), 0), object->color.g * fmax(scalar_dot(n, l), 0), object->color.b * fmax(scalar_dot(n, l), 0)};
                        }
                        else
                            color = object->color;

                    }
                }
                current = current->next;
            }

            // Calculer l'offset du pixel dans les données de l'image
            int offset = (j * scene->line_len + i * (scene->bpp / 8));
            if (offset >= 0 && offset + 3 < HEIGHT * scene->line_len)
            {
                scene->image_data[offset] = color.b;
                scene->image_data[offset + 1] = color.g;
                scene->image_data[offset + 2] = color.r;
            }
            i++;
        }
        j++;
    }

    mlx_put_image_to_window(scene->mlx, scene->win, scene->image, 0, 0);
    // printf("Render completed.\n");
}

