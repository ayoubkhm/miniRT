#ifndef OBJECTS_H
# define OBJECTS_H

# include "../includes/types.h" // Inclure types.h pour t_vec et t_color
# include "../lib/libft/libft.h"
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
    t_color quadrillage;
    bool flag_qud;
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
    t_vec base;
    t_vec axis;
    double diameter;
    double radius;
    double height;
    t_color color;
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
    void *data;
    t_color color;
    char *texture_path;
    void *texture;
    char *texture_data;
    int tex_width;
    int tex_height;
    int bpp;       
    int line_len;
    int endian;
}   t_object;

#endif