#include "../includes/minirt.h"

// double compute_shadow(t_scene *scene, t_hit hit, t_light *light)
// {
//     t_ray shadow_ray;
//     t_hit shadow_hit;
//     int i;
//     double light_distance;
//     t_object *object;
//     t_list *obj;
//     double shadow_intensity = 1.0;

//     shadow_ray.origin = vector_add(hit.point, scale_vec(hit.normal, 0.0001)); // Éviter l'auto-intersection
//     shadow_ray.direction = vector_normalize(vector_sub(light->position, hit.point));
//     light_distance = vector_length(vector_sub(light->position, hit.point));

//     obj = scene->objects;
//     i = 0;
//     while (i < scene->object_count)
//     {
//         object = obj->content;
//         if (intersect_object(shadow_ray, object, &shadow_hit))
//         {
//             if (shadow_hit.t > 0.0001 && shadow_hit.t < light_distance)
//             {
//                 // Ombre adoucie au lieu d’un blocage total
//                 shadow_intensity *= 0.5;
//                 if (shadow_intensity < 0.1) // Empêcher les ombres trop foncées
//                     return 0.0;
//             }
//         }
//         i++;
//         obj = obj->next;
//     }
//     // Atténuation en fonction de la distance à la lumière
//     double distance_factor = 1.0 / (1.0 + 0.1 * light_distance * light_distance);
//     shadow_intensity *= distance_factor;
//     return shadow_intensity;
// }

double compute_shadow(t_scene *scene, t_hit hit, t_light *light)
{
    t_ray shadow_ray;
    t_hit shadow_hit;
    int i;
    double light_distance;
    t_object *object;
    t_list *obj;
    double shadow_intensity = 1.0;
    int samples = 50;  // Nombre d'échantillons pour l'ombre douce
    double sample_factor = 1.0 / samples;
    int shadow_hits = 0;

    // Décalage pour éviter l'auto-intersection
    shadow_ray.origin = vector_add(hit.point, scale_vec(hit.normal, EPSILON));

    for (int s = 0; s < samples; s++)
    {
        // Légère variation aléatoire de la position de la source lumineuse pour simuler une lumière étendue
        t_vec light_pos = vector_add(light->position, (t_vec){
            ((double)rand() / RAND_MAX - 0.5) * 0.2,
            ((double)rand() / RAND_MAX - 0.5) * 0.2,
            ((double)rand() / RAND_MAX - 0.5) * 0.2
        });

        shadow_ray.direction = vector_normalize(vector_sub(light_pos, hit.point));
        light_distance = vector_length(vector_sub(light_pos, hit.point));

        obj = scene->objects;
        i = 0;
        while (i < scene->object_count)
        {
            object = obj->content;
            if (intersect_object(shadow_ray, object, &shadow_hit))
            {
                if (shadow_hit.t > EPSILON && shadow_hit.t < light_distance)
                {
                    shadow_hits++;
                    break; // On arrête dès qu'on trouve une obstruction
                }
            }
            i++;
            obj = obj->next;
        }
    }

    // Ombre douce : réduction progressive de la lumière selon les échantillons bloqués
    shadow_intensity = 1.0 - (sample_factor * shadow_hits);
    return fmax(shadow_intensity, 0.2);  // Évite des ombres totalement noires
}

double compute_diffuse_single(t_scene *scene, t_hit hit, t_light *light)
{
    (void)scene;
    t_vec light_dir = vector_normalize(vector_sub(light->position, hit.point));
    double diff = scalar_dot(hit.normal, light_dir);

    // Atténuation douce
    if (diff < 0.1) diff = 0;
    return (diff > 0 ? diff * light->ratio : 0);
}

double compute_specular_single(t_scene *scene, t_hit hit, t_ray ray, t_light *light)
{
    t_vec light_dir = vector_normalize(vector_sub(light->position, hit.point));
    t_vec view_dir = vector_normalize(scale_vec(ray.direction, -1));
    t_vec reflect_dir = reflect(scale_vec(light_dir, -1), hit.normal);
    double spec = scalar_dot(view_dir, reflect_dir);
    (void)scene;

    if (spec > 0)
    {
        double exponent = hit.material.specular_exponent;
        if (exponent <= 0)
            exponent = 10.0; // Valeur par défaut si non initialisée
        spec = pow(spec, exponent) * light->ratio;
    }
    else
        spec = 0;

    return spec;
}
