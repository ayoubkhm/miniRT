#ifndef MINIRT_H
# define MINIRT_H

# include "types.h"
# include <mlx.h>
# include "../lib/libft/libft.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <fcntl.h>
#include <string.h>
# include "../scene/objects.h"
# include "../scene/scene.h"
#include "../includes/mini_maths.h"
#define MOVE_SPEED 10.0
#define ROTATE_SPEED 0.5
#define MAX_DEPTH 5


t_vec   rotate_vec(t_vec vec, t_vec axis, double angle);
t_color apply_checkerboard(t_vec point, t_color color1, t_color color2);
void    close_window(t_scene *scene);
int     key_press(int keycode, t_scene *scene);
int     key_release(int keycode, t_scene *scene);
void compute_camera_parameters(t_scene *scene);
int update_camera_position(t_scene *scene);
t_vec compute_ray(int i, int j, t_scene *scene);
void    process_by_pixel(t_scene *scene, int x, int y);
t_color trace_ray(t_scene *scene, t_ray ray, int depth);
t_vec reflect(t_vec I, t_vec N);
int refract(t_vec I, t_vec N, double eta, t_vec *refracted);
t_color get_background_color(t_scene *scene, t_ray ray);
t_color combine_recursive_lighting(t_color final_color, t_color reflec_color, t_color refrac_color, double reflectivity, double transparency);
void light_processing(t_scene *scene, t_tools *t);
int trace_scene(t_scene *scene, t_ray ray, t_hit *hit);
bool intersect_object(t_ray ray, t_object *object, t_hit *hit);
double compute_shadow(t_scene *scene, t_hit hit, t_light *light);
double compute_diffuse_single(t_scene *scene, t_hit hit, t_light *light);
double compute_specular_single(t_scene *scene, t_hit hit, t_ray ray, t_light *light);
void put_pixel(char *data, int x, int y, t_color color, int size_line);
void    render_scene(t_scene *scene);
int update_render_if_idle(t_scene *scene);
t_color get_texture_color(t_object *obj, t_vec uv);


#endif
