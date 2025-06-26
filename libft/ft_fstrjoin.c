/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fstrjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:14:26 by agaland           #+#    #+#             */
/*   Updated: 2025/06/26 19:50:50 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_fstrjoin(char **s1, char **s2, int flag)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = 0;
	len2 = 0;
	if (s1 && *s1)
		len1 = ft_strlen(*s1);
	if (s2 && *s2)
		len2 = ft_strlen(*s2);
	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	if (s1 && *s1)
		ft_memcpy(result, *s1, len1);
	if (s2 && *s2)
		ft_memcpy(result + len1, *s2, len2);
	result[len1 + len2] = '\0';
	if (flag == 1 || flag == 3)
		if (s1 && *s1)
			free(*s1);
	if (flag == 2 || flag == 3)
		if (s2 && *s2)
			free(*s2);
	return (result);
}
