/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:24:41 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 18:45:47 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t			i;
	unsigned char	*tab;

	if (size != 0 && count > (SIZE_MAX / size))
		return (NULL);
	tab = (unsigned char *) malloc(count * size);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < count * size)
	{
		tab[i] = 0;
		i++;
	}
	return ((void *) tab);
}

/* int	main(void)
{
	size_t	i;
	size_t	count;

	count = 10;
	int	*tab = (int *) ft_calloc(count, sizeof(int));

	i = 0;
	while (i < count)
	{
		printf("%d\n", tab[i]);
		i++;
	}
	free(tab);
} */
