#include "scene.h"


int parse_line(t_scene *scene, char *line)
{
    char **tokens = ft_split(line, ' ');

    if (!tokens[0])
    {
        free_tokens(tokens);
        return (0);
    }

    if (ft_strcmp(tokens[0], "A") == 0)
    {
        if (!parse_ambient_light(scene, tokens))
        {
            free_tokens(tokens);
            return (0);
        }
    }
    else if (ft_strcmp(tokens[0], "C") == 0)
    {
        if (!parse_camera(scene, tokens))
        {
            free_tokens(tokens);
            return (0);
        }
    }
    else if (ft_strcmp(tokens[0], "L") == 0)
    {
        if (!parse_light(scene, tokens))
        {
            free_tokens(tokens);
            return (0);
        }
    }
    else if (ft_strcmp(tokens[0], "sp") == 0)
    {
        if (!parse_sphere(scene, tokens))
        {
            free_tokens(tokens);
            return (0);
        }
    }
    else if (ft_strcmp(tokens[0], "pl") == 0)
    {
        if (!parse_plane(scene, tokens))
        {
            free_tokens(tokens);
            return (0);
        }
    } 
    else if (ft_strcmp(tokens[0], "cy") == 0)
    {
        if (!parse_cylinder(scene, tokens))
        {
            free_tokens(tokens);
            return (0);
        }
    }
    else if (ft_strcmp(tokens[0], "hy") == 0)
    {
        if (!parse_hyperboloid(scene, tokens))
        {
            free_tokens(tokens);
            return (0);
        }
    }
    else
    {
        fprintf(stderr, "Unknown identifier: %s\n", tokens[0]);
        free_tokens(tokens);
        return (0);
    }

    free_tokens(tokens);
    return (1);
}

int parse_ambient_light(t_scene *scene, char **tokens)
{
    if (scene->ambient_light_defined)
    {
        fprintf(stderr, "Error: Ambient light already defined\n");
        return (0);
    }

    if (!tokens[1] || !tokens[2])
    {
        fprintf(stderr, "Error: Invalid ambient light definition\n");
        return (0);
    }

    scene->ambient_light.ratio = ft_atof(tokens[1]);
    scene->ambient_light.color = parse_color(tokens[2]);

    if (scene->ambient_light.ratio < 0.0 || scene->ambient_light.ratio > 1.0)
    {
        fprintf(stderr, "Error: Ambient light ratio out of range\n");
        return (0);
    }

    scene->ambient_light_defined = 1;
    return (1);
}

int parse_camera(t_scene *scene, char **tokens)
{
    if (scene->camera_defined)
    {
        fprintf(stderr, "Error: Camera already defined\n");
        return (0);
    }

    if (!tokens[1] || !tokens[2] || !tokens[3])
    {
        fprintf(stderr, "Error: Invalid camera definition\n");
        return (0);
    }

    scene->camera.position = parse_vector(tokens[1]);
    scene->camera.direction = parse_vector(tokens[2]);
    scene->camera.fov = ft_atof(tokens[3]);

    if (scene->camera.fov < 0 || scene->camera.fov > 180)
    {
        fprintf(stderr, "Error: Camera FOV out of range\n");
        return (0);
    }
    //printf("Camera defined:\n");
    //printf("  Position: {%.2f, %.2f, %.2f}\n", scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
    // printf("  Direction: {%.2f, %.2f, %.2f}\n", scene->camera.direction.x, scene->camera.direction.y, scene->camera.direction.z);
    // printf("  FOV: %.2f\n", scene->camera.fov);
    scene->camera_defined = 1;
    return (1);
}

int parse_light(t_scene *scene, char **tokens)
{
    t_light *light;

    if (!tokens[1] || !tokens[2] || !tokens[3])
    {
        fprintf(stderr, "Error: Invalid light definition\n");
        return (0);
    }

    light = malloc(sizeof(t_light));
    if (!light)
        return (0);

    light->position = parse_vector(tokens[1]);
    light->ratio = ft_atof(tokens[2]);
    light->color = parse_color(tokens[3]);

    if (light->ratio < 0.0 || light->ratio > 1.0)
    {
        fprintf(stderr, "Error: Light ratio out of range\n");
        free(light);
        return (0);
    }

    add_light(scene, light);
    return (1);
}

void free_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}