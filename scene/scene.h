#ifndef SCNENE_H
# define SCENE_H

# include "../includes/minirt.h"

typedef struct s_list {

    void    *obj;
    void    *next;
}               t_list;

typedef struct s_scene {

        t_color color;
        float   ratio;
}               t_scene;


#endif