#include "includes/minirt.h"
#include "scene/scene.h"
#include "camera/camera.h"
#include "scene/objects.h"
#include "maths/mini_maths.h"
#include <mlx.h>


#define WIDTH 800
#define HEIGHT 600

void close_window(t_scene *scene) {
    mlx_destroy_window(scene->mlx, scene->win);
    exit(0);
}

int key_press(int keycode, t_scene *scene) {
    if (keycode == 53) { // Touche Échap (code 53 sur Mac)
        close_window(scene);
    }
    // Mouvements WASD (ajuster les codes selon ton OS si nécessaire)
    else if (keycode == 13) { // W - Avancer
        scene->camera.position.z += 1;
    }
    else if (keycode == 1) { // S - Reculer
        scene->camera.position.z -= 1;
    }
    else if (keycode == 0) { // A - Gauche
        scene->camera.position.x -= 1;
    }
    else if (keycode == 2) { // D - Droite
        scene->camera.position.x += 1;
    }
    // Redessiner la scène après chaque mouvement
    render_scene(scene);
    return (0);
}

t_vec compute_ray(int i, int j, t_cam camera) {
    // Aspect ratio
    float aspect_ratio = (float)WIDTH / (float)HEIGHT;

    // Calcul de camera_right comme produit vectoriel entre le haut du monde et la direction de la caméra
//     t_vec world_up = {0, 0, 1};  // direction "haut" dans le monde 3D global
    t_vec camera_right = vector_normalize(vector_cross(camera.direction, (t_vec){0, 1, 0}));

    // Calcul de camera_up comme produit vectoriel entre camera_dir et camera_right
    t_vec camera_up = vector_normalize(vector_cross(camera.direction, camera_right));
    // Calcul des dimensions de la vue
    float viewport_height = 2.0 * tan(camera.fov / 2.0);
    float viewport_width = viewport_height * aspect_ratio;

    // Calcul des coordonnées x et y du pixel dans le plan de projection
    float x = (2 * (((float)i + 0.5) / WIDTH) - 1) * viewport_width / 2;
    float y = (1 - 2 * (((float)j + 0.5) / HEIGHT)) * viewport_height / 2;

    // Direction du rayon
    t_vec pixel_position = vector_add(camera.position, vector_add(
        scale_vec(camera_right, x),
        scale_vec(camera_up, y)
    ));
    t_vec ray_direction = vector_normalize(vector_sub(pixel_position, camera.position));

    // Retourne le rayon
    return ray_direction;
}

void render_scene(t_scene *scene, t_cylinder cyl, t_plane pl, void *mlx, void *win)
{
        int     i = 0;
        int     j = 0;
        int     col;

        while (j < HEIGHT)
        {
                i = 0;
                while (i < WIDTH)
                {
                        t_vec ray_dir = compute_ray(i, j, scene->camera);

                        double t_plane, t_cylinder;
                        bool hit_plane = intersect_plane(scene->camera.position, ray_dir, &pl, &t_plane);
                        bool hit_cylinder = intersect_cylinder(scene->camera.position, ray_dir, cyl, &t_cylinder);
                        // bool hit_sphere = intersect_sphere(scene->camera.position, ray_dir, sp, &t_sphere);

                        // if (hit_plane > 0 && hit_cylinder > 0) printf("\nplane : %d cylindre : %d\n", hit_plane > 0, hit_cylinder > 0);

                        t_color color = {255, 255, 255}; // Fond blanc par défaut ?¿

                        if (hit_plane && (!hit_cylinder || t_plane < t_cylinder))
                        {
                                if (pl.flag_qud)
                                {
                                        printf("Flags qud : %d\n", 1);
                                        color = pl.quadrillage;
                                }
                                else
                                        color = pl.color;
                        }
                        else if (hit_cylinder)
                                color = cyl.color;
                        col = color.b + (color.g << 8) + (color.r << 16);
                        mlx_pixel_put(mlx, win, i, j, col);
                        i++;
                }
                j++;
        }
}


int main(int argc, char **argv)
{
        t_scene         scene;
        t_plane         pl;
        t_cylinder      cyl;


        if (argc != 2)
                return (ft_error());
        init_scene(&scene);
        if (!load_scene(&scene, argv[1]))
                return (ft_error());

        // pl = *(t_plane *)scene.objects->content;
        // cyl = *(t_cylinder *)scene.objects->next->content;

        cyl.base = (t_vec){10, 0, 0};
        cyl.axis = (t_vec){0, 1, 0};
        cyl.color = (t_color){0, 0, 255};
        cyl.radius = 2;
        cyl.height = 0;

        pl.color  = (t_color){0, 255, 0};
        pl.quadrillage = (t_color){255 - pl.color.r, 255 - pl.color.g, 255 - pl.color.b};
        pl.normal = (t_vec){0, 1, 0};
        pl.point = (t_vec){0, 0, 0};


        printf("Scene loaded successfully:\n");
        printf("Camera position: %.1f, %.1f, %.1f\n", scene.camera.position.x, scene.camera.position.y, scene.camera.position.z);
        printf("Camera direction : %.1f, %.1f, %.1f\n", scene.camera.direction.x, scene.camera.direction.y, scene.camera.direction.z);
        printf("Ambient light ratio: %.1f\n", scene.ambient_light.ratio);
        printf("Scene loaded successfully:\n");
        printf("Cylinder propreties : axis = {%.1f, %.1f, %.1f}, base = {%.1f, %.1f, %.1f}", cyl.axis.x, cyl.axis.y, cyl.axis.z, cyl.base.x, cyl.base.y, cyl.base.z);
        printf(", radius = %.1f, hauteur = %.1f\n", cyl.radius, cyl.height);

        printf("\nplan props : normal ={%.1f, %.1f, %.1f} point = {%.1f, %.1f, %.1f}\n", pl.normal.x, pl.normal.y, pl.normal.z, pl.point.x, pl.point.y, pl.point.z);
        // mlx = mlx_init();
        // win = mlx_new_window(mlx, WIDTH, HEIGHT, "Cylindre et plan");

        render_scene(&scene, cyl, pl, mlx, win);


        mlx_loop(mlx);
        free_scene(&scene);
        return (EXIT_SUCCESS);
}
