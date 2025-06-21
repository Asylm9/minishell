/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_digits.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 21:27:31 by agaland           #+#    #+#             */
/*   Updated: 2025/06/01 01:21:57 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf_fd.h"

static char	to_upper(char c)
{
	if (c >= 'a' && c <= 'z')
		return (c -= 32);
	return (c);
}

int	ft_print_digit(long n, int base_int, int fd)
{
	int				check_div;
	long			base;

	base = (long) base_int;
	if (n < 0)
	{
		if (ft_print_char('-', fd) < 0)
			return (-1);
		return (ft_print_digit(-n, base, fd) + 1);
	}
	else if (n < base)
	{
		if (ft_print_char(BASE[n], fd) < 0)
			return (-1);
		return (1);
	}
	check_div = ft_print_digit(n / base, base, fd);
	if (check_div < 0)
		return (-1);
	if (ft_print_char(BASE[n % base], fd) < 0)
		return (-1);
	return (check_div + 1);
}

int	ft_print_uns(unsigned long n, int base_int, int maj, int fd)
{
	int				check_div;
	int				check_mod;
	unsigned long	base;

	base = (unsigned long) base_int;
	if (n < base)
	{
		if (maj == 0)
			return (ft_print_char(BASE[n], fd));
		else
			return (ft_print_char(to_upper(BASE[n]), fd));
	}
	check_div = ft_print_uns(n / base, base, maj, fd);
	if (check_div < 0)
		return (-1);
	check_mod = ft_print_uns(n % base, base, maj, fd);
	if (check_mod < 0)
		return (-1);
	return (check_div + check_mod);
}

int	ft_print_address(void *ptr, int base, int maj, int fd)
{
	uintptr_t	address;
	int			count;

	if (!ptr)
		return (ft_print_str("(nil)", fd));
	count = 0;
	address = (uintptr_t) ptr;
	count = ft_print_str("0x", fd);
	if (count < 0)
		return (-1);
	count += ft_print_uns((unsigned long)address, base, maj, fd);
	if (count < 0)
		return (-1);
	return (count);
}
