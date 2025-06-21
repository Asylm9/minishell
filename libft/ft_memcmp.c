/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:33:51 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 20:40:44 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	if (n == 0)
		return (0);
	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	while (i < n - 1)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (str1[i] - str2[i]);
}

/* # include <stdio.h>

int	main(void)
{
	printf("%d\n", ft_memcmp("Hello", "Hello", 5));
	printf("%d\n", ft_memcmp("perruche", "perruche", 7));
	
	printf("%d\n", memcmp("Hello", "", 5));
	printf("%d\n", memcmp("", "perruche", 7));
} */
