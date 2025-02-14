#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

#define EPSILON 1e-6

bool compute_plane_intersection(t_vec ray_origin, t_vec ray_dir, t_plane *plane, double *t_val)
{
    double denom;

    denom = scalar_dot(plane->normal, ray_dir);
    if (fabs(denom) > EPSILON)
    {
        *t_val = scalar_dot(vector_sub(plane->point, ray_origin), plane->normal) / denom;
        if (*t_val > EPSILON)
            return true;
    }
    return false;
}

// 2. Remplit la structure t_hit pour un plan
void set_hit_info_plane(t_hit *hit, t_plane *plane, t_vec ray_origin, t_vec ray_dir, double t_val)
{
    hit->t = t_val;
    hit->point = vector_add(ray_origin, scale_vec(ray_dir, t_val));
    hit->normal = plane->normal;
    hit->material.color = plane->color;
}

// 3. Calcule les coordonnées UV pour un plan (ici, on suppose un plan horizontal)
void compute_plane_uv(t_plane *plane, t_hit *hit)
{
    double u;
    double v;
    
    u = (hit->point.x - plane->point.x) * 0.1;
    v = (hit->point.z - plane->point.z) * 0.1;

    u = u - floor(u);
    v = v - floor(v);
    hit->uv = (t_vec){u, v, 0};
}

bool intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane *plane, t_hit *hit)
{
    double t_val;

    if (!compute_plane_intersection(ray_origin, ray_dir, plane, &t_val))
        return false;
    set_hit_info_plane(hit, plane, ray_origin, ray_dir, t_val);
    compute_plane_uv(plane, hit);
    return true;
}


bool intersect_object(t_ray ray, t_object *object, t_hit *hit)
{
    int is_hit = 0;

    if (object->type == SPHERE)
        is_hit = intersect_sphere(ray.origin, ray.direction, (t_sphere *)object->data, hit);
    else if (object->type == PLANE)
        is_hit = intersect_plane(ray.origin, ray.direction, (t_plane *)object->data, hit);
    else if (object->type == CYLINDER)
        is_hit = intersect_cylinder(ray.origin, ray.direction, (t_cylinder *)object->data, hit);
    else if (object->type == HYPERBOLOID)
        is_hit = intersect_hyperboloid(ray.origin, ray.direction, (t_hyperboloid *)object->data, hit);
    else if (object->type == PARABOLOID)
        is_hit = intersect_paraboloid(ray.origin, ray.direction, (t_paraboloid *)object->data, hit);
    else if (object->type == CONE)
        is_hit = intersect_cone(ray.origin, ray.direction, (t_cone *)object->data, hit);    

    if (is_hit)
        hit->object = object;
    return is_hit;
}