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

t_vec compute_ray(int i, int j, t_cam camera) {
    float aspect_ratio = (float)WIDTH / (float)HEIGHT;
    float fov_rad = camera.fov * M_PI / 180.0;
    float viewport_height = 2.0 * tan(fov_rad / 2.0);
    float viewport_width = viewport_height * aspect_ratio;
    
    t_vec up = {0, 1, 0}; // Vecteur "up" du monde
    t_vec camera_direction = vector_normalize(camera.direction);
    
    // Calcul des vecteurs de base de la caméra
    t_vec camera_right = vector_normalize(vector_cross(camera_direction, up));
    t_vec camera_up = vector_cross(camera_right, camera_direction); // Attention à l'ordre des arguments
    
    // Calcul des coordonnées x et y dans l'espace de la caméra
    float pixel_ndc_x = (i + 0.5) / (float)WIDTH;  // Normalized Device Coordinate
    float pixel_ndc_y = (j + 0.5) / (float)HEIGHT;
    float pixel_screen_x = 2 * pixel_ndc_x - 1;
    float pixel_screen_y = 1 - 2 * pixel_ndc_y;
    
    float pixel_camera_x = pixel_screen_x * viewport_width / 2.0;
    float pixel_camera_y = pixel_screen_y * viewport_height / 2.0;
    
    // Direction du rayon dans l'espace caméra
    t_vec ray_direction = vector_normalize(
        vector_add(
            vector_add(
                scale_vec(camera_right, pixel_camera_x),
                scale_vec(camera_up, pixel_camera_y)
            ),
            camera_direction // Ajoute la direction de la caméra
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

    if (!scene->mlx || !scene->win) {
        fprintf(stderr, "Error: mlx or window not initialized.\n");
        return;
    }

    // Parcours des objets de la scène pour vérifier leurs couleurs
    t_list *current = scene->objects;
    while (current) {
        t_object *object = (t_object *)current->content;

        // Affiche les informations de couleur pour chaque type d'objet une seule fois
        if (object->type == PLANE) {
            printf("Plane color: {R:%d, G:%d, B:%d}\n",
                   object->color.r, object->color.g, object->color.b);
        } else if (object->type == CYLINDER) {
            printf("Cylinder color: {R:%d, G:%d, B:%d}\n",
                   object->color.r, object->color.g, object->color.b);
        } else if (object->type == SPHERE) {
            printf("Sphere color: {R:%d, G:%d, B:%d}\n",
                   object->color.r, object->color.g, object->color.b);
        }

        current = current->next;
    }

    // Boucle pour chaque pixel de l'écran
    while (j < HEIGHT) {
        i = 0;
        while (i < WIDTH) {
            t_vec ray_dir = compute_ray(i, j, scene->camera);
            t_color color = {255, 255, 255}; // Fond blanc par défaut
            double t_min = INFINITY; // Distance minimale pour l'objet le plus proche

            // Parcours des objets de la scène pour le rendu
            current = scene->objects;
            while (current) {
                t_object *object = (t_object *)current->content;
                double t = INFINITY; // Distance d'intersection temporaire
                bool hit = false;    // Indicateur de collision

                // Vérification de l'intersection en fonction du type d'objet
                if (object->type == PLANE) {
                    hit = intersect_plane(scene->camera.position, ray_dir, (t_plane *)object->data, &t);
                } else if (object->type == CYLINDER) {
                    hit = intersect_cylinder(scene->camera.position, ray_dir, *(t_cylinder *)object->data, &t);
                } else if (object->type == SPHERE) {
                    hit = intersect_sphere(scene->camera.position, ray_dir, *(t_sphere *)object->data, &t);
                }

                // Si une intersection est trouvée et qu'elle est plus proche, mettre à jour `color`
                if (hit && t < t_min && t > 0) {
                    t_min = t;
                    color = object->color; // Utilise la couleur de l'objet
                }

                current = current->next;
            }

            // Conversion de la couleur en format RGB pour `mlx_pixel_put`
            col = color.b + (color.g << 8) + (color.r << 16);

            // Dessin du pixel
            if (scene->mlx && scene->win) {
                mlx_pixel_put(scene->mlx, scene->win, i, j, col);
            }

            i++;
        }
        j++;
    }

    printf("Render completed.\n");
}











int key_press(int keycode, t_scene *scene) {
    if (keycode == 53) { // Touche Échap (code 53 sur Mac)
        close_window(scene);
    } else if (keycode == 87) { // W - Avancer
        scene->camera.position.z += 1;
    } else if (keycode == 1) { // S - Reculer
        scene->camera.position.z -= 1;
    } else if (keycode == 0) { // A - Gauche
        scene->camera.position.x -= 1;
    } else if (keycode == 2) { // D - Droite
        scene->camera.position.x += 1;
    }
    render_scene(scene); // Appel avec un seul argument
    return (0);
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: %s <scene_file.rt>\n", argv[0]);
        return (EXIT_FAILURE);
    }

    printf("Initialisation\n");

    // Initialisation de la scène
    init_scene(&scene);

    // Chargement de la scène à partir du fichier
    if (!load_scene(&scene, argv[1])) {
        printf("Erreur lors du chargement de la scène.\n");
        free_scene(&scene);
        return (EXIT_FAILURE);
    }

    printf("Scene loaded successfully:\n");
    // Rendu de la scène
    render_scene(&scene);

    // Gestion des événements clavier et boucle MLX
    mlx_key_hook(scene.win, key_press, &scene);
    mlx_loop(scene.mlx);

    // Libération des ressources de la scène
    free_scene(&scene);
    return (EXIT_SUCCESS);
}
