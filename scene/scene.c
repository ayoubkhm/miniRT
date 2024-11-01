// scene.c

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

int parse_line(t_scene *scene, char *line)
{
    char **tokens = ft_split(line, ' ');

    if (!tokens[0]) {
        free_tokens(tokens);
        return (0);
    }

    if (ft_strcmp(tokens[0], "A") == 0) {
        if (!parse_ambient_light(scene, tokens)) {
            free_tokens(tokens);
            return (0);
        }
    } else if (ft_strcmp(tokens[0], "C") == 0) {
        if (!parse_camera(scene, tokens)) {
            free_tokens(tokens);
            return (0);
        }
    } else if (ft_strcmp(tokens[0], "L") == 0) {
        if (!parse_light(scene, tokens)) {
            free_tokens(tokens);
            return (0);
        }
    } else if (ft_strcmp(tokens[0], "sp") == 0) {
        if (!parse_sphere(scene, tokens)) {
            free_tokens(tokens);
            return (0);
        }
    } else if (ft_strcmp(tokens[0], "pl") == 0) {
        if (!parse_plane(scene, tokens)) {
            free_tokens(tokens);
            return (0);
        }
    } else if (ft_strcmp(tokens[0], "cy") == 0) {
        if (!parse_cylinder(scene, tokens)) {
            free_tokens(tokens);
            return (0);
        }
    } else {
        fprintf(stderr, "Unknown identifier: %s\n", tokens[0]);
        free_tokens(tokens);
        return (0);
    }

    free_tokens(tokens);
    return (1);
}

int parse_cylinder(t_scene *scene, char **tokens)
{
    t_cylinder *cylinder;

    if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5]) {
        fprintf(stderr, "Error: Invalid cylinder definition\n");
        return (0);
    }

    cylinder = malloc(sizeof(t_cylinder));
    if (!cylinder)
        return (0);

    cylinder->base = parse_vector(tokens[1]);
    cylinder->axis = parse_vector(tokens[2]);
    cylinder->diameter = ft_atof(tokens[3]);
    cylinder->height = ft_atof(tokens[4]);
    cylinder->color = parse_color(tokens[5]);

    add_cylinder(scene, cylinder);
    return (1);
}

void add_cylinder(t_scene *scene, t_cylinder *cylinder)
{
    t_list   *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object) {
        // Gérer l'erreur d'allocation
        return;
    }

    object->type = CYLINDER;
    object->data = cylinder;

    new_node->content = object;
    new_node->next = scene->objects;
    scene->objects = new_node;
}



int parse_plane(t_scene *scene, char **tokens)
{
    t_plane *plane;

    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        fprintf(stderr, "Error: Invalid plane definition\n");
        return (0);
    }

    plane = malloc(sizeof(t_plane));
    if (!plane)
        return (0);

    plane->point = parse_vector(tokens[1]);
    plane->normal = parse_vector(tokens[2]);
    plane->color = parse_color(tokens[3]);

    add_plane(scene, plane);
    return (1);
}

void add_plane(t_scene *scene, t_plane *plane)
{
    t_list   *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object) {
        // Gérer l'erreur d'allocation
        return;
    }

    object->type = PLANE;
    object->data = plane;

    new_node->content = object;
    new_node->next = scene->objects;
    scene->objects = new_node;
}



int parse_light(t_scene *scene, char **tokens)
{
    t_light *light;

    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        fprintf(stderr, "Error: Invalid light definition\n");
        return (0);
    }

    light = malloc(sizeof(t_light));
    if (!light)
        return (0);

    light->position = parse_vector(tokens[1]);
    light->ratio = ft_atof(tokens[2]);
    light->color = parse_color(tokens[3]);

    if (light->ratio < 0.0 || light->ratio > 1.0) {
        fprintf(stderr, "Error: Light ratio out of range\n");
        free(light);
        return (0);
    }

    add_light(scene, light);
    return (1);
}



int parse_ambient_light(t_scene *scene, char **tokens)
{
    if (scene->ambient_light_defined) {
        fprintf(stderr, "Error: Ambient light already defined\n");
        return (0);
    }

    if (!tokens[1] || !tokens[2]) {
        fprintf(stderr, "Error: Invalid ambient light definition\n");
        return (0);
    }

    scene->ambient_light.ratio = ft_atof(tokens[1]);
    scene->ambient_light.color = parse_color(tokens[2]);

    if (scene->ambient_light.ratio < 0.0 || scene->ambient_light.ratio > 1.0) {
        fprintf(stderr, "Error: Ambient light ratio out of range\n");
        return (0);
    }

    scene->ambient_light_defined = 1;
    return (1);
}

