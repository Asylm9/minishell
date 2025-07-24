# Nom de l'executable
NAME = minishell

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
MAKEFLAGS += --no-print-directory

# Couleurs pour les flammes
RED = \033[0;31m
ORANGE = \033[0;33m
CORAL = \033[38;2;255;127;80m
BLUE = \033[38;2;52;152;219m
MINT = \033[38;2;152;251;152m
YELLOW = \033[1;33m
RESET = \033[0m
BOLD = \033[1m
GREEN = \033[0;32m

# Dossiers sources et objets
SRCDIR = .
OBJ_DIR = objs

# Fichiers sources avec chemins complets
SRCS =  $(SRCDIR)/srcs/main.c \
        $(SRCDIR)/srcs/main_utils.c \
        $(SRCDIR)/srcs/init.c \
        $(SRCDIR)/srcs/signal.c \
        $(SRCDIR)/srcs/signal2.c \
        $(SRCDIR)/srcs/parsing/check_input.c \
        $(SRCDIR)/srcs/parsing/input_errors.c \
        $(SRCDIR)/srcs/parsing/list_utils.c \
        $(SRCDIR)/srcs/parsing/token.c \
        $(SRCDIR)/srcs/parsing/exp_token.c \
        $(SRCDIR)/srcs/parsing/exp_utils.c \
        $(SRCDIR)/srcs/parsing/exp_utils_2.c \
        $(SRCDIR)/srcs/parsing/expander.c \
        $(SRCDIR)/srcs/parsing/errors.c \
        $(SRCDIR)/srcs/parsing/parser.c \
        $(SRCDIR)/srcs/free.c \
        $(SRCDIR)/srcs/heredoc.c \
        $(SRCDIR)/srcs/heredoc2.c \
        $(SRCDIR)/srcs/exec/exec_ast.c \
        $(SRCDIR)/srcs/exec/exec_utils.c \
        $(SRCDIR)/srcs/exec/exec_builtins.c \
        $(SRCDIR)/srcs/exec/list_utils.c \
        $(SRCDIR)/srcs/exec/redirections.c \
        $(SRCDIR)/srcs/exec/env_utils.c \
        $(SRCDIR)/srcs/exec/path1.c \
        $(SRCDIR)/srcs/exec/path2.c \
        $(SRCDIR)/srcs/exec/resources.c \
        $(SRCDIR)/srcs/exec/utils.c \
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
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	@echo -e "$(RED)"
	@echo -e "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣦⣀⠀⠀⠀⠀⠀⠀⢲⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡆⠀⠀⠀⠀⠀⠀⠀⠛⣦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo -e "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣷⣤⠀⠀⠀⠀⠀⢻⣿⣷⣄⢀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo -e "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣷⣄⠀⠀⠀⠀⣿⣿⣿⣷⠱⣆⠀⠀⠀⢀⣾⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo -e "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣽⣿⡆⠀⠀⠀⢸⣿⣞⣿⣧⢸⣷⣤⠀⢸⣿⣯⣿⠆⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡟⢰⣿⡷⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⣿⣦⡄⠀⠀⠀⠀⢻⣿⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo -e "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣟⣾⣿⠀⠀⠀⣼⣿⡏⣿⣿⠀⣿⣿⣗⠺⣿⣳⣿⣧⠀⠀⠀⠀⠀⠀⠀⣴⣿⡟⠀⣸⣿⡟⣽⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣆⠀⠀⠀⠀⣿⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo -e "$(ORANGE)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣦⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣯⢿⣿⡇⠀⣰⣿⡿⢸⣿⡿⠀⣼⣿⣻⡦⣿⣯⢿⣿⡆⠀⠀⠀⠀⢀⣾⣿⣿⠀⢠⣿⡿⢱⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⢯⣿⠀⠀⢠⠀⣼⣿⣻⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo -e "$(ORANGE)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣷⡀⠀⠀⠀⠀⠀⣰⣿⣿⣞⣿⣿⠃⢀⣿⡿⣡⣿⡿⠃⢀⣿⣿⣽⣷⢹⣿⣻⢿⣿⡄⠀⠀⢀⣾⣿⢿⡇⠀⣾⣿⣱⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⠁⢀⡿⢰⣿⣣⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo -e "$(YELLOW)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⡄⠀⠀⠀⢰⣿⣿⣻⣼⣿⡿⠀⢸⣿⢣⣿⡿⠁⣠⣿⣿⡟⣾⣿⢈⣿⣯⣟⣿⣷⠀⠀⣸⣿⣟⣿⡇⠀⣿⣧⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣳⣿⡟⠀⣾⡇⢸⣷⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo -e "$(YELLOW)⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⡄⠀⢠⣿⣿⢯⣳⣿⣿⣦⣄⠘⣿⣿⡿⠁⣴⣿⣿⢯⣽⣿⡟⢀⣿⣷⢯⣿⣿⡇⠀⣿⣿⣽⣻⣿⠀⣿⣷⣿⠇⠀⠀⠀⠀⣀⠀⠀⢀⣼⣿⣿⣳⢿⣿⠁⣰⣿⣇⢸⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠁⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo -e "$(RED)⠀⠀⠀⠀⠀⠀⠀⢠⣾⠇⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⢿⣷⠀⣿⣿⢯⣟⣼⣿⡇⢻⣿⣧⠘⣿⠃⣼⣿⡿⡽⣞⣿⣿⠁⢸⣿⣟⣮⢿⣿⡇⠀⣿⣿⢶⣻⣿⣇⠘⣿⣿⠀⠀⢀⣴⡿⠁⠀⣠⣿⢱⣿⣯⣽⣻⣿⠀⣿⡿⣿⡄⢻⣧⠀⠀⠀⠀⠀⠀⣴⣾⡟⠁⠀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo -e ""
	@echo -e "$(BOLD)$(YELLOW)                             🐚  MINISHELL : $(BOLD)$(RED)COMPILATION SUCCESSFUL$(RESET) 🪸"
	@echo -e ""

# Création du dossier objs et compilation des objets
$(OBJ_DIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "$(CORAL)                                       object files compiling...$(RESET)"

# Règle pour compiler libft
$(LIBFT):
	@make -C $(LIBFT_DIR)
	@echo -e "															"
	@echo -e "$(MINT)                                     LIBFT compilation : success !$(RESET)"
	@echo -e "															"

# Nettoyage des objets
clean:
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	@if [ "$(MAKECMDGOALS)" = "clean" ]; then \
		echo -e "           $(GREEN)✓ Clean completed$(RESET)"; \
	fi

# Nettoyage complet
fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo -e "															"
	@echo -e "                                        $(GREEN)✓ Full clean completed$(RESET)"
	@echo -e "															"

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