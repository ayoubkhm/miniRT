/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:49:13 by akhamass          #+#    #+#             */
/*   Updated: 2025/02/14 08:29:14 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H
# define WIDTH 800
# define HEIGHT 600

# include "../lib/libft/libft.h"
# include "objects.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

void	init_scene(t_scene *scene);
int		load_scene(t_scene *scene, const char *filename);
int		parse_line(t_scene *scene, char *line);
int		parse_ambient_light(t_scene *scene, char **tokens);
int		parse_camera(t_scene *scene, char **tokens);
int		parse_light(t_scene *scene, char **tokens);
int		parse_sphere(t_scene *scene, char **tokens);
int		parse_plane(t_scene *scene, char **tokens);
int		parse_cylinder(t_scene *scene, char **tokens);
void	add_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid,
			char *texture_path);
t_vec	parse_vector(char *str);
t_color	parse_color(char *str);
void	free_tokens(char **tokens);
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new_node);
void	add_light(t_scene *scene, t_light *light);
void	add_sphere(t_scene *scene, t_sphere *sphere, char *texture_path);
void	add_plane(t_scene *scene, t_plane *plane, char *texture_path);
void	add_cylinder(t_scene *scene, t_cylinder *cylinder,
			char *texture_path);
void	free_scene(t_scene *scene);
void	free_objects(t_list *objects);
void	free_lights(t_list *lights);
char	*get_texture_path(char **tokens, t_cylinder *cylinder);
void	handle_texture_error(t_cylinder *cylinder, char *msg, char *str);
void	assign_texture(t_scene *scene, t_object *obj,
			char *texture_path, t_cylinder *cylinder);
int parse_hyperboloid(t_scene *scene, char **tokens);
void add_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid,
	char *texture_path);
int parse_paraboloid(t_scene *scene, char **tokens);
void add_paraboloid(t_scene *scene, t_paraboloid *paraboloid, char *texture_path);
void	add_cone(t_scene *scene, t_cone *cone, char *texture_path);
int	parse_cone(t_scene *scene, char **tokens);


#endif