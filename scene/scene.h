// scene.h

#ifndef SCENE_H
# define SCENE_H

# include "../includes/minirt.h"
# include "../lib/libft/libft.h"
# include "objects.h"

// Liste chaînée générique
typedef struct s_list {
    void          *content;
    struct s_list *next;
} t_list;

// Types d'objets
typedef enum e_obj_type {
    SPHERE,
    PLANE,
    CYLINDER
} t_obj_type;

// Objet générique
typedef struct s_object {
    t_obj_type type;
    void       *data;
} t_object;

// Lumière ambiante
typedef struct s_am_light {
    t_color color;
    double  ratio; // Intensité (entre 0.0 et 1.0)
} t_am_light;

// Caméra
typedef struct s_cam {
    t_vec   position;
    t_vec   direction;
    double  fov; // Champ de vision en degrés
} t_cam;

// Lumière ponctuelle
typedef struct s_light {
    t_vec   position;
    double  ratio; // Intensité (entre 0.0 et 1.0)
    t_color color;
} t_light;

// Scène
typedef struct s_scene {
    t_cam       camera;
    t_am_light  ambient_light;
    t_list     *lights;                // Liste des lumières ponctuelles
    t_list     *objects;               // Liste des objets (t_object *)
    int         camera_defined;
    int         ambient_light_defined;
} t_scene;

// Prototypes des fonctions
void init_scene(t_scene *scene);
int  load_scene(t_scene *scene, const char *filename);

// Prototypes des fonctions de parsing
int parse_line(t_scene *scene, char *line);
int parse_ambient_light(t_scene *scene, char **tokens);
int parse_camera(t_scene *scene, char **tokens);
int parse_light(t_scene *scene, char **tokens);
int parse_sphere(t_scene *scene, char **tokens);
int parse_plane(t_scene *scene, char **tokens);
int parse_cylinder(t_scene *scene, char **tokens);

// Fonctions utilitaires
t_vec   parse_vector(char *str);
t_color parse_color(char *str);
void    free_tokens(char **tokens);

void add_light(t_scene *scene, t_light *light);
void add_sphere(t_scene *scene, t_sphere *sphere);
void add_plane(t_scene *scene, t_plane *plane);
void add_cylinder(t_scene *scene, t_cylinder *cylinder);


void free_scene(t_scene *scene);
void free_objects(t_list *objects);
void free_lights(t_list *lights);
void free_tokens(char **tokens);


#endif
