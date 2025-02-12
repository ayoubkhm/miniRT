#include "../includes/mini_maths.h"

t_vec vector_cross(t_vec a, t_vec b)
{
    t_vec result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double vector_length(t_vec v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void    ft_swap(double *a, double *b)
{
    double tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}
