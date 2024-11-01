#ifndef TYPES_H
# define TYPES_H

// Définition du vecteur
typedef struct s_vec {
    double x;
    double y;
    double z;
} t_vec;

// Définition de la couleur
typedef struct s_color {
    int r;
    int g;
    int b;
} t_color;

// Définition du rayon
typedef struct s_ray {
    t_vec origin;
    t_vec direction;
} t_ray;

#endif
