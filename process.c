#include "../includes/minirt.h"

// t_color combine_lighting(t_color base_color, double ambient, double diffuse, double specular, double reflection)
// {
//     base_color.r = (int)(base_color.r * (ambient + diffuse) + 255 * specular + 255 * reflection);
//     base_color.g = (int)(base_color.g * (ambient + diffuse) + 255 * specular + 255 * reflection);
//     base_color.b = (int)(base_color.b * (ambient + diffuse) + 255 * specular + 255 * reflection);
//     if (base_color.r > 255)
//         base_color.r = 255;
//     if (base_color.g > 255)
//         base_color.g = 255;
//     if (base_color.b > 255)
//         base_color.b = 255;
//     return (base_color);
// }

t_color combine_lighting(t_color base_color, double ambient, double diffuse, double specular, double reflection)
{
    // Convertir la couleur de base dans l'intervalle [0,1]
    double base_r = base_color.r / 255.0;
    double base_g = base_color.g / 255.0;
    double base_b = base_color.b / 255.0;

    /* 
     * On suppose que ambient, diffuse, specular et reflection sont des valeurs dans [0,1].
     * L'éclairage local est défini par (ambient + diffuse).
     * La contribution extra (spéculaire + reflets) vient s'ajouter.
     * 
     * Attention : Ces contributions doivent être équilibrées de sorte que le résultat final
     * ne dépasse pas 1.0 (pour éviter une saturation qui sera alors tronquée).
     */

    double local = (ambient + diffuse);
    if (local > 1.0)
        local = 1.0;
    
    double extra = 0.5 * specular + 0.5 * reflection;  // ou d'autres coefficients
    if (extra > 1.0)
        extra = 1.0;
    
    // Calcul de la couleur finale en espace [0,1]
    double final_r = base_r * local + extra;
    double final_g = base_g * local + extra;
    double final_b = base_b * local + extra;
    
    // Clamp entre 0 et 1
    if (final_r > 1.0) final_r = 1.0;
    if (final_g > 1.0) final_g = 1.0;
    if (final_b > 1.0) final_b = 1.0;
    
    // Reconversion dans l'intervalle [0,255]
    t_color final_color = {
        (int)(final_r * 255),
        (int)(final_g * 255),
        (int)(final_b * 255)
    };

    return final_color;
}

void light_processing(t_scene *scene, t_tools *t)
{
    int     i;
    double  shadow_factor;
    double  local_diffuse;
    double  local_specular;
    t_light *light;
    t_list  *lig;

    i = 0;
    t->base_color = t->hit.material.color;
    t->ambient = scene->ambient_light.ratio;  // Par exemple, une constante ou fonction d'occlusion ambiante
    t->diffuse = 0.0;
    t->specular = 0.0;
    lig = scene->lights;
    while (i < scene->light_count)
    {
        light = lig->content;
        shadow_factor = compute_shadow(scene, t->hit, light);         // Calcul du facteur d'ombre pour cette lumière
        local_diffuse = compute_diffuse_single(scene, t->hit, light);   // Éclairage diffus (modèle de Lambert)
        t->diffuse += local_diffuse * shadow_factor;
        local_specular = compute_specular_single(scene, t->hit, t->ray, light); // Éclairage spéculaire
        t->specular += local_specular * shadow_factor;
        i++;
        lig = lig->next;
    }
    t->final_color = combine_lighting(t->base_color, t->ambient, t->diffuse, t->specular, 0.0);
}

/*
// t_color get_background_color(t_scene *scene, t_ray ray)
// {
//     (void)scene;  // Si non utilisé
//     // Exemple : dégradé vertical basé sur la composante y de la direction du rayon
//     // t_color color;
//     // double t = 0.5 * (ray.direction.y + 1.0);
//     // color.r = (int)((1.0 - t) * 255 + t * 128);
//     // color.g = (int)((1.0 - t) * 255 + t * 178);
//     // color.b = (int)((1.0 - t) * 255 + t * 255);
//     
//     return (t_color){0, 0, 0};
// }
*/

t_color get_background_color(t_scene *scene, t_ray ray)
{
    (void)scene;  // Non utilisé ici

    t_color color;
    /* Calcul de t : 
       Si ray.direction.y est bien normalisé entre -1 et 1,
       alors t sera dans [0, 1] */
    double t = 0.5 * (ray.direction.y + 1.0);
    // Interpolation entre deux couleurs :
    // Pour t=0 -> couleur1 (blanc) et t=1 -> couleur2 (un bleu pastel)
    color.r = (int)((1.0 - t) * 255 + t * 128);
    color.g = (int)((1.0 - t) * 255 + t * 178);
    color.b = (int)((1.0 - t) * 255 + t * 255);

    // Clamp (normalement déjà dans [0, 255])
    if (color.r > 255) color.r = 255;
    if (color.g > 255) color.g = 255;
    if (color.b > 255) color.b = 255;
    
    return color;
}

