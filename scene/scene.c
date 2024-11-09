#include "../includes/minirt.h"

void    init_scene(t_scene *scene)
{
    scene->camera_defined = 0;
    scene->ambient_light_defined = 0;
    scene->lights = NULL;
    scene->objects = NULL;
    scene->mlx = mlx_init();
    scene->win = mlx_new_window(scene->mlx, 800, 600, "MERVAN LE GOAT");
    scene->image = mlx_new_image(scene->mlx, WIDTH, HEIGHT);
    scene->image_data = mlx_get_data_addr(scene->image, &scene->bpp, &scene->line_len, &scene->endian);
    scene->move_forward = false;
    scene->move_backward = false;
    scene->move_left = false;
    scene->move_right = false;
    scene->rotate_left = false;
    scene->rotate_right = false;
    scene->rotate_up = false;
    scene->rotate_down = false;
}

int load_scene(t_scene *scene, const char *filename)
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *trimmed_line;


    file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return (0);
    }

    while ((read = getline(&line, &len, file)) != -1)
    {
        trimmed_line = ft_strtrim(line, " \t\n\r");
        if (trimmed_line[0] == '#' || trimmed_line[0] == '\0')
        {
            free(trimmed_line);
            continue ;
        }
        if (!parse_line(scene, trimmed_line))
        {
            fprintf(stderr, "Error parsing line: %s\n", trimmed_line);
            free(trimmed_line);
            free(line);
            fclose(file);
            return (0);
        }
        free(trimmed_line);
    }
    free(line);
    fclose(file);
    if (!scene->camera_defined || !scene->ambient_light_defined)
    {
        fprintf(stderr, "Error: Camera or ambient light not defined\n");
        return (0);
    }
    return (1);
}














