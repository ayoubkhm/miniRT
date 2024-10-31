// objects.h

#ifndef OBJECTS_H
#define OBJECTS_H

typedef struct s_vector {
    double x;
    double y;
    double z;
} t_vector;

typedef struct s_color {
    int r;
    int g;
    int b;
} t_color;

typedef struct s_sphere {
    t_vector center;
    double radius;
    t_color color;
} t_sphere;

typedef struct s_plane {
    t_vector point;
    t_vector normal;
    t_color color;
} t_plane;

typedef struct s_cylinder {
    t_vector base;
    t_vector axis;
    double radius;
    double height;
    t_color color;
} t_cylinder;

#endif
