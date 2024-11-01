#include "includes/minirt.h"
#include "scene/scene.h"
#include "camera/camera.h"
#include "scene/objects.h"

int main(int argc, char **argv)
{
    t_scene scene;
    //t_camera camera;

    if (argc != 2) {
        ft_error();
        return (EXIT_FAILURE);
    }

    // Initialiser la scène
    init_scene(&scene);

    // Charger la scène à partir du fichier
    if (!load_scene(&scene, argv[1])) {
        ft_error();
        return (EXIT_FAILURE);
    }

    printf("Scene loaded successfully:\n");
    printf("Camera position: %.1f, %.1f, %.1f\n", scene.camera.position.x, scene.camera.position.y, scene.camera.position.z);
    printf("Ambient light ratio: %.1f\n", scene.ambient_light.ratio);

    printf("Scene loaded successfully:\n");
    print_scene(&scene);
    draw_scene(&scene);


    // Initialiser la caméra (géré par Mervan)
    //init_camera(&camera, &scene);

    // Lancer le rendu (géré par Mervan)
    //render_scene(&scene, &camera);

    // Boucle d'événements pour afficher l'image (géré par Mervan)
    //mlx_loop(camera.mlx_ptr);

    // Libérer la mémoire
    free_scene(&scene);
    //free_camera(&camera);

    return (EXIT_SUCCESS);
}
