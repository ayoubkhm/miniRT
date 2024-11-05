#include <math.h>
// #include "lib/mlx_macos/mlx.h"
#include <mlx.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    double x, y, z;
} Vector3;

typedef struct {
    Vector3 pos;
    double radius;
    int color;
} Sphere;

typedef struct {
    Vector3 pos;
    double fov;
} Camera;

typedef struct {
    void *mlx;
    void *win;
    Camera cam;
    Sphere sphere;
} Scene;

Vector3 vector_sub(Vector3 a, Vector3 b) {
    return (Vector3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector3 vector_normalize(Vector3 v) {
    double mag = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return (Vector3){v.x / mag, v.y / mag, v.z / mag};
}

double dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool intersect_sphere(Vector3 ray_origin, Vector3 ray_dir, Sphere sphere, double *t) {
    Vector3 oc = vector_sub(ray_origin, sphere.pos);
    double a = dot(ray_dir, ray_dir);
    double b = 2.0 * dot(oc, ray_dir);
    double c = dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0) return false;
    *t = (-b - sqrt(discriminant)) / (2.0 * a);
    return (*t >= 0);
}

void render_scene(Scene *scene) 
{
    if (!scene->win)
        return;
    int i = 0, j = 0, color;
    double u, v, t;
    Vector3 ray_dir;

    while (i < HEIGHT) 
    {
        j = 0;
        while (j < WIDTH) 
        {
            // Calculer direction du rayon pour chaque pixel
            u = (2.0 * j / WIDTH - 1) * tan(scene->cam.fov / 2.0) * (WIDTH / (double)HEIGHT);
            v = (1 - 2.0 * i / HEIGHT) * tan(scene->cam.fov / 2.0);
            ray_dir = vector_normalize((Vector3){u, v, -1});
            
                // Calculer couleur en fonction de l'éclairage (simple pour cet exemple)
            if (intersect_sphere(scene->cam.pos, ray_dir, scene->sphere, &t)) 
            {
                color = scene->sphere.color;
                mlx_pixel_put(scene->mlx, scene->win, j, i, color);
            } 
                // Afficher un fond noir
            else
                mlx_pixel_put(scene->mlx, scene->win, j, i, 0x000000);
            j++;
        }
        i++;
    }
}

int main() 
{
    Scene *scene;
    scene = malloc(sizeof(Scene));
    scene->mlx = mlx_init();
    if (!scene->mlx)
    {
        free(scene);   
        return 1;
    }
    scene->win = mlx_new_window(scene->mlx, WIDTH, HEIGHT, "miniRT");

    scene->cam.pos.x = 0;
    scene->cam.pos.y = 0;
    scene->cam.pos.z = 0;
    scene->cam.fov = M_PI / 3;

    scene->sphere.pos.x = 0;
    scene->sphere.pos.y = 0;
    scene->sphere.pos.z = -5;
    scene->sphere.radius = 1;
    scene->sphere.color = 0xFF0000; // Sphère rouge

    render_scene(scene);
    mlx_loop(scene->mlx);

    mlx_destroy_window(scene->mlx, scene->win);

    free(scene->mlx);
    free(scene);
    return (0);

}






