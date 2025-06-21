/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:06:05 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 19:29:33 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	ft_nbrlen(int n)
{
	size_t	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	size_t		len;
	char		*res;
	long		nb;

	nb = (long) n;
	len = ft_nbrlen(nb);
	res = (char *) malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (nb < 0)
	{
		res[0] = '-';
		nb = -nb;
	}
	while (nb >= 10)
	{
		res[len - 1] = (nb % 10) + '0';
		nb /= 10;
		len--;
	}
	res[len - 1] = nb + '0';
	return (res);
}
/*
int	main(void)
{
	int	n = 0;	
	int	i = 0;
	char	*t1 = ft_itoa();
	char 	*t2 = ft_itoa(-2147483648);
	char 	*t3 = ft_itoa(-623);
 	char 	*t4 = ft_itoa(9);
 	char 	*t5 = ft_itoa(-0);
	char	*t6 = ft_itoa(2147483647);
	printf("%s\n", t1);
	printf("%s\n", t2);
	while (t2[i])
	{
		printf("%c", t2[i]);
		i++;
	}
	printf("\n");
	printf("%s\n", t3);
	printf("%s\n", t4);
	printf("%s\n", t5);
	printf("%s\n", t6);
}
*/
