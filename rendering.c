/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 00:27:53 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 00:27:57 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"
#include <math.h>

t_vec rotate_vec(t_vec v, t_vec axis, double angle)
{
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    t_vec term1 = scale_vec(v, cos_a);
    t_vec term2 = scale_vec(vector_cross(axis, v), sin_a);
    t_vec term3 = scale_vec(axis, scalar_dot(axis, v) * (1 - cos_a));
    return vector_normalize(vector_add(term1, vector_add(term2, term3)));
}

t_vec compute_ray(int i, int j, t_scene *scene)
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

void render(t_scene *scene)
{
    int j = 0;
    int i;
    int offset;
    float pixel_camera_y;
    float pixel_camera_x;
    t_vec ray_dir;
    t_color color;

    while (j < HEIGHT)
    {
        pixel_camera_y = (scene->viewport_height / 2.0f) - ((j + 0.5f) * scene->pixel_size_y);
        i = 0;
        while (i < WIDTH)
        {
            pixel_camera_x = (-scene->viewport_width / 2.0f) + ((i + 0.5f) * scene->pixel_size_x);
            ray_dir = vector_normalize(vector_add(
                        vector_add(
                            scale_vec(scene->camera_right, pixel_camera_x),
                            scale_vec(scene->camera_up, pixel_camera_y)
                        ),
                        scene->camera_direction));
            (void)ray_dir;
            color.r = 255;
            color.g = 255;
            color.b = 255;
            offset = (j * scene->line_len) + (i * (scene->bpp / 8));
            if (offset >= 0 && offset + 3 < HEIGHT * scene->line_len)
            {
                scene->image_data[offset] = color.b;
                scene->image_data[offset + 1] = color.g;
                scene->image_data[offset + 2] = color.r;
            }
            i++;
        }
        j++;
    }
}
