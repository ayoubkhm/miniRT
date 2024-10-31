// main.c

#include "includes/minirt.h"

int main(int argc, char **argv) {
    t_scene scene;
    t_camera camera;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./minirt scene_file.rt\n");
        return (EXIT_FAILURE);
    }

    // Initialiser la scène
    init_scene(&scene);

    // Charger la scène à partir du fichier
    if (!load_scene(&scene, argv[1])) {
        fprintf(stderr, "Error loading scene\n");
        return (EXIT_FAILURE);
    }

    // Initialiser la caméra (géré par Mervan)
    init_camera(&camera, &scene);

    // Lancer le rendu (géré par Mervan)
    render_scene(&scene, &camera);

    // Boucle d'événements pour afficher l'image (géré par Mervan)
    mlx_loop(camera.mlx_ptr);

    // Libérer la mémoire
    free_scene(&scene);
    free_camera(&camera);

    return (EXIT_SUCCESS);
}