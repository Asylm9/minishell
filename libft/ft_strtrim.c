/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 01:18:22 by agaland           #+#    #+#             */
/*   Updated: 2024/10/30 21:44:49 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_trim(char const c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*res;

	if (!s1 || !set)
		return (NULL);
	len = ft_strlen(s1);
	if (len == 0)
		return (ft_strdup(""));
	start = 0;
	end = len - 1;
	while ((s1[start]) && (is_trim(s1[start], set)))
		start++;
	while (end > start && (is_trim(s1[end], set)))
		end--;
	res = (char *) malloc((end - start + 2) * sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, &s1[start], (end - start + 2));
	return (res);
}
/*
int	main(void)
{
	char const	*s1;
	char const	*set;
	char		*str;

	s1 = " ";
	set = "";
	//s1 = "&*!!!&*belette&&*!";
	//set = "&!*";
	str = ft_strtrim(s1, set);

	printf("%s\n", str);

	free(str);
}
*/
