#include "../includes/minirt.h"

double compute_shadow(t_scene *scene, t_hit hit, t_light *light)
{
    t_ray shadow_ray;
    t_hit shadow_hit;
    int i;
    double light_distance;
    t_object *object;
    t_list *obj;

    shadow_ray.origin = vector_add(hit.point, scale_vec(hit.normal, 0.001));/* Décalage pour éviter l'auto-intersection */
    shadow_ray.direction = vector_normalize(vector_sub(light->position, hit.point));/* Pour une lumière ponctuelle, on calcule la direction vers la lumière */
    light_distance = vector_length(vector_sub(light->position, hit.point));/* Distance entre le point et la source lumineuse */
    obj = scene->objects;
    i = 0;
    while (i < scene->object_count)
    {
        object = obj->content;
        if (intersect_object(shadow_ray, object, &shadow_hit))
            if (shadow_hit.t > 0.001 && shadow_hit.t < light_distance)
                return (0.0);  // Le point est dans l'ombre
        i++;
        obj = obj->next;
    }
    return (1.0);  // Le point est entièrement éclairé par cette source
}

double compute_diffuse_single(t_scene *scene, t_hit hit, t_light *light)
{
    t_vec light_dir;

    (void)scene;
    /* Direction du rayon de lumière depuis le point d'intersection */
    light_dir = vector_normalize(vector_sub(light->position, hit.point));

    double diff = scalar_dot(hit.normal, light_dir);
    return (diff > 0 ? diff * light->ratio : 0);
}

double compute_specular_single(t_scene *scene, t_hit hit, t_ray ray, t_light *light)
{
    t_vec   light_dir;
    t_vec   view_dir;
    t_vec   reflect_dir;
    double  spec;

    (void)scene;
    light_dir = vector_normalize(vector_sub(light->position, hit.point));/* Calcul de la direction de la lumière (depuis le point d'intersection vers la lumière) */
    view_dir = vector_normalize(scale_vec(ray.direction, -1));/* La direction de vue est l'opposé de la direction du rayon incident */
    reflect_dir = reflect(scale_vec(light_dir, -1), hit.normal);/* Calcul du vecteur réfléchi à partir de la direction de la lumière */
    spec = scalar_dot(view_dir, reflect_dir);
    if (spec > 0)
    {
        double exponent = 0;
        if (exponent <= 0)
        {
            // Si non initialisé, on utilise une valeur par défaut
            exponent = 10.0;
        }
        spec = pow(spec, exponent) * light->ratio;
    }
    else
        spec = 0;
    return spec;
}
