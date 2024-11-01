#include "includes/minirt.h"
#include "scene/scene.h"
#include "camera/camera.h"
#include "scene/objects.h"


void init_grid_with_border(char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3])
{
    // Remplir la grille vide
    for (int y = 0; y < GRID_HEIGHT + 2; y++) {
        for (int x = 0; x < GRID_WIDTH + 2; x++) {
            grid[y][x] = ' ';
        }
        grid[y][GRID_WIDTH + 2] = '\0';  // Fin de ligne
    }

    // Dessiner les bordures
    for (int x = 1; x <= GRID_WIDTH; x++) {
        grid[0][x] = '-';               // Bordure du haut
        grid[GRID_HEIGHT + 1][x] = '-'; // Bordure du bas
    }
    for (int y = 1; y <= GRID_HEIGHT; y++) {
        grid[y][0] = '|';               // Bordure gauche
        grid[y][GRID_WIDTH + 1] = '|';  // Bordure droite
    }

    // Coins
    grid[0][0] = '+';
    grid[0][GRID_WIDTH + 1] = '+';
    grid[GRID_HEIGHT + 1][0] = '+';
    grid[GRID_HEIGHT + 1][GRID_WIDTH + 1] = '+';
}

void display_grid(char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3]) {
    for (int y = 0; y < GRID_HEIGHT + 2; y++) {
        printf("%s\n", grid[y]);
    }
}

void project_sphere(t_scene *scene, t_sphere *sphere, char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3], double depth_buffer[GRID_HEIGHT][GRID_WIDTH]) {
    double x = sphere->center.x - scene->camera.position.x;
    double y = sphere->center.y - scene->camera.position.y;
    double z = sphere->center.z - scene->camera.position.z;

    if (z >= 0) return;

    double fov_factor = tan((scene->camera.fov / 2) * (M_PI / 180));
    int screen_x = (int)((x / (-z * fov_factor)) * (GRID_WIDTH / 2)) + (GRID_WIDTH / 2);
    int screen_y = (int)((y / (-z * fov_factor)) * (GRID_HEIGHT / 2)) + (GRID_HEIGHT / 2);

    if (screen_x >= 0 && screen_x < GRID_WIDTH && screen_y >= 0 && screen_y < GRID_HEIGHT) {
        if (-z < depth_buffer[screen_y][screen_x]) {
            depth_buffer[screen_y][screen_x] = -z;
            grid[screen_y + 1][screen_x + 1] = 'O'; // Décalage pour éviter les bordures
        }
    }
}

void project_cylinder(t_scene *scene, t_cylinder *cylinder, char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3], double depth_buffer[GRID_HEIGHT][GRID_WIDTH]) {
    double x = cylinder->base.x - scene->camera.position.x;
    double y = cylinder->base.y - scene->camera.position.y;
    double z = cylinder->base.z - scene->camera.position.z;

    if (z >= 0) return;

    double fov_factor = tan((scene->camera.fov / 2) * (M_PI / 180));
    int screen_x = (int)((x / (-z * fov_factor)) * (GRID_WIDTH / 2)) + (GRID_WIDTH / 2);
    int screen_y = (int)((y / (-z * fov_factor)) * (GRID_HEIGHT / 2)) + (GRID_HEIGHT / 2);

    if (screen_x >= 0 && screen_x < GRID_WIDTH && screen_y >= 0 && screen_y < GRID_HEIGHT) {
        if (-z < depth_buffer[screen_y][screen_x]) {
            depth_buffer[screen_y][screen_x] = -z;
            grid[screen_y + 1][screen_x + 1] = 'Y'; // Marquer la base avec un décalage de 1
        }
    }

    for (int i = 1; i <= cylinder->height; i++) {
        double step_x = x + i * cylinder->axis.x;
        double step_y = y + i * cylinder->axis.y;
        double step_z = z + i * cylinder->axis.z;

        if (step_z >= 0) continue;

        int proj_x = (int)((step_x / (-step_z * fov_factor)) * (GRID_WIDTH / 2)) + (GRID_WIDTH / 2);
        int proj_y = (int)((step_y / (-step_z * fov_factor)) * (GRID_HEIGHT / 2)) + (GRID_HEIGHT / 2);

        if (proj_x >= 0 && proj_x < GRID_WIDTH && proj_y >= 0 && proj_y < GRID_HEIGHT) {
            if (-step_z < depth_buffer[proj_y][proj_x]) {
                depth_buffer[proj_y][proj_x] = -step_z;
                grid[proj_y + 1][proj_x + 1] = 'Y'; // Représenter une partie du cylindre avec décalage
            }
        }
    }
}

