/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:28:57 by agaland           #+#    #+#             */
/*   Updated: 2024/10/22 17:20:45 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*last_occur;

	last_occur = NULL;
	while (*s)
	{
		if ((char) *s == (char) c)
			last_occur = (char *) s;
		s++;
	}
	if ((char) c == '\0')
		return ((char *) s);
	return (last_occur);
}
/*
int	main(void)
{
	const char	*s = "Derivations";
	int	c = 'i';

	printf("%s\n", ft_strrchr(&s[1], c));
	printf("%s\n", strrchr(&s[1], c));
}
*/	
