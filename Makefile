# Nom de l'exécutable
NAME = minirt

# Compilateur et flags
CC = cc
# Vous pouvez ajouter ou enlever -g pour le débogage
CFLAGS = -Wall -Wextra -Werror -g -I/usr/X11/include -Iincludes -Iscene -Icamera -Ilib/libft

# Dossiers des sources
SRCDIR = .
SCENEDIR = scene
CAMERADIR = camera
MATHSDIR = maths

# Dossiers des bibliothèques
LIBFT_DIR = lib/libft
MLX_DIR = lib/mlx

# Flags pour les bibliothèques
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

# Détection du système d'exploitation
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lbsd
else ifeq ($(UNAME_S),Darwin)
    MLX_FLAGS = -L/usr/local/lib -lmlx -framework OpenGL -framework AppKit
endif

# Fichiers sources
SRC =	./maths/intersection.c \
	./maths/cone_intersection.c \
	./maths/cylinder_inter2.c \
	./maths/hyperboloid_intersection.c \
	./maths/hyperboloid_transform.c \
	./maths/paraboloid_math.c \
	./maths/quadratic.c \
	./maths/sphere_intersection.c \
	./maths/cylinder_inter.c \
	./maths/hit_utils.c \
	./maths/hyperboloid_math.c \
	./maths/paraboloid_transform.c \
	./maths/rotation.c \
	./maths/vector_op.c \
	./maths/vector_op2.c \
	./engine/color_utils.c \
	./engine/material_utils.c \
	./engine/ray_tracing.c \
	./engine/recursive_lighting.c \
	./engine/rendering.c \
	./engine/vector_utils.c \
	./main.c \
	./engine/texture_mapping.c \
	./mlx_utils.c \
	./engine/light_effects.c \
	./scene/parse_sphere.c \
	./scene/add_sphere.c \
	./scene/parse_hyperboloid.c \
	./scene/add_hyperboloid.c \
	./scene/parse_paraboloid.c \
	./scene/add_paraboloid.c \
	./scene/parse_cone.c \
	./scene/add_cone.c \
	./scene/scene.c \
	./scene/scene_2.c \
	./scene/parse_plane.c \
	./scene/add_plane.c \
	./scene/parse_scene.c \
	./scene/utils_list.c \
	./scene/add_cylinder.c \
	./scene/parse_cylinder.c \
	./scene/free_scene.c \
	./scene/add_light.c \
	./scene/utils_parse.c

# Fichiers objets (conversion de .c en .o)
OBJ = $(SRC:.c=.o)

# Règles
all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR)       # Compile libft si nécessaire
	@$(MAKE) -C $(MLX_DIR)         # Compile mlx si nécessaire
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_FLAGS) $(MLX_FLAGS) -lm
	@echo "Compilation de $(NAME) terminée."

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilation de $< effectuée."

clean:
	@rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(MLX_DIR) clean
	@echo "Nettoyage des fichiers objets effectué."

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "Nettoyage complet (objets et exécutable) effectué."

re: fclean all

.PHONY: all clean fclean re