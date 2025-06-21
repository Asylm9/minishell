# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agaland <agaland@student.s19.be>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/16 19:27:12 by agaland           #+#    #+#              #
#    Updated: 2025/06/01 14:36:26 by agaland          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_isspec.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_bzero.c ft_memchr.c ft_strchr.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_calloc.c ft_strdup.c
OBJS = $(SRCS:%.c=%.o)

SUPP_SRCS = ft_substr.c ft_strjoin.c ft_strtrim.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_split.c
SUPP_OBJS = $(SUPP_SRCS:%.c=%.o)

BONUS_SRCS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c
BONUS_OBJS = $(BONUS_SRCS:%.c=%.o)

PRINTF_FD_DIR = printf_fd
PRINTF_FD_LIB = $(PRINTF_FD_DIR)/libftprintf.a

ALL_OBJS = $(OBJS) $(SUPP_OBJS)

CC = gcc

CCFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(ALL_OBJS) $(PRINTF_FD_LIB)
	@ar rcs $(NAME) $(LIBFT_OBJS)
	@ar x $(PRINTF_FD_LIB)
	@ar rcs $(NAME) *.o

%.o: %.c
	@$(CC) $(CCFLAGS) -c $< -o $@

$(PRINTF_FD_LIB):
	@$(MAKE) -C $(PRINTF_FD_DIR)

bonus: all $(NAME) $(BONUS_OBJS)
	@ar rcs $(NAME) $(BONUS_OBJS)
clean:	
	@rm -f $(ALL_OBJS) $(BONUS_OBJS)
	@$(MAKE) -C $(PRINTF_FD_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(PRINTF_FD_DIR) fclean

re:	fclean all

.PHONY: all clean fclean re bonus
