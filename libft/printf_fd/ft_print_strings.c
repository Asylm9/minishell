/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_strings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 20:26:27 by agaland           #+#    #+#             */
/*   Updated: 2025/06/01 01:22:03 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf_fd.h"

static int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	if (!str)
		return (0);
	while (str[len])
		len++;
	return (len);
}

int	ft_print_char(char c, int fd)
{
	return (write(fd, &c, 1));
}

int	ft_print_str(char *str, int fd)
{
	if (!str)
		return (write(fd, "(null)", 6));
	return (write(fd, str, ft_strlen((const char *)str)));
}
