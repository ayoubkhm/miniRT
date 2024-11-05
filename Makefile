# Nom de l'exécutable
NAME = minirt

# Compilateur et flags
CC = cc
# CFLAGS = 	-Wall -Wextra -Werror -Iincludes -Iscene -Icamera -Ilib/libft -Ilib/mlx
CFLAGS =	-Wall -Wextra -Werror -g -I /usr/X11/include -Iincludes -Iscene -Icamera -Ilib/libft #Pour que cela fonctionne chez Mervan

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
    MLX_FLAGS = -L$(MLX_DIR) -lmlx42 -lXext -lX11 -lm -lbsd
else ifeq ($(UNAME_S),Darwin)
#     MLX_FLAGS = -L$(MLX_DIR) -lmlx42 -framework OpenGL -framework AppKit
      MLX_FLAGS = -L /usr/X11/lib -l mlx -framework OpenGL -framework AppKit
endif

# Fichiers sources
SRC = $(SRCDIR)/main2.c \
      $(SCENEDIR)/scene.c \
      $(SCENEDIR)/add_light.c \
	  $(SCENEDIR)/free_scene.c \
	  $(SCENEDIR)/parse_cylinder.c \
	  $(SCENEDIR)/parse_hyperboloid.c \
	  $(SCENEDIR)/parse_objects.c \
	  $(SCENEDIR)/parse_scene.c \
	  $(SCENEDIR)/utils_list.c \
	  $(SCENEDIR)/utils_parse.c \
	  $(SRCDIR)/testing.c \
	  $(MATHSDIR)/vector_op.c \
	  $(MATHSDIR)/intersection.c
      # Ajoutez $(CAMERADIR)/camera.c ici lorsque vous aurez le fichier camera.c

# Fichiers objets
OBJ = $(SRC:.c=.o)

# Règles
all: $(NAME)

# Compilation de l'exécutable avec libft et mlx
$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR)       # Compile libft si nécessaire
	@$(MAKE) -C $(MLX_DIR)         # Compile mlx si nécessaire
	@$(CC) $(CFLAGS) $(MLX_FLAGS) -o $(NAME) $(OBJ) $(LIBFT_FLAGS)
	@echo "Compilation de $(NAME) terminée."

# Compilation des objets
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilation de $< effectuée."

# Nettoyage des objets
clean:
	@rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean  # Nettoyage de libft
	@$(MAKE) -C $(MLX_DIR) clean    # Nettoyage de mlx
	@echo "Nettoyage des fichiers objets."

# Nettoyage complet des objets et de l'exécutable
fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean # Nettoyage complet de libft
	@echo "Nettoyage complet (objets et exécutable)."

# Recompilation complète
re: fclean all

.PHONY: all clean fclean re