void project_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid, char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3], double depth_buffer[GRID_HEIGHT][GRID_WIDTH])
{
    // Projection simplifiée du centre de l'hyperboloïde
    double x = hyperboloid->center.x - scene->camera.position.x;
    double y = hyperboloid->center.y - scene->camera.position.y;
    double z = hyperboloid->center.z - scene->camera.position.z;

    if (z >= 0) return;

    double fov_factor = tan((scene->camera.fov / 2) * (M_PI / 180));
    int screen_x = (int)((x / (-z * fov_factor)) * (GRID_WIDTH / 2)) + (GRID_WIDTH / 2);
    int screen_y = (int)((y / (-z * fov_factor)) * (GRID_HEIGHT / 2)) + (GRID_HEIGHT / 2);

    if (screen_x >= 0 && screen_x < GRID_WIDTH && screen_y >= 0 && screen_y < GRID_HEIGHT) {
        if (-z < depth_buffer[screen_y][screen_x]) {
            depth_buffer[screen_y][screen_x] = -z;
            grid[screen_y + 1][screen_x + 1] = 'H'; // Représenter l'hyperboloïde par 'H'
        }
    }
}



void draw_scene(t_scene *scene) {
    char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3]; // Dimensions ajustées pour inclure les bordures
    double depth_buffer[GRID_HEIGHT][GRID_WIDTH];

    init_grid_with_border(grid);

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            depth_buffer[y][x] = 1e30;
        }
    }

    t_list *object_node = scene->objects;
    while (object_node) {
        t_object *object = (t_object *)object_node->content;
        if (object->type == SPHERE) {
            project_sphere(scene, (t_sphere *)object->data, grid, depth_buffer);
        } else if (object->type == CYLINDER) {
            project_cylinder(scene, (t_cylinder *)object->data, grid, depth_buffer);
        }
            else if (object->type == HYPERBOLOID)
            {
                project_hyperboloid(scene, (t_hyperboloid *)object->data, grid, depth_buffer);
            }

        object_node = object_node->next;
    }

    display_grid(grid);
}




void print_scene(t_scene *scene)
{
    // Afficher les informations de la caméra
    printf("Camera:\n");
    printf("  Position: %.2f, %.2f, %.2f\n", scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
    printf("  Direction: %.2f, %.2f, %.2f\n", scene->camera.direction.x, scene->camera.direction.y, scene->camera.direction.z);
    printf("  FOV: %.2f\n", scene->camera.fov);

    // Afficher les informations de la lumière ambiante
    printf("Ambient Light:\n");
    printf("  Ratio: %.2f\n", scene->ambient_light.ratio);
    printf("  Color: %d, %d, %d\n", scene->ambient_light.color.r, scene->ambient_light.color.g, scene->ambient_light.color.b);

    // Afficher les lumières ponctuelles
    printf("Lights:\n");
    t_list *light_node = scene->lights;
    while (light_node) {
        t_light *light = (t_light *)light_node->content;
        printf("  Light:\n");
        printf("    Position: %.2f, %.2f, %.2f\n", light->position.x, light->position.y, light->position.z);
        printf("    Ratio: %.2f\n", light->ratio);
        printf("    Color: %d, %d, %d\n", light->color.r, light->color.g, light->color.b);
        light_node = light_node->next;
    }

    // Afficher les objets
    printf("Objects:\n");
    t_list *object_node = scene->objects;
    while (object_node) {
        t_object *object = (t_object *)object_node->content;
        if (object->type == SPHERE) {
            t_sphere *sphere = (t_sphere *)object->data;
            printf("  Sphere:\n");
            printf("    Center: %.2f, %.2f, %.2f\n", sphere->center.x, sphere->center.y, sphere->center.z);
            printf("    Radius: %.2f\n", sphere->radius);
            printf("    Color: %d, %d, %d\n", sphere->color.r, sphere->color.g, sphere->color.b);
        } else if (object->type == PLANE) {
            t_plane *plane = (t_plane *)object->data;
            printf("  Plane:\n");
            printf("    Point: %.2f, %.2f, %.2f\n", plane->point.x, plane->point.y, plane->point.z);
            printf("    Normal: %.2f, %.2f, %.2f\n", plane->normal.x, plane->normal.y, plane->normal.z);
            printf("    Color: %d, %d, %d\n", plane->color.r, plane->color.g, plane->color.b);
        } else if (object->type == CYLINDER) {
            t_cylinder *cylinder = (t_cylinder *)object->data;
            printf("  Cylinder:\n");
            printf("    Base: %.2f, %.2f, %.2f\n", cylinder->base.x, cylinder->base.y, cylinder->base.z);
            printf("    Axis: %.2f, %.2f, %.2f\n", cylinder->axis.x, cylinder->axis.y, cylinder->axis.z);
            printf("    Diameter: %.2f\n", cylinder->diameter);
            printf("    Height: %.2f\n", cylinder->height);
            printf("    Color: %d, %d, %d\n", cylinder->color.r, cylinder->color.g, cylinder->color.b);
        } else {
            printf("  Unknown object type.\n");
        }
        object_node = object_node->next;
    }
}