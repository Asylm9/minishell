# Makefile pour tester la partie exécution du minishell

NAME = texec

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -w
LIBS = -lreadline -lhistory

# Répertoires
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Fichiers sources
SRCS = test_ast.c \
       new_exec.c \
       path.c \
	   list_utils.c \
       redirections.c \
       env_utils.c \
       resources.c \
       utils.c \
	   builtins/builtins_utils.c \
	   builtins/cd.c \
	   builtins/echo.c \
	   builtins/env.c \
	   builtins/exit.c \
	   builtins/pwd.c \
	   builtins/unset.c \
	   builtins/export.c \
	   builtins/export_sorting.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle principale
all: $(NAME)

# Compilation de libft
$(LIBFT):
	@make -C $(LIBFT_DIR)

# Compilation du programme principal
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -f $(OBJS)
	@make clean -C $(LIBFT_DIR)

# Nettoyage complet
fclean: clean
	rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

# Recompilation complète
re: fclean all

# Tests spécifiques
test: $(NAME)
	@echo "=== Exécution des tests ==="
	./$(NAME)
	@echo "=== Tests terminés ==="

# Nettoyage des fichiers de test créés
clean_test_files:
	rm -f output.txt c_files.txt

.PHONY: all clean fclean re test clean_test_files