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

#define EPSILON 1e-5


typedef struct s_lighting_params {
    double ambient;
    double diffuse;
    double specular;
    double reflection;
} t_lighting_params;

typedef struct s_recursive_params {
    double reflectivity;
    double transparency;
} t_recursive_params;

typedef struct s_ray_calc {
    float ndc_x;
    float ndc_y;
    float screen_x;
    float screen_y;
    float cam_x;
    float cam_y;
} t_ray_calc;

typedef struct s_trace {
    t_tools t;
    t_color reflect_color;
    t_color refract_color;
    t_ray reflect_ray;
    t_ray refract_ray;
    double eta;
} t_trace;

typedef struct s_color_d {
    double r;
    double g;
    double b;
} t_color_d;

t_color apply_checkerboard(t_vec point, t_color color1, t_color color2);
void    close_window(t_scene *scene);
int     key_press(int keycode, t_scene *scene);
int     key_release(int keycode, t_scene *scene);
void compute_camera_parameters(t_scene *scene);
int update_camera_position(t_scene *scene);
void    process_by_pixel(t_scene *scene, int x, int y);
t_color trace_ray(t_scene *scene, t_ray ray, int depth);
t_vec reflect(t_vec I, t_vec N);
int refract(t_vec I, t_vec N, double eta, t_vec *refracted);
t_color get_background_color(t_scene *scene, t_ray ray);
t_color combine_recursive_lighting(t_color local_color, t_color reflect_color, t_color refract_color, t_recursive_params params);
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
t_vec	reflect(t_vec I, t_vec N);
int	refract(t_vec I, t_vec N, double eta, t_vec *refracted);
void	light_processing(t_scene *scene, t_tools *t);
t_color	get_background_color(t_scene *scene, t_ray ray);
void	compute_base_color(t_tools *t);
void	process_by_pixel(t_scene *scene, int x, int y);
t_color	compute_reflection(t_scene *scene, t_trace *trace, t_ray ray, int depth);
t_color	compute_refraction(t_scene *scene, t_trace *trace,
    t_ray ray, int depth);
t_color	trace_ray(t_scene *scene, t_ray ray, int depth);
t_color	get_checkerboard_color(t_vec uv, int scale,
    t_color color1, t_color color2);
void	normalize_material(t_hit *hit);
void	compute_base_color(t_tools *t);
t_color_d	apply_lighting_factors(t_color_d base, double local, double extra);
t_color	convert_color_d_to_color(t_color_d cd);
t_color	combine_lighting(t_color base_color, t_lighting_params params);



#endif
