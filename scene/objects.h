/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 13:29:05 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H
# define WIDTH 800
# define HEIGHT 600

# include "../includes/types.h"
# include "../lib/libft/libft.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_sphere
{
	t_vec	center;
	double	radius;
	bool	is_checkerboard;
	t_color	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec	point;
	t_vec	normal;
	t_color	color;
	t_color	quadrillage;
	bool	is_checkerboard;
	bool	flag_qud;
}	t_plane;

typedef struct s_cylinder
{
	t_vec	base;
	t_vec	axis;
	double	radius;
	double	height;
	double	diameter;
	bool	is_checkerboard;
	t_color	color;
}	t_cylinder;

typedef struct s_hyperboloid {
    t_vec   base;
    t_vec   axis;
    double  diameter;
    double  radius;
    double  height;
	bool	is_checkerboard;
    t_color color;
} t_hyperboloid;

typedef struct s_paraboloid {
    t_vec    base;     // Le sommet (ou le point de référence) du paraboloïde
    t_vec    axis;     // L’axe d’orientation (par exemple, (0,1,0) pour un paraboloïde vertical)
    double   p;        // Le paramètre d'ouverture du paraboloïde. L’équation locale est : x² + y² - 2p*z = 0
    double   height;   // (Optionnel) Une hauteur maximale (pour limiter l’objet)
	bool	is_checkerboard;
    t_color  color;
} t_paraboloid;

typedef struct s_cone {
    t_vec vertex;      // Le sommet du cône
    t_vec axis;        // L'axe (direction) du cône (normalisé)
    double angle;      // L'angle ouvert du cône en radians
    double height;     // (Optionnel) une hauteur limite si besoin
	bool	is_checkerboard;
    t_color color;
} t_cone;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	HYPERBOLOID,
	PARABOLOID,
	CONE
}	t_object_type;

typedef struct s_object
{
	t_object_type	type;
	void			*data;
	t_color			color;
	char			*texture_path;
	void			*texture;
	char			*texture_data;
	int				tex_width;
	int				tex_height;
	int				bpp;
	int				line_len;
	int				endian;
	bool			is_checkerboard;
}	t_object;

typedef struct s_am_light
{
	t_color	color;
	double	ratio;
}	t_am_light;

typedef struct s_cam
{
	t_ray	ray;
	double	fov;
}	t_cam;

typedef struct s_light
{
	t_vec	position;
	t_color	color;
	double	ratio;
}	t_light;

typedef struct s_scene
{
	t_cam			camera;
	t_am_light		ambient_light;
	t_list			*lights;
	t_list			*objects;
	int				object_count;
	int				light_count;
	int				camera_defined;
	int				ambient_light_defined;
	void			*mlx;
	void			*win;
	void			*image;
	char			*image_data;
	int				bpp;
	int				line_len;
	int				endian;
	bool			move_forward;
	bool			move_backward;
	bool			move_left;
	bool			move_right;
	bool			rotate_left;
	bool			rotate_right;
	bool			rotate_up;
	bool			rotate_down;
    bool            move_up;
    bool            move_down;  
	float			aspect_ratio;
	float			fov_rad;
	float			viewport_width;
	float			viewport_height;
	float			pixel_size_x;
	float			pixel_size_y;
	t_vec			camera_direction;
	t_vec			camera_right;
	t_vec			camera_up;
    double  last_key_press_time;
    double  last_key_release_time;
    bool    key_active;
    bool    rendering_pending;
	t_vec accumulated_translation;
	double accumulated_rotation_y;
	double accumulated_rotation_x; 
}	t_scene;

#endif