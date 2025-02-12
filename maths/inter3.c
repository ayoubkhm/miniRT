#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

#define EPSILON 1e-6

void fill_cylinder_hit_record(t_vec ray_origin, t_vec ray_dir, t_cylinder *cylinder, double t_val, int candidate_type, t_hit *hit, t_vec axis)
{
    double projection;
    t_vec proj_point;

    hit->t = t_val;
    hit->point = vector_add(ray_origin, scale_vec(ray_dir, t_val));
    if (candidate_type == 0)
    {
        projection = scalar_dot(vector_sub(hit->point, cylinder->base), axis);
        proj_point = vector_add(cylinder->base, scale_vec(axis, projection));
        hit->normal = vector_normalize(vector_sub(hit->point, proj_point));
    }
    else if (candidate_type == 1)
        hit->normal = vector_normalize(scale_vec(axis, -1));
    else
        hit->normal = vector_normalize(axis);
    hit->material.color = cylinder->color;
}

bool intersect_cylinder(t_vec ray_origin, t_vec ray_dir, t_cylinder *cylinder, t_hit *hit)
{
    double *value;
    int candidate;
    t_vec axis;
    t_vec top_center;

    value = (double *)malloc(sizeof(double) * 4);
    if (!value)
        return false;
    axis = vector_normalize(cylinder->axis);
    value[0] = cylinder_lateral_intersection(ray_origin, ray_dir, cylinder, axis);
    value[1] = cylinder_cap_intersection(ray_origin, ray_dir, cylinder->base, cylinder->radius, axis);
    top_center = vector_add(cylinder->base, scale_vec(axis, cylinder->height));
    value[2] = cylinder_cap_intersection(ray_origin, ray_dir, top_center, cylinder->radius, axis);
    candidate = choose_cylinder_candidate(value[0], value[1], value[2], &value[3]);
    if (candidate < 0)
        return false;
    fill_cylinder_hit_record(ray_origin, ray_dir, cylinder, value[3], candidate, hit, axis);

    return true;
}

double choose_sphere_intersection(double t0, double t1)
{
    if (t0 < t1)
        return t0;
    return t1;
}

void fill_sphere_hit_record(t_vec ray_origin, t_vec ray_dir, t_sphere *sphere, double t_val, t_hit *hit)
{
    hit->t = t_val;
    hit->point = vector_add(ray_origin, scale_vec(ray_dir, t_val));
    hit->normal = vector_normalize(vector_sub(hit->point, sphere->center));
    hit->material.color = sphere->color;
}

bool intersect_sphere(t_vec ray_origin, t_vec ray_dir, t_sphere *sphere, t_hit *hit)
{
    double  t0;
    double  t1;
    double  t_val;
    t_vec   oc;

    oc = vector_sub(ray_origin, sphere->center);
    if (!solve_quadratic(scalar_dot(ray_dir, ray_dir), 2.0 * scalar_dot(oc, ray_dir), scalar_dot(oc, oc) - (sphere->radius * sphere->radius), &t0, &t1))
        return false;
    t_val = choose_sphere_intersection(t0, t1);
    if (t_val < EPSILON)
        return false;
    fill_sphere_hit_record(ray_origin, ray_dir, sphere, t_val, hit);
    return true;
}
