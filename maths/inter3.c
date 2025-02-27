#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

#define EPSILON 1e-4

// ---------------------------------------------------------------------------
// Cylindre

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
    // Initialisation des UV par défaut
    hit->uv = (t_vec){0.0, 0.0, 0.0};
    // Initialisation des autres propriétés du matériau
    hit->material.reflectivity = 0.0;
    hit->material.transparency = 0.0;
    hit->material.refractive_index = 1.0;
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
    {
        free(value);
        return false;
    }
    fill_cylinder_hit_record(ray_origin, ray_dir, cylinder, value[3], candidate, hit, axis);
    free(value);
    return true;
}

// ---------------------------------------------------------------------------
// Sphère

double choose_sphere_intersection(double t0, double t1)
{
    if (t0 < t1)
        return t0;
    return t1;
}

void fill_sphere_hit_record(t_vec ray_origin, t_vec ray_dir, t_sphere *sphere, double t_val, t_hit *hit)
{
    // Calcul du point d'intersection
    hit->t = t_val;
    hit->point = vector_add(ray_origin, scale_vec(ray_dir, t_val));
    // Calcul de la normale : pour une sphère, c'est (point - center)
    hit->normal = vector_normalize(vector_sub(hit->point, sphere->center));
    hit->material.color = sphere->color;

    // Calcul du mapping sphérique pour les UV
    t_vec p = vector_normalize(vector_sub(hit->point, sphere->center));
    double phi = atan2(-p.z, p.x) + M_PI;
    double theta = acos(-p.y);
    hit->uv.x = phi / (2.0 * M_PI);
    hit->uv.y = 1.0 - (theta / M_PI);
    hit->uv.z = 0.0;

    // Initialisation des autres propriétés du matériau
    hit->material.reflectivity = 0.0;
    hit->material.transparency = 0.0;
    hit->material.refractive_index = 1.0;
}

bool intersect_sphere(t_vec ray_origin, t_vec ray_dir, t_sphere *sphere, t_hit *hit)
{
    double t0;
    double t1;
    double t_val;
    t_vec oc;

    oc = vector_sub(ray_origin, sphere->center);
    if (!solve_quadratic(scalar_dot(ray_dir, ray_dir),
                         2.0 * scalar_dot(oc, ray_dir),
                         scalar_dot(oc, oc) - (sphere->radius * sphere->radius),
                         &t0, &t1))
        return false;
    t_val = choose_sphere_intersection(t0, t1);
    if (t_val < EPSILON)
        return false;
    fill_sphere_hit_record(ray_origin, ray_dir, sphere, t_val, hit);
    return true;
}

// ---------------------------------------------------------------------------
// Hyperboloïde

t_vec rodrigues_rotate(t_vec v, t_vec k, double angle)
{
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    t_vec term1 = scale_vec(v, cos_a);
    t_vec term2 = scale_vec(vector_cross(k, v), sin_a);
    t_vec term3 = scale_vec(k, scalar_dot(k, v) * (1 - cos_a));
    return vector_add(vector_add(term1, term2), term3);
}

// bool intersect_hyperboloid(t_vec ray_origin, t_vec ray_dir, t_hyperboloid *hyperboloid, t_hit *hit)
// {
//     t_vec local_origin;
//     t_vec local_dir;
//     t_vec k = vector_normalize(hyperboloid->axis);
//     t_vec desired = {0, 0, 1};
//     double dot_val = scalar_dot(k, desired);
//     t_vec rotation_axis;
//     double angle;

//     if (fabs(dot_val - 1.0) < EPSILON)
//     {
//         local_origin = vector_sub(ray_origin, hyperboloid->base);
//         local_dir = ray_dir;
//         rotation_axis = (t_vec){0, 0, 0};
//         angle = 0;
//     }
//     else if (fabs(dot_val + 1.0) < EPSILON)
//     {
//         rotation_axis = (t_vec){1, 0, 0};
//         angle = M_PI;
//         local_origin = rodrigues_rotate(vector_sub(ray_origin, hyperboloid->base), rotation_axis, angle);
//         local_dir = rodrigues_rotate(ray_dir, rotation_axis, angle);
//     }
//     else
//     {
//         rotation_axis = vector_normalize(vector_cross(k, desired));
//         angle = acos(dot_val);
//         local_origin = rodrigues_rotate(vector_sub(ray_origin, hyperboloid->base), rotation_axis, angle);
//         local_dir = rodrigues_rotate(ray_dir, rotation_axis, angle);
//     }

