// objects.h

#ifndef OBJECTS_H
#define OBJECTS_H

typedef struct s_vec {
    double x;
    double y;
    double z;
} t_vec;

typedef struct s_color {
    int r;
    int g;
    int b;
} t_color;

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
    t_color color;
} t_cylinder;

#endif
