#ifndef MINIRT_H
# define MINIRT_H

#include "minirt.h"
# include "lib/mlx/include/MLX42/MLX42.h"
# include "lib/libft/libft.h"

typedef struct s_vec {
    double x;
    double y;
    double z;
} t_vec;

typedef struct s_ray {
    t_vec origin;
    t_vec direction;
} t_ray;

#endif