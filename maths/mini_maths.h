#ifndef MINI_MATHS_H
# define MINI_MATHS_H

# include "../includes/types.h"
# include "../lib/libft/libft.h"
# include "objects.h"
# include <math.h>

t_vec vector_sub(t_vec a, t_vec b);
t_vec vector_normalize(t_vec v);
t_vec vector_add(t_vec a, t_vec b);
double scalar_dot(t_vec a, t_vec b);
t_vec  scale_vec(t_vec a, float k);
t_vec vector_cross(t_vec a, t_vec b);
bool intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane *plane, double *t);
bool intersect_cylinder(t_vec ray_origin, t_vec ray_dir, t_cylinder cylinder, double *t);
bool intersect_sphere(t_vec ray_origin, t_vec ray_dir, t_sphere sphere, double *t);
bool intersect_hyperboloid(t_vec ray_origin, t_vec ray_dir, t_hyperboloid *hyperboloid, double *t);

double vector_length(t_vec v);





#endif