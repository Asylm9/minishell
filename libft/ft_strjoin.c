/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:29:12 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/17 15:47:09 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	int		len;
	char	*result;

	i = 0;
	j = 0;
	if (ft_strlen(s1) + ft_strlen(s2) == 0)
		return (ft_strdup(""));
	len = (ft_strlen(s1) + ft_strlen(s2));
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	while (s1[i] != '\0')
		result[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		result[j++] = s2[i++];
	result[j] = '\0';
	return (result);
}

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

/* int	test(char *s1, char *s2, int flag, char **result)
{
	(*result) = ft_fstrjoin(&s1, &s2, flag);
	if (!(*result))
	{
		perror("ft_fstrjoin failed");
		return (1);
	}
	printf("Result: %s\n", (*result));
	// free(result);
	return (0);
}

int	main(void)
{
	char	*a;
	char	*tmp;
	char	*tmp2;

	// char	*b;
	// char	*c;
	tmp = ft_strdup("Bonjour, ");
	tmp2 = ft_strdup("comment allez vous?");
	// a = ft_fstrjoin(tmp, tmp2, 3);
	test(tmp, tmp2, 3, &a);
	// c = ft_fstrjoin(tmp, tmp2, 2);
	// c = ft_fstrjoin("Bonjour, ", "\0", 0);
	// char *d = ft_strjoin("Bonjour, ", "comment allez vous?");
	// char *e = ft_strjoin("Bonjour, ", "comment allez vous?");
	// char *f = ft_strjoin("Bonjour, ", "comment allez vous?");
	// char *g = ft_strjoin("Bonjour, ", "comment allez vous?");
	// printf("%s\n", a);
	printf("%s\n", a);
	// printf("%s\n", c);
	// free(a);
	free(a);
	// free(c);
} */
