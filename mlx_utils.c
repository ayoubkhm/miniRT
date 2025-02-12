#include "includes/minirt.h"
#include <stdio.h>    // Pour printf
#include <stdlib.h>   // Pour exit()
#include <sys/time.h>   // Pour gettimeofday()


void compute_camera_parameters(t_scene *scene)
{
    t_vec up = {0, 1, 0};

    scene->aspect_ratio = (float)WIDTH / (float)HEIGHT;
    scene->fov_rad = scene->camera.fov * M_PI / 180.0f;
    scene->viewport_height = 2.0f * tan(scene->fov_rad / 2.0f);
    scene->viewport_width = scene->viewport_height * scene->aspect_ratio;

    // Correction de l'ordre des produits vectoriels :
    scene->camera_direction = vector_normalize(scene->camera.ray.direction);
    scene->camera_right = vector_normalize(vector_cross(up, scene->camera_direction));
    scene->camera_up = vector_cross(scene->camera_direction, scene->camera_right);

    scene->pixel_size_x = scene->viewport_width / (float)WIDTH;
    scene->pixel_size_y = scene->viewport_height / (float)HEIGHT;
}

static double get_current_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void close_window(t_scene *scene) 
{
    mlx_destroy_window(scene->mlx, scene->win);
    exit(0);
}

int key_press(int keycode, t_scene *scene)
{
    // Debug : afficher la touche pressée
    printf("Key pressed: %d\n", keycode);

    if (keycode == 13 || keycode == 119) // W
        scene->move_forward = true;
    else if (keycode == 1 || keycode == 115) // S
        scene->move_backward = true;
    else if (keycode == 0 || keycode == 97) // A
        scene->move_right = true;
    else if (keycode == 2 || keycode == 100) // D
        scene->move_left = true;
    else if (keycode == 123 || keycode == 65361) // Flèche gauche
        scene->rotate_left = true;
    else if (keycode == 124 || keycode == 65363) // Flèche droite
        scene->rotate_right = true;
    else if (keycode == 126 || keycode == 65362) // Flèche haut
        scene->rotate_up = true;
    else if (keycode == 125 || keycode == 65364) // Flèche bas
        scene->rotate_down = true;
    else if (keycode == 53 || keycode == 65307) // Échap
        close_window(scene);

    return 0;
}

int key_release(int keycode, t_scene *scene)
{
    // Debug : afficher la touche relâchée
    printf("Key released: %d\n", keycode);

    if (keycode == 13 || keycode == 119) // W
        scene->move_forward = false;
    else if (keycode == 1 || keycode == 115) // S
        scene->move_backward = false;
    else if (keycode == 0 || keycode == 97) // A
        scene->move_left = false;
    else if (keycode == 2 || keycode == 100) // D
        scene->move_right = false;
    else if (keycode == 123 || keycode == 65361) // Flèche gauche
        scene->rotate_left = false;
    else if (keycode == 124 || keycode == 65363) // Flèche droite
        scene->rotate_right = false;
    else if (keycode == 126 || keycode == 65362) // Flèche haut
        scene->rotate_up = false;
    else if (keycode == 125 || keycode == 65364) // Flèche bas
        scene->rotate_down = false;
    return 0;
}

int update_camera_position(t_scene *scene)
{
    // On se sert d'une variable statique pour contrôler le taux de rendu.
    static double last_render_time = 0;
    double current_time = get_current_time();

    t_vec up = {0, 1, 0};

    // Met à jour la position en fonction des flags
    if (scene->move_forward)
        scene->camera.ray.origin = vector_add(scene->camera.ray.origin,
                                      scale_vec(scene->camera.ray.direction, MOVE_SPEED));
    if (scene->move_backward)
        scene->camera.ray.origin = vector_sub(scene->camera.ray.origin,
                                      scale_vec(scene->camera.ray.direction, MOVE_SPEED));

    t_vec left = vector_normalize(vector_cross(up, scene->camera.ray.direction));
    if (scene->move_left)
        scene->camera.ray.origin = vector_add(scene->camera.ray.origin,
                                      scale_vec(left, MOVE_SPEED));
    if (scene->move_right)
        scene->camera.ray.origin = vector_sub(scene->camera.ray.origin,
                                      scale_vec(left, MOVE_SPEED));

    // Met à jour la direction en fonction des flags de rotation
    if (scene->rotate_left)
        scene->camera.ray.direction = rotate_vec(scene->camera.ray.direction, up, -ROTATE_SPEED);
    if (scene->rotate_right)
        scene->camera.ray.direction = rotate_vec(scene->camera.ray.direction, up, ROTATE_SPEED);

    t_vec right = vector_normalize(vector_cross(scene->camera.ray.direction, up));
    if (scene->rotate_up)
        scene->camera.ray.direction = rotate_vec(scene->camera.ray.direction, right, ROTATE_SPEED);
    if (scene->rotate_down)
        scene->camera.ray.direction = rotate_vec(scene->camera.ray.direction, right, -ROTATE_SPEED);

    // Debug : afficher la nouvelle position et direction de la caméra
    printf("Updated Camera Position:\n");
    printf("  Origin:    (%f, %f, %f)\n",
           scene->camera.ray.origin.x,
           scene->camera.ray.origin.y,
           scene->camera.ray.origin.z);
    printf("  Direction: (%f, %f, %f)\n",
           scene->camera.ray.direction.x,
           scene->camera.ray.direction.y,
           scene->camera.ray.direction.z);

    // Recalcule les paramètres de la caméra (viewport, camera_right, camera_up, etc.)
    compute_camera_parameters(scene);

    /* On ne redessine la scène que si un certain délai s'est écoulé (ici 0.1 sec),
       pour éviter de lancer un rendu trop fréquent. */
    if (current_time - last_render_time > 0.1)
    {
         render_scene(scene);
         last_render_time = current_time;
    }

    return 0;
}