//     double a = hyperboloid->radius * hyperboloid->radius;
//     double c = (hyperboloid->height / 2.0) * (hyperboloid->height / 2.0);
//     double A = (local_dir.x * local_dir.x + local_dir.y * local_dir.y) / a - (local_dir.z * local_dir.z) / c;
//     double B = 2.0 * ((local_origin.x * local_dir.x + local_origin.y * local_dir.y) / a - (local_origin.z * local_dir.z) / c);
//     double C = (local_origin.x * local_origin.x + local_origin.y * local_origin.y) / a - (local_origin.z * local_dir.z) / c - 1.0;

//     double discriminant = B * B - 4.0 * A * C;
//     if (discriminant < 0)
//         return false;
//     double sqrt_disc = sqrt(discriminant);
//     double t0 = (-B - sqrt_disc) / (2.0 * A);
//     double t1 = (-B + sqrt_disc) / (2.0 * A);
//     double t_val = (t0 > EPSILON) ? t0 : t1;
//     if (t_val < EPSILON)
//         return false;

//     t_vec local_hit_point = vector_add(local_origin, scale_vec(local_dir, t_val));
//     t_vec world_hit_point;
//     if (angle != 0)
//         world_hit_point = vector_add(hyperboloid->base, rodrigues_rotate(local_hit_point, rotation_axis, -angle));
//     else
//         world_hit_point = vector_add(hyperboloid->base, local_hit_point);

//     hit->t = t_val;
//     hit->point = world_hit_point;

//     t_vec local_normal;
//     local_normal.x = 2.0 * local_hit_point.x / a;
//     local_normal.y = 2.0 * local_hit_point.y / a;
//     local_normal.z = -2.0 * local_hit_point.z / c;
//     local_normal = vector_normalize(local_normal);

//     t_vec world_normal;
//     if (angle != 0)
//         world_normal = rodrigues_rotate(local_normal, rotation_axis, -angle);
//     else
//         world_normal = local_normal;
//     hit->normal = vector_normalize(world_normal);
//     hit->material.color = hyperboloid->color;
//     // Initialisation des UV par défaut
//     hit->uv = (t_vec){0.0, 0.0, 0.0};
//     // Initialisation des propriétés du matériau
//     hit->material.reflectivity = 0.0;
//     hit->material.transparency = 0.0;
//     hit->material.refractive_index = 1.0;
//     return true;
// }

bool intersect_hyperboloid(t_vec ray_origin, t_vec ray_dir, t_hyperboloid *hyperboloid, t_hit *hit)
{
    t_vec local_origin, local_dir;
    t_vec k = vector_normalize(hyperboloid->axis);
    t_vec desired = {0, 0, 1}; // Axe cible
    double dot_val = scalar_dot(k, desired);
    t_vec rotation_axis;
    double angle;

    if (fabs(dot_val - 1.0) < EPSILON)
    {
        local_origin = vector_sub(ray_origin, hyperboloid->base);
        local_dir = ray_dir;
        rotation_axis = (t_vec){0, 0, 0};
        angle = 0;
    }
    else if (fabs(dot_val + 1.0) < EPSILON)
    {
        rotation_axis = (t_vec){1, 0, 0};
        angle = M_PI;
        local_origin = rodrigues_rotate(vector_sub(ray_origin, hyperboloid->base), rotation_axis, angle);
        local_dir = rodrigues_rotate(ray_dir, rotation_axis, angle);
    }
    else
    {
        rotation_axis = vector_normalize(vector_cross(k, desired));
        angle = acos(dot_val);
        local_origin = rodrigues_rotate(vector_sub(ray_origin, hyperboloid->base), rotation_axis, angle);
        local_dir = rodrigues_rotate(ray_dir, rotation_axis, angle);
    }

    // ✅ Correction de la formule de l'hyperboloïde
    double a = hyperboloid->radius * hyperboloid->radius;
    double c = (hyperboloid->height / 2.0) * (hyperboloid->height / 2.0);

    double A = (local_dir.x * local_dir.x + local_dir.y * local_dir.y) / a - (local_dir.z * local_dir.z) / c;
    double B = 2.0 * ((local_origin.x * local_dir.x + local_origin.y * local_dir.y) / a - (local_origin.z * local_dir.z) / c);
    double C = (local_origin.x * local_origin.x + local_origin.y * local_origin.y) / a - (local_origin.z * local_origin.z) / c - 1.0;

    double discriminant = B * B - 4.0 * A * C;

    // ✅ Vérification correcte du discriminant
    if (discriminant < 0)
        return false;

    double sqrt_disc = sqrt(discriminant);
    double t0 = (-B - sqrt_disc) / (2.0 * A);
    double t1 = (-B + sqrt_disc) / (2.0 * A);

    // ✅ Sélection correcte du plus petit `t` positif
    double t_val = (t0 > EPSILON) ? t0 : ((t1 > EPSILON) ? t1 : -1);
    if (t_val < EPSILON)
        return false;

    // ✅ Correction de la transformation du point d'intersection
    t_vec local_hit_point = vector_add(local_origin, scale_vec(local_dir, t_val));
    t_vec world_hit_point = (angle != 0) ?
        vector_add(hyperboloid->base, rodrigues_rotate(local_hit_point, rotation_axis, -angle)) :
        vector_add(hyperboloid->base, local_hit_point);

    hit->t = t_val;
    hit->point = world_hit_point;

    // ✅ Correction du calcul de la normale
    t_vec local_normal = {
        2.0 * local_hit_point.x / a,
        2.0 * local_hit_point.y / a,
        -2.0 * local_hit_point.z / c
    };
    local_normal = vector_normalize(local_normal);

    t_vec world_normal = (angle != 0) ? rodrigues_rotate(local_normal, rotation_axis, -angle) : local_normal;
    hit->normal = vector_normalize(world_normal);

    // ✅ Ajout de propriétés correctes pour le matériau
    hit->material.color = hyperboloid->color;
    hit->uv = (t_vec){0.0, 0.0, 0.0};
    // hit->material.reflectivity = hyperboloid->reflectivity;
    // hit->material.transparency = hyperboloid->transparency;
    // hit->material.refractive_index = hyperboloid->refractive_index;

    return true;
}

