/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:27:07 by agaland           #+#    #+#             */
/*   Updated: 2024/10/30 20:13:53 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	src_len;
	size_t	dst_len;

	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	i = dst_len;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	j = 0;
	while (src[j] && i + j < dstsize - 1)
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst_len + src_len);
}
/*
int	main(void)
{
	char		dst[150] = "a    :*86kljgk,kihew    OK:KLHGFDGJYILYkl";
	size_t		dstsize = sizeof(dst);
	const char	*src = "lorem ipsum dolor sit amet";

	printf("ft_strlcat:\n");
	printf("%zu\n", ft_strlcat(dst, src, dstsize));
	printf("%s\n\n", dst);
	//printf("%zu\n", ft_strlcat(dst, src, 10));

	printf("dstsize : %zu\n\n", dstsize);

	printf("Strlcat:\n");
	printf("%zu\n", strlcat(dst, src, dstsize));
	printf("%s\n", dst);
	//printf("%zu\n", ft_strlcat(dst, src, 10));
	//printf("%zu\n", strlcat(dst, src, 10));

}
*/
