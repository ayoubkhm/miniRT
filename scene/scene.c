#include "scene.h"


void init_scene(t_scene *scene)
{
    scene->camera_defined = 0;
    scene->ambient_light_defined = 0;
    scene->lights = NULL;
    scene->objects = NULL;
}


int load_scene(t_scene *scene, const char *filename)
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return (0);
    }

    while ((read = getline(&line, &len, file)) != -1) {
        // Supprimer les espaces en début et fin de ligne
        char *trimmed_line = ft_strtrim(line, " \t\n\r");
        if (trimmed_line[0] == '#' || trimmed_line[0] == '\0') {
            free(trimmed_line);
            continue; // Ignorer les commentaires et les lignes vides
        }

        // Analyser la ligne
        if (!parse_line(scene, trimmed_line)) {
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

    // Vérifier que la caméra et la lumière ambiante ont été définies
    if (!scene->camera_defined || !scene->ambient_light_defined) {
        fprintf(stderr, "Error: Camera or ambient light not defined\n");
        return (0);
    }

    return (1);
}














