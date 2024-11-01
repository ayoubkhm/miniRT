// objects.c

#include "../includes/minirt.h"
#include "objects.h"


t_sphere *create_sphere(t_vec center, double radius, t_color color) {
    t_sphere *sphere = malloc(sizeof(t_sphere));
    if (!sphere)
        return (NULL);
    sphere->center = center;
    sphere->radius = radius;
    sphere->color = color;
    return (sphere);
}

void free_sphere(t_sphere *sphere) {
    free(sphere);
}

// ... fonctions similaires pour les autres objets
