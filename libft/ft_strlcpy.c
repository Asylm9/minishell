/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:36:16 by agaland           #+#    #+#             */
/*   Updated: 2024/10/30 19:37:15 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char*src, size_t dstsize)
{
	size_t	src_size;
	size_t	i;

	src_size = ft_strlen(src);
	if (dstsize == 0)
		return (src_size);
	i = 0;
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_size);
}
/*
int	main(void)
{
	const char	*src = "helloooow";
	char	dst[5];
	size_t	size = 0;

	printf("%zu\n", ft_strlcpy(dst, src, size));
	printf("%zu\n", strlcpy(dst, src, size));

	ft_strlcpy(dst, src, size);
	printf("%s\n", dst);
	strlcpy(dst, src, size);
	printf("%s\n", dst);
}
*/
