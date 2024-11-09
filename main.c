#include "includes/minirt.h"

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
    mlx_hook(scene.win, 2, 1L << 0, key_press, &scene);     // 2 est le code d'un keypress dans MiniLibX
    mlx_hook(scene.win, 3, 1L << 1, key_release, &scene);   // 3 est le code d'un keyrelease dans MiniLibX
    mlx_loop_hook(scene.mlx, (void *)update_camera_position, &scene);
    mlx_loop(scene.mlx);
    free_scene(&scene);
    return (EXIT_SUCCESS);
}
