/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:00:26 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 20:00:27 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

t_vec	rodrigues_rotate(t_vec v, t_vec k, double ang)
{
	t_vec	t1;
	t_vec	t2;
	t_vec	t3;

	t1 = scale_vec(v, cos(ang));
	t2 = scale_vec(vector_cross(k, v), sin(ang));
	t3 = scale_vec(k, scalar_dot(k, v) * (1 - cos(ang)));
	return (vector_add(vector_add(t1, t2), t3));
}
