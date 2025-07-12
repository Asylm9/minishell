# Nom de l'executable
NAME = minishell

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Dossiers sources et objets
SRCDIR = .
OBJ_DIR = objs

# Fichiers sources avec chemins complets
SRCS =  $(SRCDIR)/srcs/main.c \
        $(SRCDIR)/srcs/init.c \
        $(SRCDIR)/srcs/signal.c \
        $(SRCDIR)/srcs/parsing/check_input.c \
        $(SRCDIR)/srcs/parsing/input_errors.c \
        $(SRCDIR)/srcs/parsing/list_utils.c \
        $(SRCDIR)/srcs/parsing/token.c \
        $(SRCDIR)/srcs/parsing/expander.c \
        $(SRCDIR)/srcs/parsing/parser.c \
        $(SRCDIR)/srcs/free.c \
        $(SRCDIR)/srcs/heredoc.c \
        $(SRCDIR)/srcs/exec/env_utils.c \
        $(SRCDIR)/srcs/exec/path.c \
        $(SRCDIR)/srcs/exec/exec_utils.c \
        $(SRCDIR)/srcs/exec/list_utils.c \
        $(SRCDIR)/srcs/exec/new_exec.c \
        $(SRCDIR)/srcs/exec/redirections.c \
        $(SRCDIR)/srcs/exec/resources.c \
        $(SRCDIR)/srcs/exec/utils.c \
        $(SRCDIR)/srcs/builtins/builtins_utils.c \
        $(SRCDIR)/srcs/builtins/cd.c \
        $(SRCDIR)/srcs/builtins/echo.c \
        $(SRCDIR)/srcs/builtins/env.c \
        $(SRCDIR)/srcs/builtins/exit.c \
        $(SRCDIR)/srcs/builtins/pwd.c \
        $(SRCDIR)/srcs/builtins/unset.c \
        $(SRCDIR)/srcs/builtins/export.c \
        $(SRCDIR)/srcs/builtins/export_sorting.c

# Création des objets à partir des sources
OBJ = $(SRCS:$(SRCDIR)/%.c=$(OBJ_DIR)/%.o)

# Libft path
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
# Librairies à linker
LIBS = $(LIBFT) -lreadline -lhistory

# Compilation principale
all: $(NAME)

# Règle pour compiler le programme
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

# Création du dossier objs et compilation des objets
$(OBJ_DIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour compiler libft
$(LIBFT):
	@make -C $(LIBFT_DIR)

# Nettoyage des objets
clean:
	rm -rf $(OBJ_DIR)
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

# Affichage de la structure pour debug
debug:
	@echo "SRCS: $(SRCS)"
	@echo "OBJ: $(OBJ)"

.PHONY: all clean fclean re test clean_test_files debug