t_color combine_recursive_lighting(t_color local_color, t_color reflect_color, t_color refract_color, double reflectivity, double transparency)
{
    t_color final_color;
    double local_weight = 1.0 - reflectivity - transparency;
    
    if (local_weight < 0)
        local_weight = 0.0;
    
    final_color.r = (int)(local_color.r * local_weight + reflect_color.r * reflectivity + refract_color.r * transparency);
    final_color.g = (int)(local_color.g * local_weight + reflect_color.g * reflectivity + refract_color.g * transparency);
    final_color.b = (int)(local_color.b * local_weight + reflect_color.b * reflectivity + refract_color.b * transparency);
    
    // Clamp pour rester dans [0, 255]
    if (final_color.r > 255) final_color.r = 255;
    if (final_color.g > 255) final_color.g = 255;
    if (final_color.b > 255) final_color.b = 255;
    
    return final_color;
}

t_vec reflect(t_vec I, t_vec N)
{
    double dot_value = scalar_dot(I, N);
    return vector_sub(I, scale_vec(N, 2.0 * dot_value));
}

/*
 * refract - Calcule la direction du rayon réfracté.
 * @I: Vecteur incident (normalisé).
 * @N: Normale à la surface au point d'intersection (normalisée).
 * @eta: Rapport des indices de réfraction (n_i / n_t). Par exemple, pour un rayon allant de l'air (1.0)
 *       vers du verre (1.5), eta = 1.0 / 1.5.
 * @refracted: Pointeur vers un t_vec dans lequel sera stocké le vecteur réfracté (normalisé).
 *
 * Retourne 1 si la réfraction est possible, ou 0 en cas de réflexion totale interne.
 */
int refract(t_vec I, t_vec N, double eta, t_vec *refracted)
{
    double cosi = -scalar_dot(I, N);
    double cost2 = 1.0 - eta * eta * (1.0 - cosi * cosi);
    if (cost2 < 0)
        return 0;  // Réflexion totale interne
    *refracted = vector_normalize(
                     vector_add(
                         scale_vec(I, eta),
                         scale_vec(N, eta * cosi - sqrt(cost2))
                     )
                 );
    return 1;
}

t_color trace_ray(t_scene *scene, t_ray ray, int depth)
{
    t_tools     t;
    t_color     reflec_color;
    t_color     refrac_color;

    reflec_color = (t_color){0, 0, 0};
    refrac_color = (t_color){0, 0, 0};
    if (depth > 0 && trace_scene(scene, ray, &t.hit)) // Test d'intersection + profondeur restante
    {
        /* Initialisation par défaut du matériau :
         * Si les valeurs n'ont pas été définies lors du parsing, on les met à 0 (ou 1 pour l'indice de réfraction).
         */
        if (t.hit.material.reflectivity < 0.0 || t.hit.material.reflectivity > 1.0)
            t.hit.material.reflectivity = 0.0;
        if (t.hit.material.transparency < 0.0 || t.hit.material.transparency > 1.0)
            t.hit.material.transparency = 0.0;
        if (t.hit.material.refractive_index <= 0.0)
            t.hit.material.refractive_index = 1.0;

        t.ray = ray; // Conserver le rayon courant dans la structure pour le calcul local
        t.base_color = t.hit.material.color;
        light_processing(scene, &t); // Calcul de l'éclairage local (ambiant, diffus, spéculaire)
        
        if (t.hit.material.reflectivity > 0.0) // Traitement de la réflexion
        {
            t_ray reflec;
            reflec.origin = vector_add(t.hit.point, scale_vec(t.hit.normal, 0.0001));
            reflec.direction = reflect(ray.direction, t.hit.normal);
            reflec_color = trace_ray(scene, reflec, depth - 1);
        }
        if (t.hit.material.transparency > 0.0) // Traitement de la réfraction
        {
            t_ray refrac;
            double eta;
            if (scalar_dot(ray.direction, t.hit.normal) < 0)
                eta = 1.0 / t.hit.material.refractive_index; // Rayon entrant : air -> matériau
            else
            {
                t.hit.normal = scale_vec(t.hit.normal, -1); // Rayon sortant : matériau -> air ; on inverse la normale
                eta = t.hit.material.refractive_index;
            }
            if (refract(ray.direction, t.hit.normal, eta, &refrac.direction)) // Calcul du rayon réfracté
            {
                refrac.origin = vector_add(t.hit.point, scale_vec(t.hit.normal, 0.001));
                refrac_color = trace_ray(scene, refrac, depth - 1);
            }
        }
        return combine_recursive_lighting(t.final_color, reflec_color, refrac_color, 
                                          t.hit.material.reflectivity, t.hit.material.transparency);
    }
    else
        return get_background_color(scene, ray);
}

void process_by_pixel(t_scene *scene, int x, int y)
{
    t_ray ray;
    t_color color;
    double curr;
    double total;

    curr = y * scene->line_len + x * 4;
    total = HEIGHT * scene->line_len + WIDTH * 4;
    // Utiliser la position dans scene->camera.ray.origin
    ray.origin = scene->camera.ray.origin;
    // Passez le pointeur vers la scène pour que compute_ray utilise les paramètres calculés
    ray.direction = compute_ray(x, y, scene);
    color = trace_ray(scene, ray, MAX_DEPTH); // Traçage récursif
    put_pixel(scene->image_data, x, y, color, scene->line_len);
    printf("\rRendering: %0.f%%", (100.0 * (double)curr / (double)total));
}
