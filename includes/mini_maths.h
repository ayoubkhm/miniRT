/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_maths.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:40:35 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:40:36 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_MATHS_H
# define MINI_MATHS_H

# include "../includes/types.h"
# include "../lib/libft/libft.h"
# include "../scene/objects.h"
# include <math.h>

bool			solve_quadratic(t_quad q, double *t0, double *t1);
bool			valid_cylinder_candidate(t_ray ray, t_cylinder *cylinder,
					t_vec axis, double t_candidate);
double			cylinder_lateral_intersection(t_vec ray_origin,
					t_vec ray_dir, t_cylinder *cylinder, t_vec axis);
double			cylinder_cap_intersection(t_ray ray, t_vec cap_center,
					double radius, t_vec axis);
int				choose_cylinder_candidate(double t_lateral, double t_cap_bottom,
					double t_cap_top, double *t_val);
void			fill_cyl_hit(t_ray r, t_cylinder *c, t_intinfo *info, t_hit *h);
bool			intersect_cylinder(t_vec ro, t_vec rd, t_cylinder *c, t_hit *h);
bool			compute_plane_intersection(t_vec ray_origin, t_vec ray_dir,
					t_plane *plane, double *t_val);
void			set_hit_info_plane(t_hit *hit, t_plane *plane,
					t_ray ray, double t_val);
void			compute_plane_uv(t_plane *plane, t_hit *hit);
bool			intersect_object(t_ray ray, t_object *object, t_hit *hit);
bool			intersect_plane(t_vec ray_origin, t_vec ray_dir,
					t_plane *plane, t_hit *t);
bool			intersect_sphere(t_vec ro, t_vec rd, t_sphere *s, t_hit *h);
double			choose_sphere(double t0, double t1);
void			fill_sphere_hit(t_ray r, t_sphere *s, double t, t_hit *h);
bool			intersect_hyperboloid(t_vec ro, t_vec rd,
					t_hyperboloid *h, t_hit *hit);
t_quad			compute_hyp_coeffs(t_ray l, double a, double c);
bool			solve_hyp_quad(double A, double B, double C, double *t);
t_vec			compute_hyp_hit_point(t_hypint *I, t_vec lp);
t_vec			compute_hyp_hit_normal(t_hypint *I, t_vec lp);
void			set_hyp_hit(t_hypint *I, double t, t_hit *h);
void			compute_hyp_transform_params(t_hyperboloid *h,
					t_vec *rot, double *ang);
void			transform_hyp_ray(t_ray r, t_hyperboloid *h,
					t_ray *l, t_trans *tr);
t_parabcoeff	compute_parab_coeff(t_ray l, double p);
bool			solve_parab_quad(double A, double B, double C, double *t);
void			compute_parab_hit_geometry(t_parabint *I,
					double t, t_vec *point, t_vec *normal);
void			set_parab_hit(t_parabint *I, double t, t_hit *h);
bool			intersect_paraboloid(t_vec ro, t_vec rd,
					t_paraboloid *p, t_hit *hit);
void			compute_parab_transform_params(t_paraboloid *p,
					t_vec *rot, double *ang);
void			transform_parab_ray(t_ray r, t_paraboloid *p,
					t_ray *l, t_trans *tr);
bool			compute_cone_lat_t(t_ray r, t_cone *c, double *t);
bool			intersect_cone_lat(t_ray r, t_cone *c, t_hit *h);
bool			intersect_cone_base(t_ray r, t_cone *c, t_hit *h);
bool			intersect_cone(t_vec ro, t_vec rd, t_cone *c, t_hit *h);
void			init_hit(t_hit *h);
t_vec			vector_sub(t_vec a, t_vec b);
t_vec			vector_normalize(t_vec v);
t_vec			vector_add(t_vec a, t_vec b);
double			scalar_dot(t_vec a, t_vec b);
t_vec			scale_vec(t_vec a, double k);
t_vec			vector_cross(t_vec a, t_vec b);
double			vector_length(t_vec v);
void			ft_swap(double *a, double *b);
t_vec			rodrigues_rotate(t_vec v, t_vec k, double ang);

#endif
