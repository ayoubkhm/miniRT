#ifndef MINIRT_H
# define MINIRT_H

# include "types.h"                    // Inclure types.h en premier
// # include "../lib/mlx/include/MLX42/MLX42.h"
// # include <mlx.h>
# include "../lib/libft/libft.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../scene/objects.h"
# include "../scene/scene.h"

// Définitions de la grille
# define GRID_WIDTH 80
# define GRID_HEIGHT 24

// Prototypes des fonctions de la grille et des projections
void init_grid_with_border(char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3]);
void display_grid(char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3]);
void project_sphere(t_scene *scene, t_sphere *sphere, char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3], double depth_buffer[GRID_HEIGHT][GRID_WIDTH]);
void project_cylinder(t_scene *scene, t_cylinder *cylinder, char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3], double depth_buffer[GRID_HEIGHT][GRID_WIDTH]);
void project_hyperboloid(t_scene *scene, t_hyperboloid *hyperboloid, char grid[GRID_HEIGHT + 2][GRID_WIDTH + 3], double depth_buffer[GRID_HEIGHT][GRID_WIDTH]);
void draw_scene(t_scene *scene);
void print_scene(t_scene *scene);

#endif
