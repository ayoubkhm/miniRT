#ifndef OBJECTS_H
# define OBJECTS_H

# include "../includes/types.h" // Inclure types.h pour t_vec et t_color
# include "lib/libft/libft.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_sphere {
    t_vec center;
    double radius;
    t_color color;
} t_sphere;

typedef struct s_plane {
    t_vec point;
    t_vec normal;
    t_color color;
} t_plane;

typedef struct s_cylinder {
    t_vec base;
    t_vec axis;
    double radius;
    double height;
    double diameter;
    t_color color;
} t_cylinder;

typedef struct s_hyperboloid {
    t_vec center;      // Centre de l'hyperboloïde
    t_vec axis;        // Axe de symétrie
    double a;          // Paramètre 'a' de l'équation
    double b;          // Paramètre 'b' de l'équation
    double c;          // Paramètre 'c' de l'équation
    double height;     // Hauteur de l'hyperboloïde
    t_color color;     // Couleur de l'hyperboloïde
} t_hyperboloid;


// Structure générique pour les objets
typedef enum e_object_type {
    SPHERE,
    PLANE,
    CYLINDER,
    HYPERBOLOID
} t_object_type;

typedef struct s_object {
    t_object_type type;
    void *data; // Pointeur vers t_sphere, t_plane ou t_cylinder
    t_color color;
} t_object;

#endif

