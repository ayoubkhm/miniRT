#include "includes/minirt.h"

void close_window(t_scene *scene) 
{
    mlx_destroy_window(scene->mlx, scene->win);
    exit(0);
}

int key_press(int keycode, t_scene *scene)
{
    if (keycode == 13 || keycode == 119) // W
        scene->move_forward = true;
    else if (keycode == 1 || keycode == 115) // S
        scene->move_backward = true;
    else if (keycode == 0 || keycode == 97) // A
        scene->move_left = true;
    else if (keycode == 2 || keycode == 100) // D
        scene->move_right = true;
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

    return (0);
}

int key_release(int keycode, t_scene *scene)
{
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
    return (0);
}

void update_camera_position(t_scene *scene)
{
    if (scene->move_forward)
        scene->camera.position = vector_add(scene->camera.position, scale_vec(scene->camera.direction, MOVE_SPEED));
    if (scene->move_backward)
        scene->camera.position = vector_sub(scene->camera.position, scale_vec(scene->camera.direction, MOVE_SPEED));
    if (scene->move_left)
    {
        t_vec left = vector_cross((t_vec){0, 1, 0}, scene->camera.direction);
        scene->camera.position = vector_add(scene->camera.position, scale_vec(vector_normalize(left), MOVE_SPEED));
    }
    if (scene->move_right)
    {
        t_vec right = vector_cross(scene->camera.direction, (t_vec){0, 1, 0});
        scene->camera.position = vector_add(scene->camera.position, scale_vec(vector_normalize(right), MOVE_SPEED));
    }
    if (scene->rotate_left)
        scene->camera.direction = rotate_vec(scene->camera.direction, (t_vec){0, 1, 0}, -ROTATE_SPEED);
    if (scene->rotate_right)
        scene->camera.direction = rotate_vec(scene->camera.direction, (t_vec){0, 1, 0}, ROTATE_SPEED);
    if (scene->rotate_up)
        scene->camera.direction = rotate_vec(scene->camera.direction, (t_vec){1, 0, 0}, ROTATE_SPEED);
    if (scene->rotate_down)
        scene->camera.direction = rotate_vec(scene->camera.direction, (t_vec){1, 0, 0}, -ROTATE_SPEED);
    //printf("Updated Camera Position: {x: %.2f, y: %.2f, z: %.2f}\n", scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
    render_scene(scene);
}
