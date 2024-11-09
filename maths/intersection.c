#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "mini_maths.h"

#define EPSILON 1e-6

double vector_length(t_vec v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

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
    // Normalisation de l'axe du cylindre
    t_vec axis = vector_normalize(cylinder.axis);

    // Calcul des composantes perpendiculaires
    t_vec delta_p = vector_sub(ray_origin, cylinder.base);
    t_vec v = vector_sub(ray_dir, scale_vec(axis, scalar_dot(ray_dir, axis)));
    t_vec delta_p_v = vector_sub(delta_p, scale_vec(axis, scalar_dot(delta_p, axis)));

    double a = scalar_dot(v, v);
    double b = 2 * scalar_dot(v, delta_p_v);
    double c = scalar_dot(delta_p_v, delta_p_v) - cylinder.radius * cylinder.radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return false;

    double sqrt_discriminant = sqrt(discriminant);
    double t0 = (-b - sqrt_discriminant) / (2 * a);
    double t1 = (-b + sqrt_discriminant) / (2 * a);

    // Assurer que t0 <= t1
    if (t0 > t1)
    {
        double temp = t0;
        t0 = t1;
        t1 = temp;
    }

    *t = INFINITY;

    // Vérifier les deux racines pour l'intersection avec la surface latérale
    for (int i = 0; i < 2; i++)
    {
        double t_candidate = (i == 0) ? t0 : t1;
        if (t_candidate < 0)
            continue; // Ignorer les intersections derrière l'origine du rayon

        t_vec hit_point = vector_add(ray_origin, scale_vec(ray_dir, t_candidate));
        double projection = scalar_dot(vector_sub(hit_point, cylinder.base), axis);

        if (projection >= 0 && projection <= cylinder.height)
        {
            if (t_candidate < *t)
                *t = t_candidate;
        }
    }

    // Tester l'intersection avec la base inférieure
    double denom = scalar_dot(ray_dir, axis);
    if (fabs(denom) > 1e-6)
    {
        double t_candidate = scalar_dot(vector_sub(cylinder.base, ray_origin), axis) / denom;
        if (t_candidate >= 0)
        {
            t_vec hit_point = vector_add(ray_origin, scale_vec(ray_dir, t_candidate));
            double dist = vector_length(vector_sub(hit_point, cylinder.base));
            if (dist <= cylinder.radius && t_candidate < *t)
                *t = t_candidate;
        }
    }

    // Tester l'intersection avec la base supérieure
    t_vec top_center = vector_add(cylinder.base, scale_vec(axis, cylinder.height));
    if (fabs(denom) > 1e-6)
    {
        double t_candidate = scalar_dot(vector_sub(top_center, ray_origin), axis) / denom;
        if (t_candidate >= 0)
        {
            t_vec hit_point = vector_add(ray_origin, scale_vec(ray_dir, t_candidate));
            double dist = vector_length(vector_sub(hit_point, top_center));
            if (dist <= cylinder.radius && t_candidate < *t)
                *t = t_candidate;
        }
    }
    //printf("Quadratic coefficients: a = %f, b = %f, c = %f, discriminant = %f\n", a, b, c, discriminant);

    // Retourner vrai si une intersection valide a été trouvée
    return (*t < INFINITY);
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