int parse_camera(t_scene *scene, char **tokens)
{
    if (scene->camera_defined) {
        fprintf(stderr, "Error: Camera already defined\n");
        return (0);
    }

    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        fprintf(stderr, "Error: Invalid camera definition\n");
        return (0);
    }

    scene->camera.position = parse_vector(tokens[1]);
    scene->camera.direction = parse_vector(tokens[2]);
    scene->camera.fov = ft_atof(tokens[3]);

    if (scene->camera.fov < 0 || scene->camera.fov > 180) {
        fprintf(stderr, "Error: Camera FOV out of range\n");
        return (0);
    }

    scene->camera_defined = 1;
    return (1);
}

int parse_sphere(t_scene *scene, char **tokens)
{
    t_sphere *sphere;

    if (!tokens[1] || !tokens[2] || !tokens[3]) {
        fprintf(stderr, "Error: Invalid sphere definition\n");
        return (0);
    }

    sphere = malloc(sizeof(t_sphere));
    if (!sphere)
        return (0);

    sphere->center = parse_vector(tokens[1]);
    sphere->radius = ft_atof(tokens[2]) / 2.0; // Diviser par 2 si le diamètre est donné
    sphere->color = parse_color(tokens[3]);

    add_sphere(scene, sphere);
    return (1);
}

void add_sphere(t_scene *scene, t_sphere *sphere)
{
    t_list *new_node = malloc(sizeof(t_list));
    t_object *object = malloc(sizeof(t_object));

    if (!new_node || !object) {
        // Gérer l'erreur d'allocation
        return;
    }

    object->type = SPHERE;
    object->data = sphere;

    new_node->content = object;
    new_node->next = scene->objects;
    scene->objects = new_node;
}

t_vec parse_vector(char *str)
{
    char **components = ft_split(str, ',');
    t_vec vec;

    if (!components[0] || !components[1] || !components[2]) {
        fprintf(stderr, "Error: Invalid vector format\n");
        // Gérer l'erreur
    }

    vec.x = ft_atof(components[0]);
    vec.y = ft_atof(components[1]);
    vec.z = ft_atof(components[2]);

    free_tokens(components);
    return vec;
}

t_color parse_color(char *str)
{
    char **components = ft_split(str, ',');
    t_color color;

    if (!components[0] || !components[1] || !components[2]) {
        fprintf(stderr, "Error: Invalid color format\n");
        // Gérer l'erreur
    }

    color.r = ft_atoi(components[0]);
    color.g = ft_atoi(components[1]);
    color.b = ft_atoi(components[2]);

    // Vérifier que les valeurs sont entre 0 et 255
    if (color.r < 0 || color.r > 255 ||
        color.g < 0 || color.g > 255 ||
        color.b < 0 || color.b > 255) {
        fprintf(stderr, "Error: Color value out of range\n");
        // Gérer l'erreur
    }

    free_tokens(components);
    return color;
}



void add_light(t_scene *scene, t_light *light)
{
    t_list *new_node = malloc(sizeof(t_list));

    if (!new_node) {
        // Gérer l'erreur d'allocation
        return;
    }

    new_node->content = light;
    new_node->next = scene->lights;
    scene->lights = new_node;
}




void free_scene(t_scene *scene)
{
    free_objects(scene->objects);
    free_lights(scene->lights);
    // Si vous avez alloué d'autres éléments, libérez-les ici
}

void free_objects(t_list *objects)
{
    t_list *current;
    t_list *next;
    t_object *object;

    current = objects;
    while (current) {
        next = current->next;
        object = (t_object *)current->content;

        // Libérer les données spécifiques de l'objet
        if (object->type == SPHERE)
            free(object->data); // Libère t_sphere
        else if (object->type == PLANE)
            free(object->data); // Libère t_plane
        else if (object->type == CYLINDER)
            free(object->data); // Libère t_cylinder

        free(object);   // Libère t_object
        free(current);  // Libère le nœud de la liste
        current = next;
    }
}

void free_lights(t_list *lights)
{
    t_list *current;
    t_list *next;

    current = lights;
    while (current) {
        next = current->next;
        free(current->content); // Libère t_light
        free(current);          // Libère le nœud de la liste
        current = next;
    }
}

void free_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i]) {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}
