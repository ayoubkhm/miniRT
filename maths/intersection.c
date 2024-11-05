#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "mini_maths.h"


double intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane plane, double *t)
{
    double denom = scalar_dot(plane.normal, ray_dir);

    if (fabs(denom) > 1e-6)
    {
        t_vec p0l0 = vector_sub(plane.point, ray_origin);
        *t = scalar_dot(p0l0, plane.normal) / denom;
        return (*t);
    }
    return 0;
}

// Fonction d'intersection pour le cylindre
double intersect_cylinder(t_vec ray_origin, t_vec ray_dir, t_cylinder cylinder, double *t)
{

        t_vec oc = vector_sub(ray_origin, cylinder.base);
        t_vec d = vector_normalize(cylinder.axis);

        t_vec oc_proj = vector_sub(oc, (t_vec){scalar_dot(oc, d) * d.x, scalar_dot(oc, d) * d.y, scalar_dot(oc, d) * d.z});
        t_vec ray_dir_proj = vector_sub(ray_dir, (t_vec){scalar_dot(ray_dir, d) * d.x, scalar_dot(ray_dir, d) * d.y, scalar_dot(ray_dir, d) * d.z});

        double a = scalar_dot(ray_dir_proj, ray_dir_proj);
        double b = 2 * scalar_dot(oc_proj, ray_dir_proj);
        double c = scalar_dot(oc_proj, oc_proj) - cylinder.radius * cylinder.radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) 
                return 0;

        double t0 = (-b - sqrt(discriminant)) / (2 * a);
        double t1 = (-b + sqrt(discriminant)) / (2 * a);
        *t = fmin(t0, t1);

        t_vec hit_point = (t_vec){ray_origin.x + *t * ray_dir.x, ray_origin.y + *t * ray_dir.y, ray_origin.z + *t * ray_dir.z};
        double height_check = scalar_dot(vector_sub(hit_point, cylinder.base), d);

        return (*t >= 0 && height_check >= 0 && height_check <= cylinder.height);
}

double intersect_sphere(t_vec ray_origin, t_vec ray_dir, t_sphere sphere, double *t)
{
    t_vec oc = vector_sub(ray_origin, sphere.center);
    double a = scalar_dot(ray_dir, ray_dir);
    double b = 2.0 * scalar_dot(oc, ray_dir);
    double c = scalar_dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0)
        return 0;
    *t = (-b - sqrt(discriminant)) / (2.0 * a);
    return (*t);
}



