# Makefile pour tester la partie exécution du minishell

NAME = minishell

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g 
LIBS = -lreadline -lhistory

# Répertoires
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
OBJS_DIR = objs

# Configuration des chemins de recherche pour les fichiers sources
vpath %.c .
vpath %.c srcs/exec
vpath %.c srcs/builtins

# Fichiers sources (noms seulement)
ROOT_SRCS = matt.c token.c expander.c parser.c
EXEC_SRCS = env_utils.c path.c exec_utils.c list_utils.c manual_init.c new_exec.c redirections.c resources.c utils.c
BUILTINS_SRCS = builtins_utils.c cd.c echo.c env.c exit.c pwd.c unset.c export.c export_sorting.c

# Tous les fichiers sources
ALL_SRCS = $(ROOT_SRCS) $(EXEC_SRCS) $(BUILTINS_SRCS)

# Fichiers objets dans objs/
OBJS = $(addprefix $(OBJS_DIR)/, $(ALL_SRCS:.c=.o))

# Règle principale
all: $(NAME)

# Compilation de libft
$(LIBFT):
	@make -C $(LIBFT_DIR)

# Création du dossier objs
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# Compilation du programme principal
$(NAME): $(OBJS_DIR) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

# Règle pattern unique pour compiler tous les .c vers objs/
# vpath se charge de trouver les fichiers dans les bons répertoires
$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -rf $(OBJS_DIR)
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
	@echo "ROOT_SRCS: $(ROOT_SRCS)"
	@echo "EXEC_SRCS: $(EXEC_SRCS)"
	@echo "BUILTINS_SRCS: $(BUILTINS_SRCS)"
	@echo "ALL_SRCS: $(ALL_SRCS)"
	@echo "OBJS: $(OBJS)"

.PHONY: all clean fclean re test clean_test_files debug