// ---------------------------------------------------------------------------
// Paraboloïde

bool intersect_paraboloid(t_vec ray_origin, t_vec ray_dir, t_paraboloid *paraboloid, t_hit *hit)
{
    t_vec k = vector_normalize(paraboloid->axis);
    t_vec desired = {0, 0, 1};
    double dot_val = scalar_dot(k, desired);
    t_vec rotation_axis;
    double angle;
    t_vec local_origin, local_dir;

    if (fabs(dot_val - 1.0) < EPSILON) {
        local_origin = vector_sub(ray_origin, paraboloid->base);
        local_dir = ray_dir;
        angle = 0;
    }
    else if (fabs(dot_val + 1.0) < EPSILON) {
        rotation_axis = (t_vec){1, 0, 0};
        angle = M_PI;
        local_origin = rodrigues_rotate(vector_sub(ray_origin, paraboloid->base), rotation_axis, angle);
        local_dir = rodrigues_rotate(ray_dir, rotation_axis, angle);
    }
    else {
        rotation_axis = vector_normalize(vector_cross(k, desired));
        angle = acos(dot_val);
        local_origin = rodrigues_rotate(vector_sub(ray_origin, paraboloid->base), rotation_axis, angle);
        local_dir = rodrigues_rotate(ray_dir, rotation_axis, angle);
    }
    
    double A = local_dir.x * local_dir.x + local_dir.y * local_dir.y;
    double B = 2.0 * (local_origin.x * local_dir.x + local_origin.y * local_dir.y) - 2.0 * paraboloid->p * local_dir.z;
    double C = local_origin.x * local_origin.x + local_origin.y * local_origin.y - 2.0 * paraboloid->p * local_origin.z;
    
    double discriminant = B * B - 4.0 * A * C;
    if (discriminant < 0)
        return false;
    
    double sqrt_disc = sqrt(discriminant);
    double t0 = (-B - sqrt_disc) / (2.0 * A);
    double t1 = (-B + sqrt_disc) / (2.0 * A);
    double t_val = (t0 > EPSILON) ? t0 : t1;
    if (t_val < EPSILON)
        return false;
    
    t_vec local_hit_point = vector_add(local_origin, scale_vec(local_dir, t_val));
    
    t_vec world_hit_point;
    if (angle != 0)
        world_hit_point = vector_add(paraboloid->base, rodrigues_rotate(local_hit_point, rotation_axis, -angle));
    else
        world_hit_point = vector_add(paraboloid->base, local_hit_point);
    
    hit->t = t_val;
    hit->point = world_hit_point;
    
    t_vec local_normal = {2.0 * local_hit_point.x,
                          2.0 * local_hit_point.y,
                          -2.0 * paraboloid->p};
    local_normal = vector_normalize(local_normal);
    
    t_vec world_normal;
    if (angle != 0)
        world_normal = rodrigues_rotate(local_normal, rotation_axis, -angle);
    else
        world_normal = local_normal;
    
    hit->normal = vector_normalize(world_normal);
    hit->material.color = paraboloid->color;
    // Initialisation des UV par défaut
    hit->uv = (t_vec){0.0, 0.0, 0.0};
    // Initialisation des propriétés du matériau
    hit->material.reflectivity = 0.0;
    hit->material.transparency = 0.0;
    hit->material.refractive_index = 1.0;
    return true;
}

