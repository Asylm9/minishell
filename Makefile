NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
MAKEFLAGS += --no-print-directory

RED = \033[0;31m
ORANGE = \033[0;33m
CORAL = \033[38;2;255;127;80m
BLUE = \033[38;2;52;152;219m
MINT = \033[38;2;152;251;152m
YELLOW = \033[1;33m
RESET = \033[0m
BOLD = \033[1m
GREEN = \033[0;32m

SRCDIR = .
OBJ_DIR = objs

SRCS =  $(SRCDIR)/srcs/main.c \
        $(SRCDIR)/srcs/main_utils.c \
        $(SRCDIR)/srcs/init.c \
        $(SRCDIR)/srcs/signal.c \
        $(SRCDIR)/srcs/signal2.c \
	    $(SRCDIR)/srcs/free.c \
        $(SRCDIR)/srcs/resources.c \
        $(SRCDIR)/srcs/wrappers.c \
        $(SRCDIR)/srcs/parsing/check_input.c \
        $(SRCDIR)/srcs/parsing/input_errors.c \
        $(SRCDIR)/srcs/parsing/list_utils.c \
        $(SRCDIR)/srcs/parsing/token.c \
        $(SRCDIR)/srcs/parsing/exp_token.c \
        $(SRCDIR)/srcs/parsing/exp_utils.c \
        $(SRCDIR)/srcs/parsing/exp_utils_2.c \
        $(SRCDIR)/srcs/parsing/exp_utils_3.c \
        $(SRCDIR)/srcs/parsing/expander.c \
		$(SRCDIR)/srcs/parsing/heredoc.c \
        $(SRCDIR)/srcs/parsing/heredoc2.c \
        $(SRCDIR)/srcs/parsing/errors.c \
        $(SRCDIR)/srcs/parsing/parser.c \
        $(SRCDIR)/srcs/exec/exec_ast.c \
        $(SRCDIR)/srcs/exec/exec_utils.c \
        $(SRCDIR)/srcs/exec/exec_builtins.c \
        $(SRCDIR)/srcs/exec/list_utils.c \
        $(SRCDIR)/srcs/exec/redirections.c \
        $(SRCDIR)/srcs/exec/env_utils.c \
        $(SRCDIR)/srcs/exec/path1.c \
        $(SRCDIR)/srcs/exec/path2.c \
        $(SRCDIR)/srcs/exec/utils.c \
        $(SRCDIR)/srcs/builtins/cd.c \
        $(SRCDIR)/srcs/builtins/echo.c \
        $(SRCDIR)/srcs/builtins/env.c \
        $(SRCDIR)/srcs/builtins/exit.c \
        $(SRCDIR)/srcs/builtins/pwd.c \
        $(SRCDIR)/srcs/builtins/unset.c \
        $(SRCDIR)/srcs/builtins/export.c \
        $(SRCDIR)/srcs/builtins/export_sorting.c

OBJ = $(SRCS:$(SRCDIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

LIBS = $(LIBFT) -lreadline -lhistory

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	@echo "$(RED)"
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣦⣀⠀⠀⠀⠀⠀⠀⢲⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡆⠀⠀⠀⠀⠀⠀⠀⠛⣦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣷⣤⠀⠀⠀⠀⠀⢻⣿⣷⣄⢀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣷⣄⠀⠀⠀⠀⣿⣿⣿⣷⠱⣆⠀⠀⠀⢀⣾⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣽⣿⡆⠀⠀⠀⢸⣿⣞⣿⣧⢸⣷⣤⠀⢸⣿⣯⣿⠆⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡟⢰⣿⡷⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⣿⣦⡄⠀⠀⠀⠀⢻⣿⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣟⣾⣿⠀⠀⠀⣼⣿⡏⣿⣿⠀⣿⣿⣗⠺⣿⣳⣿⣧⠀⠀⠀⠀⠀⠀⠀⣴⣿⡟⠀⣸⣿⡟⣽⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣆⠀⠀⠀⠀⣿⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "$(ORANGE)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣦⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣯⢿⣿⡇⠀⣰⣿⡿⢸⣿⡿⠀⣼⣿⣻⡦⣿⣯⢿⣿⡆⠀⠀⠀⠀⢀⣾⣿⣿⠀⢠⣿⡿⢱⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⢯⣿⠀⠀⢠⠀⣼⣿⣻⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo "$(ORANGE)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣷⡀⠀⠀⠀⠀⠀⣰⣿⣿⣞⣿⣿⠃⢀⣿⡿⣡⣿⡿⠃⢀⣿⣿⣽⣷⢹⣿⣻⢿⣿⡄⠀⠀⢀⣾⣿⢿⡇⠀⣾⣿⣱⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⠁⢀⡿⢰⣿⣣⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo "$(YELLOW)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⡄⠀⠀⠀⢰⣿⣿⣻⣼⣿⡿⠀⢸⣿⢣⣿⡿⠁⣠⣿⣿⡟⣾⣿⢈⣿⣯⣟⣿⣷⠀⠀⣸⣿⣟⣿⡇⠀⣿⣧⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣳⣿⡟⠀⣾⡇⢸⣷⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo "$(YELLOW)⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⡄⠀⢠⣿⣿⢯⣳⣿⣿⣦⣄⠘⣿⣿⡿⠁⣴⣿⣿⢯⣽⣿⡟⢀⣿⣷⢯⣿⣿⡇⠀⣿⣿⣽⣻⣿⠀⣿⣷⣿⠇⠀⠀⠀⠀⣀⠀⠀⢀⣼⣿⣿⣳⢿⣿⠁⣰⣿⣇⢸⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠁⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo "$(RED)⠀⠀⠀⠀⠀⠀⠀⢠⣾⠇⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⢿⣷⠀⣿⣿⢯⣟⣼⣿⡇⢻⣿⣧⠘⣿⠃⣼⣿⡿⡽⣞⣿⣿⠁⢸⣿⣟⣮⢿⣿⡇⠀⣿⣿⢶⣻⣿⣇⠘⣿⣿⠀⠀⢀⣴⡿⠁⠀⣠⣿⢱⣿⣯⣽⣻⣿⠀⣿⡿⣿⡄⢻⣧⠀⠀⠀⠀⠀⠀⣴⣾⡟⠁⠀⠀⠀⠀⠀⠀⠀$(RESET)"
	@echo ""
	@echo "$(BOLD)$(YELLOW)                             🐚  MINISHELL : $(BOLD)$(MINT)COMPILATION SUCCESSFUL$(RESET) 🪸"
	@echo ""

$(OBJ_DIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(ORANGE)                                       object files compiling...$(RESET)"

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@echo "															"
	@echo "$(MINT)                                     LIBFT compilation : success !$(RESET)"
	@echo "															"

clean:
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)
	@if [ "$(MAKECMDGOALS)" = "clean" ]; then \
		echo "           $(GREEN)✓ Clean completed$(RESET)"; \
	fi

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "															"
	@echo "                                        $(GREEN)✓ Full clean completed$(RESET)"
	@echo "															"

re: fclean all

run: $(NAME)
	@./minishell

clean_test_files:
	rm -f output.txt c_files.txt


.PHONY: all clean fclean re test clean_test_files debug