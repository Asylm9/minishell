/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 20:18:36 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 20:22:19 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*src;
	unsigned char	char_to_find;

	src = (unsigned char *) s;
	char_to_find = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		if (src[i] == char_to_find)
			return ((void *) &src[i]);
		i++;
	}
	return (NULL);
}
/* int	main(void)
{
	const char *s1 = "babiroussa";
//	const char *s2 = "blrblbrl";
	int			c = 's';
	size_t		n = 8; 
	void		*result = ft_memchr(s1, c, n);

	if (result)
	{
		printf("%s\n", (char *)result);
		printf("%s\n", (char *)result);
	}
} */
