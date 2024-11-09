#include "scene.h"

t_vec parse_vector(char *str)
{
    char **components = ft_split(str, ',');
    t_vec vec;

    if (!components[0] || !components[1] || !components[2])
    {
        fprintf(stderr, "Error: Invalid vector format\n");
        // Gérer l'erreur
    }
    vec.x = ft_atof(components[0]);
    vec.y = ft_atof(components[1]);
    vec.z = ft_atof(components[2]);
    // printf("%.1f %.1f %.1f", vec.x, vec.y, vec.z);

    free_tokens(components);
    return vec;
}

t_color parse_color(char *str)
{
    char **components = ft_split(str, ',');
    t_color color;

    if (!components[0] || !components[1] || !components[2])
    {
        fprintf(stderr, "Error: Invalid color format\n");
    }

    color.r = ft_atoi(components[0]);
    color.g = ft_atoi(components[1]);
    color.b = ft_atoi(components[2]);

    if (color.r < 0 || color.r > 255 ||
        color.g < 0 || color.g > 255 ||
        color.b < 0 || color.b > 255) {
        fprintf(stderr, "Error: Color value out of range\n");
    }

    free_tokens(components);
    return color;
}