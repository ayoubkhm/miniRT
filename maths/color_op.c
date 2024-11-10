#include "../includes/minirt.h"
#include "../includes/types.h"
#include "../scene/objects.h"
#include "../scene/scene.h"
#include "mini_maths.h"

t_color color_scale(t_color color, double k)
{
        return ((t_color){color.r * k, color.g * k, color.b * k});
}

t_color color_add(t_color color, t_color c)
{
        return ((t_color){(color.r + c.r)/2, (color.g + c.g)/2, (color.b + c.b)/2});
}
