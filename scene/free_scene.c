/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:57:21 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/09 23:57:40 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	free_scene(t_scene *scene)
{
	free_objects(scene->objects);
	free_lights(scene->lights);
	if (scene->image)
		mlx_destroy_image(scene->mlx, scene->image);
	if (scene->win)
		mlx_destroy_window(scene->mlx, scene->win);
}

void	free_objects(t_list *objects)
{
	t_list		*current;
	t_list		*next;
	t_object	*object;

	current = objects;
	while (current)
	{
		next = current->next;
		object = (t_object *)current->content;
		if (object->type == SPHERE)
			free(object->data);
		else if (object->type == PLANE)
			free(object->data);
		else if (object->type == CYLINDER)
			free(object->data);
		else if (object->type == HYPERBOLOID)
			free(object->data);
		free(object);
		free(current);
		current = next;
	}
}

void	free_lights(t_list *lights)
{
	t_list	*current;
	t_list	*next;

	current = lights;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}
