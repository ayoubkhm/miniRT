NAME		=	philo

HEADER		=	./include/philo.h

CC			=	cc

CFLAGS		=	-Werror -Wall -Wextra -g -I $(HEADER) -pthread

OBJDIR 		= obj
SRCDIR 		= src

SRCS		=	$(SRCDIR)/main.c \
			$(SRCDIR)/lib_utils.c \
			$(SRCDIR)/init.c \
			$(SRCDIR)/philo_utils.c \

# OBJS		=	$(SRCS:.c=.o)
OBJS 		= 	$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))


all			:	$(NAME)

$(OBJDIR)/%.o	: 	$(SRCDIR)/%.c | $(OBJDIR)
				$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)	:
				mkdir -p $(OBJDIR)

$(NAME)		:	$(OBJS) $(HEADER)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean		:
				rm -rf $(OBJDIR)

fclean		:	clean
				rm -rf $(NAME)

re			:	fclean all