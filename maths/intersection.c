#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "mini_maths.h"

#define EPSILON 1e-6


// bool intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane plane, double *t)
// {
//     double denom = scalar_dot(plane.normal, ray_dir);

//     if (fabs(denom) > EPSILON)
//     {
//         t_vec paul = vector_sub(plane.point, ray_origin);
//         *t = scalar_dot(paul, plane.normal) / denom;
//         return (*t > 0);
//     }
//     return 0;
// }

bool intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane *plane, double *t)
{
    double denom = scalar_dot(plane->normal, ray_dir);

    if (fabs(denom) > EPSILON)
    {
        t_vec point_to_origin = vector_sub(plane->point, ray_origin);
        *t = scalar_dot(point_to_origin, plane->normal) / denom;

        if (*t > 0)
        {
            // Calcul du point d'intersection
            t_vec intersection = vector_add(ray_origin, scale_vec(ray_dir, *t));

            // Définir la taille d'une cellule du damier
            double cell_size = 6.0;

            // Calcul des indices de cellule (par rapport à x et z pour un plan horizontal)
            int cell_x = (int)floor(intersection.x / cell_size);
            int cell_z = (int)floor(intersection.z / cell_size);

            // Alternance du damier en utilisant la somme des indices de cellule
            plane->flag_qud = (cell_x + cell_z) % 2;

            return true;
        }
    }
    return false;
}

// bool intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane *plane, double *t)
// {
//     double denom = scalar_dot(plane->normal, ray_dir);

//     if (fabs(denom) > EPSILON)
//     {
//         t_vec paul = vector_sub(plane->point, ray_origin);
//         *t = scalar_dot(paul, plane->normal) / denom;

//         if (*t > 0)
//         {
//             // Point d'intersection
//             t_vec intersection = vector_add(ray_origin, scale_vec(ray_dir, *t));

//             // Taille de chaque cellule du quadrillage
//             double cell_size = 8.0;

//             // Déterminer si l'on est dans une cellule noire ou blanche
//             int cell_x = (int)(fabs(intersection.x) / cell_size);
//             int cell_z = (int)(fabs(intersection.z) / cell_size);

//             // Alternance quadrillage : si la somme des cellules est paire, on colorie différemment
//             plane->flag_qud = (cell_x + cell_z) % 2;

//             return true;
//         }
//     }
//     return false;
// }


// bool	cylinder_inter(t_vec ray_or, t_vec ray_dir, t_cylinder *cy, double *hit)
// {
// 	t_plane	pl;
// 	double	tmp_hit;

// 	*hit = INFINITY;
// 	pl.point = cy->base;
// 	pl.normal = cy->axis;
// 	if (intersect_plane(ray_or, ray_dir, pl, &tmp_hit)
// 		&& distance(tmp_hit, cy->base)
// 		<= cy->radius && *hit > tmp_hit.t)
// 		*hit = tmp_hit;
// 	pl.coords = cy->base;
// 	if (plane_inter(r, &pl, &tmp_hit)
// 		&& distance(tmp_hit.phit, cy->p2) <= cy->diameter * 0.5
// 		&& hit->t > tmp_hit.t)
// 		*hit = tmp_hit;
// 	if (infinite_cyl_inter(r, cy, &tmp_hit)
// 		&& pow(distance(cy->coords, tmp_hit.phit), 2)
// 		<= pow(cy->height * 0.5, 2) + cy->r2
// 		&& hit->t > tmp_hit.t)
// 		*hit = tmp_hit;
// 	return (hit->t < INFINITY && hit->t > EPSILON);
// }

// Fonction d'intersection pour le cylindre
bool intersect_cylinder(t_vec ray_origin, t_vec ray_dir, t_cylinder cylinder, double *t)
{

        t_vec oc = vector_sub(cylinder.base, ray_origin);
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
        // double t1 = (-b + sqrt(discriminant)) / (2 * a);
        // *t = fmin(t0, t1);
        *t = t0;

        t_vec hit_point = (t_vec){ray_origin.x + *t * ray_dir.x, ray_origin.y + *t * ray_dir.y, ray_origin.z + *t * ray_dir.z};
        double height_check = scalar_dot(vector_sub(hit_point, cylinder.base), d);

        return (*t >= 0 && height_check >= 0 && height_check <= cylinder.height);
}

bool intersect_sphere(t_vec ray_origin, t_vec ray_dir, t_sphere sphere, double *t)
{
    t_vec oc = vector_sub(ray_origin, sphere.center);
    double a = scalar_dot(ray_dir, ray_dir);
    double b = 2.0 * scalar_dot(oc, ray_dir);
    double c = scalar_dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = (b * b) - (4 * a * c);

    if (discriminant < 0)
        return 0;
    *t = fmin((-b - sqrt(discriminant)) / (2.0 * a), (-b + sqrt(discriminant)) / (2.0 * a));
    return (*t > 0);
}



