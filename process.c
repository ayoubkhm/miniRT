// process.c

#include "../includes/minirt.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//--------------------------------------------------------------------------
// Fonctions utilitaires pour le checkerboard, l'éclairage et le mélange des couleurs

t_color get_checkerboard_color(t_vec uv, int scale, t_color color1, t_color color2)
{
    int u = (int)(uv.x * scale);
    int v = (int)(uv.y * scale);
    
    // Si la somme des indices est paire, renvoyer color1, sinon color2.
    if ((u + v) % 2 == 0)
        return color1;
    else
        return color2;
}

t_color combine_lighting(t_color base_color, double ambient, double diffuse, double specular, double reflection)
{
    double base_r = base_color.r / 255.0;
    double base_g = base_color.g / 255.0;
    double base_b = base_color.b / 255.0;

    // Limitation des valeurs pour éviter une saturation excessive
    if (diffuse > 1.0) diffuse = 1.0;
    if (specular > 1.0) specular = 1.0;
    if (reflection > 1.0) reflection = 1.0;

    double local = ambient + diffuse;
    if (local > 1.0) local = 1.0;

    double extra = specular + reflection;
    if (extra > 1.0) extra = 1.0;

    double final_r = base_r * local + extra * 0.7; // Ajustement pour éviter la surexposition
    double final_g = base_g * local + extra * 0.7;
    double final_b = base_b * local + extra * 0.7;

    // Clamping final
    final_r = fmin(1.0, final_r);
    final_g = fmin(1.0, final_g);
    final_b = fmin(1.0, final_b);

    t_color final_color = {
        (int)(final_r * 255),
        (int)(final_g * 255),
        (int)(final_b * 255)
    };

    return final_color;
}

void light_processing(t_scene *scene, t_tools *t)
{
    int i;
    double shadow_factor;
    double local_diffuse;
    double local_specular;
    t_light *light;
    t_list *lig;

    i = 0;
    // Ne pas écraser t->base_color si l'objet utilise une texture ou un checkerboard.
    if (!t->hit.object->texture_data && !t->hit.object->is_checkerboard)
        t->base_color = t->hit.material.color;

    t->ambient = scene->ambient_light.ratio;
    t->diffuse = 0.0;
    t->specular = 0.0;
    lig = scene->lights;
    while (i < scene->light_count)
    {
        light = lig->content;
        shadow_factor = compute_shadow(scene, t->hit, light);
        local_diffuse = compute_diffuse_single(scene, t->hit, light);
        t->diffuse += local_diffuse * shadow_factor;
        local_specular = compute_specular_single(scene, t->hit, t->ray, light);
        t->specular += local_specular * shadow_factor;
        i++;
        lig = lig->next;
    }
    t->final_color = combine_lighting(t->base_color, t->ambient, t->diffuse, t->specular, 0.0);
}

