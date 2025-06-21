/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:21:43 by agaland           #+#    #+#             */
/*   Updated: 2025/06/01 02:21:03 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

static char	*allocate_word(char const *s, size_t start, size_t len)
{
	size_t	i;
	char	*word;

	word = ft_calloc((len + 1), sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i++] = s[start++];
	}
	word[i] = '\0';
	return (word);
}

void	free_split(char ***split, size_t words)
{
	size_t	i;

	i = 0;
	while (i < words)
	{
		free((*split)[i]);
		i++;
	}
	free(*split);
	split = NULL;
}

static char	**fill_split(char **split, char const *s, char c, size_t word_count)
{
	size_t	i;
	size_t	start;
	size_t	len;

	i = 0;
	start = 0;
	while (i < word_count)
	{
		while (s[start] == c)
			start++;
		len = 0;
		while (s[start + len] && s[start + len] != c)
			len++;
		split[i] = allocate_word(s, start, len);
		if (!split[i])
		{
			free_split(&split, i);
			return (NULL);
		}
		start += len;
		i++;
	}
	split[i] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	if (word_count == 0)
	{
		split = ft_calloc(1, sizeof(char *));
		return (split);
	}
	split = ft_calloc((word_count + 1), sizeof(char *));
	if (!split)
		return (NULL);
	split = fill_split(split, s, c, word_count);
	return (split);
}

/* int	main(int ac, char **av)
{
	size_t		i;
	//char const	*s = av[1];
	//char		c = av[2][0];
	//char const	*s = "Hell,or,,nooon,";
	//char		*s = "hello!";+
	//char		c = ',';

	char		**result = ft_split(av[1], av[2][0]);
	//ft_split(av[1], av[2][0]);
	//char		**result = ft_split(av[1], av[2][0]);
	
	i = 0;
	while (result[i])
	{
		printf("%s\n", result[i]);
		i++;
	}
	free_split(&result, i);
} */
