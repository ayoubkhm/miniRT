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
#include "../maths/mini_maths.h"
#define MOVE_SPEED 2.0
#define ROTATE_SPEED 0.5

t_vec rotate_vec(t_vec vec, t_vec axis, double angle);
t_vec compute_ray(int i, int j, t_cam camera);
t_color apply_checkerboard(t_vec point, t_color color1, t_color color2);
void render_scene(t_scene *scene);

void close_window(t_scene *scene);
int key_press(int keycode, t_scene *scene);
int key_release(int keycode, t_scene *scene);
void update_camera_position(t_scene *scene);

#endif
