#ifndef MINI_MATHS_H
# define MINI_MATHS_H

# include "../includes/types.h"
# include "../lib/libft/libft.h"
# include "objects.h"
# include <math.h>
# include <float.h>

bool solve_quadratic(double A, double B, double C, double *t0, double *t1);
bool valid_cylinder_candidate(t_vec ray_origin, t_vec ray_dir, t_cylinder *cylinder, t_vec axis, double t_candidate);
double cylinder_lateral_intersection(t_vec ray_origin, t_vec ray_dir, t_cylinder *cylinder, t_vec axis);
double cylinder_cap_intersection(t_vec ray_origin, t_vec ray_dir, t_vec cap_center, double radius, t_vec axis);
int choose_cylinder_candidate(double t_lateral, double t_cap_bottom, double t_cap_top, double *t_val);
void fill_cylinder_hit_record(t_vec ray_origin, t_vec ray_dir, t_cylinder *cylinder, double t_val, int candidate_type, t_hit *hit, t_vec axis);
bool intersect_cylinder(t_vec ray_origin, t_vec ray_dir, t_cylinder *cylinder, t_hit *t);

bool compute_plane_intersection(t_vec ray_origin, t_vec ray_dir, t_plane *plane, double *t_val);
void set_hit_info_plane(t_hit *hit, t_plane *plane, t_vec ray_origin, t_vec ray_dir, double t_val);
void compute_plane_uv(t_plane *plane, t_hit *hit);
bool intersect_object(t_ray ray, t_object *object, t_hit *hit);
bool intersect_plane(t_vec ray_origin, t_vec ray_dir, t_plane *plane, t_hit *t);

bool intersect_sphere(t_vec ray_origin, t_vec ray_dir, t_sphere *sphere, t_hit *t);
double choose_sphere_intersection(double t0, double t1);
void fill_sphere_hit_record(t_vec ray_origin, t_vec ray_dir, t_sphere *sphere, double t_val, t_hit *hit);

bool intersect_hyperboloid(t_vec ray_origin, t_vec ray_dir, t_hyperboloid *hyperboloid, t_hit *hit);
bool intersect_paraboloid(t_vec ray_origin, t_vec ray_dir, t_paraboloid *paraboloid, t_hit *hit);
bool intersect_cone(t_vec ray_origin, t_vec ray_dir, t_cone *cone, t_hit *hit);


t_vec vector_sub(t_vec a, t_vec b);
t_vec vector_normalize(t_vec v);
t_vec vector_add(t_vec a, t_vec b);
double scalar_dot(t_vec a, t_vec b);
t_vec  scale_vec(t_vec a, double k);
t_vec vector_cross(t_vec a, t_vec b);


double vector_length(t_vec v);
void    ft_swap(double *a, double *b);

#endif