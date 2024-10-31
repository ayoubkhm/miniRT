#ifndef SCNENE_H
# define SCENE_H

# include "../includes/minirt.h"

typedef struct s_list {

    void    *obj;
    void    *next;
}               t_list;

typedef struct s_am_ligth {

        t_color color;
        double   ratio;
}               t_am_light;

typedef struct s_cam {

        double  fov;
        double  x;
        double  y;
        double  z;
        t_vec   dir;
}       t_cam;

typedef struct s_light 
{
        double  x;
        double  y;
        double  z;
        double  ratio;
        t_color color;
}               t_light;

typedef struct s_scene
{
        
};



#endif