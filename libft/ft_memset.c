/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 20:30:41 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 21:21:28 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*dst;

	dst = (unsigned char *) b;
	while (len--)
		*dst++ = (unsigned char) c;
	return ((void *) b);
}

/* # include <stdio.h>

int	main (void)
{
	char		tab1[10] = "meow";
	char		tab2[10] = "meow";
	size_t	len = sizeof(tab1);
	size_t	i;

	printf("tab1: %s\n", tab1);
	printf("tab2: %s\n", tab2);

	ft_memset(&tab1, '3', len);
	memset(&tab2, '5', 2);

	printf("tab1:\n");
	i = 0;
	while (i < 10)
	{
		printf("%c", tab1[i]);
		i++;
	}

	printf("\n");

	i = 0;
	printf("tab2:\n");
	while (i < 10)
	{
		printf("%c", tab2[i]);
		i++;
	}

	printf("\n");
} */