// ---------------------------------------------------------------------------
// Cône

bool intersect_cone(t_vec ray_origin, t_vec ray_dir, t_cone *cone, t_hit *hit)
{
    double t_lateral = INFINITY;
    double t_base = INFINITY;
    t_hit hit_lateral, hit_base;
    bool hitLateral = false, hitBase = false;
    double cos2 = cos(cone->angle) * cos(cone->angle);
    t_vec v = vector_sub(ray_origin, cone->vertex);
    
    // Intersection latérale du cône
    {
        double A = scalar_dot(ray_dir, cone->axis) * scalar_dot(ray_dir, cone->axis)
                   - cos2 * scalar_dot(ray_dir, ray_dir);
        double B = 2.0 * (scalar_dot(ray_dir, cone->axis) * scalar_dot(v, cone->axis)
                   - cos2 * scalar_dot(v, ray_dir));
        double C = scalar_dot(v, cone->axis) * scalar_dot(v, cone->axis)
                   - cos2 * scalar_dot(v, v);
        double discriminant = B * B - 4.0 * A * C;
        if (discriminant >= 0)
        {
            double sqrt_disc = sqrt(discriminant);
            double t0 = (-B - sqrt_disc) / (2.0 * A);
            double t1 = (-B + sqrt_disc) / (2.0 * A);
            double t_candidate = (t0 > EPSILON) ? t0 : t1;
            if (t_candidate > EPSILON)
            {
                t_vec P = vector_add(ray_origin, scale_vec(ray_dir, t_candidate));
                t_vec P_minus_V = vector_sub(P, cone->vertex);
                double t_proj = scalar_dot(P_minus_V, cone->axis);
                if (t_proj >= 0 && t_proj <= cone->height)
                {
                    t_lateral = t_candidate;
                    hit_lateral.t = t_candidate;
                    hit_lateral.point = P;
                    t_vec grad = vector_sub(scale_vec(cone->axis, 2.0 * scalar_dot(P_minus_V, cone->axis)),
                                             scale_vec(P_minus_V, 2.0 * cos2));
                    hit_lateral.normal = vector_normalize(grad);
                    hit_lateral.material.color = cone->color;
                    hitLateral = true;
                }
            }
        }
    }
    
    // Intersection avec la base du cône
    {
        t_vec base_center = vector_add(cone->vertex, scale_vec(cone->axis, cone->height));
        double denom = scalar_dot(ray_dir, cone->axis);
        if (fabs(denom) > EPSILON)
        {
            double t_candidate = scalar_dot(vector_sub(base_center, ray_origin), cone->axis) / denom;
            if (t_candidate > EPSILON)
            {
                t_vec P = vector_add(ray_origin, scale_vec(ray_dir, t_candidate));
                double base_radius = cone->height * tan(cone->angle);
                double dist = vector_length(vector_sub(P, base_center));
                if (dist <= base_radius)
                {
                    t_base = t_candidate;
                    hit_base.t = t_candidate;
                    hit_base.point = P;
                    hit_base.normal = scale_vec(vector_normalize(cone->axis), -1);
                    hit_base.material.color = cone->color;
                    hitBase = true;
                }
            }
        }
    }
    
    // Choix de la solution la plus proche
    if (hitLateral && hitBase)
    {
        if (t_lateral < t_base)
            *hit = hit_lateral;
        else
            *hit = hit_base;
        hit->uv = (t_vec){0.0, 0.0, 0.0};
        // Initialisation des propriétés du matériau
        hit->material.reflectivity = 0.0;
        hit->material.transparency = 0.0;
        hit->material.refractive_index = 1.0;
        return true;
    }
    else if (hitLateral)
    {
        *hit = hit_lateral;
        hit->uv = (t_vec){0.0, 0.0, 0.0};
        hit->material.reflectivity = 0.0;
        hit->material.transparency = 0.0;
        hit->material.refractive_index = 1.0;
        return true;
    }
    else if (hitBase)
    {
        *hit = hit_base;
        hit->uv = (t_vec){0.0, 0.0, 0.0};
        hit->material.reflectivity = 0.0;
        hit->material.transparency = 0.0;
        hit->material.refractive_index = 1.0;
        return true;
    }
    
    return false;
}