t_color get_background_color(t_scene *scene, t_ray ray)
{
    (void)scene;  // Non utilisé ici
    t_color color;
    double t = 0.5 * (ray.direction.y + 1.0);
    color.r = (int)((1.0 - t) * 255 + t * 128);
    color.g = (int)((1.0 - t) * 255 + t * 178);
    color.b = (int)((1.0 - t) * 255 + t * 255);
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

//--------------------------------------------------------------------------
// Version d'anti-aliasing de compute_ray qui accepte des coordonnées float

t_vec compute_ray_aa(float i, float j, t_scene *scene)
{
    float pixel_ndc_x = (i + 0.5f) / (float)WIDTH;
    float pixel_ndc_y = (j + 0.5f) / (float)HEIGHT;
    float pixel_screen_x = 2.0f * pixel_ndc_x - 1.0f;
    float pixel_screen_y = 1.0f - 2.0f * pixel_ndc_y;
    float pixel_camera_x = pixel_screen_x * scene->viewport_width / 2.0f;
    float pixel_camera_y = pixel_screen_y * scene->viewport_height / 2.0f;
    t_vec ray_direction = vector_normalize(
        vector_add(
            vector_add(
                scale_vec(scene->camera_right, pixel_camera_x),
                scale_vec(scene->camera_up, pixel_camera_y)
            ),
            scene->camera_direction
        )
    );
    return ray_direction;
}

//--------------------------------------------------------------------------
// Fonction trace_ray (inchangée)

t_color trace_ray(t_scene *scene, t_ray ray, int depth)
{
    t_tools t;
    t_color reflec_color;
    t_color refrac_color;

    memset(&t, 0, sizeof(t_tools));
    reflec_color = (t_color){0, 0, 0};
    refrac_color = (t_color){0, 0, 0};
    if (depth > 0 && trace_scene(scene, ray, &t.hit))
    {
        if (t.hit.material.reflectivity < 0.0 || t.hit.material.reflectivity > 1.0)
            t.hit.material.reflectivity = 0.0;
        if (t.hit.material.transparency < 0.0 || t.hit.material.transparency > 1.0)
            t.hit.material.transparency = 0.0;
        if (t.hit.material.refractive_index <= 0.0)
            t.hit.material.refractive_index = 1.0;

        t.ray = ray; // Rayon courant
        t.base_color = t.hit.material.color;
        
        if (t.hit.object->is_checkerboard)
        {
            t.base_color = get_checkerboard_color(t.hit.uv, 10, (t_color){255,255,255}, (t_color){0,0,0});
            printf("Checkerboard applied: UV=(%f, %f) -> base color set to (%d, %d, %d)\n",
                   t.hit.uv.x, t.hit.uv.y,
                   t.base_color.r, t.base_color.g, t.base_color.b);
        }
        else if (t.hit.object->texture_data)
        {
            t.base_color = get_texture_color(t.hit.object, t.hit.uv);
        }

        light_processing(scene, &t);

        if (t.hit.material.reflectivity > 0.0)
        {
            t_ray reflec;
            reflec.origin = vector_add(t.hit.point, scale_vec(t.hit.normal, 0.0001));
            reflec.direction = reflect(ray.direction, t.hit.normal);
            reflec_color = trace_ray(scene, reflec, depth - 1);
        }
        if (t.hit.material.transparency > 0.0)
        {
            t_ray refrac;
            double eta;
            if (scalar_dot(ray.direction, t.hit.normal) < 0)
                eta = 1.0 / t.hit.material.refractive_index;
            else
            {
                t.hit.normal = scale_vec(t.hit.normal, -1);
                eta = t.hit.material.refractive_index;
            }
            if (refract(ray.direction, t.hit.normal, eta, &refrac.direction))
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

//--------------------------------------------------------------------------
// Fonction process_by_pixel avec anti-aliasing (supersampling 2x2)

#define AA_SAMPLES 1

void process_by_pixel(t_scene *scene, int x, int y)
{
    t_color accum = {0, 0, 0};
    int s;
    int samples = AA_SAMPLES;
    float jitter_x, jitter_y;
    t_ray ray;
    t_color sample_color;

    // Pour chaque échantillon, on ajoute un petit décalage aléatoire dans le pixel
    for (s = 0; s < samples; s++)
    {
        jitter_x = ((float)rand() / (float)RAND_MAX) - 0.5f;
        jitter_y = ((float)rand() / (float)RAND_MAX) - 0.5f;
        ray.origin = scene->camera.ray.origin;
        // Utilisation de compute_ray_aa avec le décalage
        ray.direction = compute_ray_aa((float)x + jitter_x, (float)y + jitter_y, scene);
        sample_color = trace_ray(scene, ray, MAX_DEPTH);
        accum.r += sample_color.r;
        accum.g += sample_color.g;
        accum.b += sample_color.b;
    }
    // Moyenne des couleurs
    t_color color;
    color.r = accum.r / samples;
    color.g = accum.g / samples;
    color.b = accum.b / samples;

    put_pixel(scene->image_data, x, y, color, scene->line_len);

    // Optionnel : affichage du pourcentage de rendu
    double curr = y * scene->line_len + x * (scene->bpp / 8);
    double total = HEIGHT * scene->line_len + WIDTH * (scene->bpp / 8);
    printf("\rRendering: %0.f%%", (100.0 * curr / total));
}