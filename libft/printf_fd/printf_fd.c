/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 22:14:14 by agaland           #+#    #+#             */
/*   Updated: 2025/06/01 02:19:45 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf_fd.h"

static int	ft_write_error(va_list *arg)
{
	va_end(*arg);
	return (-1);
}

static int	ft_format(va_list arg, const char spec, int fd)
{
	if (spec == 'c')
		return (ft_print_char(va_arg(arg, int), fd));
	else if (spec == 's')
		return (ft_print_str(va_arg(arg, char *), fd));
	else if (spec == 'd' || spec == 'i')
		return (ft_print_digit((long)va_arg(arg, int), 10, fd));
	else if (spec == 'u')
		return (ft_print_uns((unsigned long)
				va_arg(arg, unsigned int), 10, 0, fd));
	else if (spec == 'x')
		return (ft_print_uns((unsigned long)
				va_arg(arg, unsigned int), 16, 0, fd));
	else if (spec == 'X')
		return (ft_print_uns((unsigned long)
				va_arg(arg, unsigned int), 16, 1, fd));
	else if (spec == '%')
		return (ft_print_char('%', fd));
	else if (spec == 'p')
		return (ft_print_address(va_arg(arg, void *), 16, 0, fd));
	return (0);
}

int	printf_fd(int fd, const char *format, ...)
{
	va_list	arg;
	int		count;
	int		add;

	count = 0;
	add = 0;
	va_start(arg, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '\0')
				break ;
			add = ft_format(arg, *format, fd);
		}
		else
			add = write(fd, format, 1);
		if (add < 0)
			return (ft_write_error(&arg));
		count += add;
		format++;
	}
	va_end(arg);
	return (count);
}
