/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_fd.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 20:12:09 by agaland           #+#    #+#             */
/*   Updated: 2025/06/01 02:01:44 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_FD_H
# define PRINTF_FD_H

# define BASE "0123456789abcdef"

# include <stdio.h>
# include <unistd.h>
# include <stdarg.h>
# include <limits.h>
# include <stdint.h>

int	printf_fd(int fd, const char *format, ...);

int	ft_print_char(char c, int fd);
int	ft_print_str(char *str, int fd);

int	ft_print_digit(long n, int base_int, int fd);
int	ft_print_uns(unsigned long n, int base_int, int maj, int fd);
int	ft_print_address(void *ptr, int base, int maj, int fd);

#endif
