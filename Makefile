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

# Fichiers sources à la racine
ROOT_SRCS = matt.c \
			token.c \
			expander.c \
			parser.c

# Fichiers sources dans srcs/exec/
EXEC_SRCS = srcs/exec/env_utils.c \
			srcs/exec/path.c \
			srcs/exec/exec_utils.c \
			srcs/exec/list_utils.c \
			srcs/exec/manual_init.c \
			srcs/exec/new_exec.c \
			srcs/exec/redirections.c \
			srcs/exec/resources.c \
			srcs/exec/utils.c

# Fichiers sources dans srcs/builtins/
BUILTINS_SRCS = srcs/builtins/builtins_utils.c \
				srcs/builtins/cd.c \
				srcs/builtins/echo.c \
				srcs/builtins/env.c \
				srcs/builtins/exit.c \
				srcs/builtins/pwd.c \
				srcs/builtins/unset.c \
				srcs/builtins/export.c \
				srcs/builtins/export_sorting.c

# Tous les fichiers sources
SRCS = $(ROOT_SRCS) $(EXEC_SRCS) $(BUILTINS_SRCS)

# Fichiers objets (nom de base seulement dans objs/)
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))

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

# Règles pour fichiers à la racine
$(OBJS_DIR)/matt.o: matt.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/token.o: token.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/expander.o: expander.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/parser.o: parser.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Règles pour srcs/exec/
$(OBJS_DIR)/env_utils.o: srcs/exec/env_utils.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/path.o: srcs/exec/path.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/exec_utils.o: srcs/exec/exec_utils.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/list_utils.o: srcs/exec/list_utils.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/manual_init.o: srcs/exec/manual_init.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/new_exec.o: srcs/exec/new_exec.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/redirections.o: srcs/exec/redirections.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/resources.o: srcs/exec/resources.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/utils.o: srcs/exec/utils.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Règles pour srcs/builtins/
$(OBJS_DIR)/builtins_utils.o: srcs/builtins/builtins_utils.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/cd.o: srcs/builtins/cd.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/echo.o: srcs/builtins/echo.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/env.o: srcs/builtins/env.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/exit.o: srcs/builtins/exit.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/pwd.o: srcs/builtins/pwd.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/unset.o: srcs/builtins/unset.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/export.o: srcs/builtins/export.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/export_sorting.o: srcs/builtins/export_sorting.c | $(OBJS_DIR)
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
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"

.PHONY: all clean fclean re test clean_test_files debug