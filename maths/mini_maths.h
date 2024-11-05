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

double intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane plane, double *t);
double intersect_cylinder(t_vec ray_origin, t_vec ray_dir, t_cylinder cylinder, double *t);





#endif