#include "mini_maths.h"

t_vec vector_sub(t_vec a, t_vec b)
{
    return (t_vec){a.x - b.x, a.y - b.y, a.z - b.z};
}

t_vec vector_normalize(t_vec v)
{
    double mag = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return (t_vec){v.x / mag, v.y / mag, v.z / mag};
}

double scalar_dot(t_vec a, t_vec b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

t_vec vector_add(t_vec a, t_vec b)
{
    return (t_vec){a.x + b.x, a.y + b.y, a.z + b.z};
}

