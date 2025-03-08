/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:00:30 by akhamass          #+#    #+#             */
/*   Updated: 2025/03/08 19:00:30 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "../includes/mini_maths.h"

void	init_hit(t_hit *h)
{
	h->uv = (t_vec){0, 0, 0};
	h->material.reflectivity = 0;
	h->material.transparency = 0;
	h->material.refractive_index = 1;
}
