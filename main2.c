#include "includes/minirt.h"
#include "scene/scene.h"
#include "camera/camera.h"
#include "scene/objects.h"
#include "maths/mini_maths.h"
#include <mlx.h>


#define WIDTH 800
#define HEIGHT 600

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
                        double u = (2.0 * i / WIDTH - 1) * tan(scene->camera.fov / 2.0) * (WIDTH / (double)HEIGHT);
                        double v = (1 - 2.0 * j / HEIGHT) * tan(scene->camera.fov / 2.0);
                        t_vec ray_dir = vector_normalize((t_vec){u, v, -1});

                        double t_plane, t_cylinder;
                        bool hit_plane = intersect_plane(scene->camera.position, ray_dir, pl, &t_plane) > 0;
                        bool hit_cylinder = intersect_cylinder(scene->camera.position, ray_dir, cyl, &t_cylinder) > 0;

                        t_color color = {255, 255, 255}; // Fond blanc par défaut ?¿

                        if (hit_plane && (!hit_cylinder || t_plane < t_cylinder))
                                color = pl.color;
                        else if (hit_cylinder)
                                color = cyl.color;
                        col = color.b + color.g * 8 + color.r * 16;
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

        void            *mlx;
        void            *win;

        if (argc != 2)
                return (ft_error());
        init_scene(&scene);
        if (!load_scene(&scene, argv[1]))
                return (ft_error());

        pl = *(t_plane *)scene.objects->content;
        cyl = *(t_cylinder *)scene.objects->next->content;

        printf("Scene loaded successfully:\n");
        printf("Camera position: %.1f, %.1f, %.1f\n", scene.camera.position.x, scene.camera.position.y, scene.camera.position.z);
        printf("Ambient light ratio: %.1f\n", scene.ambient_light.ratio);
        printf("Scene loaded successfully:\n");

        mlx = mlx_init();
        win = mlx_new_window(mlx, WIDTH, HEIGHT, "Cylindre et plan");

        render_scene(&scene, cyl, pl, mlx, win);


        mlx_loop(mlx);
        free_scene(&scene);
        return (EXIT_SUCCESS);
}
