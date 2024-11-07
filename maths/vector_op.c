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

t_vec  scale_vec(t_vec a, float k)
{
    return ((t_vec){k * a.x, k * a.y, k * a.z});
}

t_vec vector_cross(t_vec a, t_vec b)
{
    t_vec result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

