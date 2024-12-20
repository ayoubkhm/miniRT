#ifndef SCENE_H
# define SCENE_H
# define WIDTH 800
# define HEIGHT 600
# include "../includes/types.h"
# include "../lib/libft/libft.h"
# include "objects.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list {
    void *content;
    struct s_list *next;
} t_list;

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
    double  ratio;
    t_color color;
} t_light;

// Scène
typedef struct s_scene {
    t_cam       camera;
    t_am_light  ambient_light;
    t_list     *lights;
    t_list     *objects;
    int         camera_defined;
    int         ambient_light_defined;
    void    *mlx;
    void    *win;
    void *image;
    char *image_data;    
    int         bpp;
    int         line_len;
    int         endian;
    bool move_forward;
    bool move_backward;
    bool move_left;
    bool move_right;
    bool rotate_left;
    bool rotate_right;
    bool rotate_up;
    bool rotate_down;
} t_scene;

//scene.c
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

int parse_hyperboloid(t_scene *scene, char **tokens);
void add_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid, char *texture_path);

// Fonctions utilitaires
t_vec   parse_vector(char *str);
t_color parse_color(char *str);
void    free_tokens(char **tokens);
t_list  *ft_lstnew(void *content);
void    ft_lstadd_front(t_list **lst, t_list *new_node);

// Fonctions d'ajout d'objets et de lumières
void add_light(t_scene *scene, t_light *light);
void add_sphere(t_scene *scene, t_sphere *sphere, char *texture_path);
void add_plane(t_scene *scene, t_plane *plane, char *texture_path);
void add_cylinder(t_scene *scene, t_cylinder *cylinder, char *texture_path);

// Fonctions de libération de la mémoire
void free_scene(t_scene *scene);
void free_objects(t_list *objects);
void free_lights(t_list *lights);

#endif
