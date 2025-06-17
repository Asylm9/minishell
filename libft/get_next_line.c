/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:25:18 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/17 15:47:25 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*devers(char *stash, char *filled)
{
	char	*temp;

	temp = ft_fstrjoin(&stash, &filled, 0);
	ft_free(stash);
	return (temp);
}

char	*fill_stash(int fd, char *stash)
{
	char	*filled;
	ssize_t	lu;

	if (!stash)
		stash = ft_calloc(1, 1);
	if (!stash)
		return (NULL);
	filled = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!filled)
		return (free(stash), NULL);
	lu = 1;
	while (lu > 0)
	{
		lu = read(fd, filled, BUFFER_SIZE);
		if (lu == -1)
			return (free(filled), NULL);
		filled[lu] = '\0';
		stash = devers(stash, filled);
		if (!stash)
			return (free(filled), NULL);
		if (is_line(filled))
			break ;
	}
	free(filled);
	return (stash);
}

char	*fill_line(char *stash)
{
	int		i;
	char	*result;

	i = 0;
	if (!stash[i])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		result = (char *)malloc(sizeof(char) * (i + 2));
	else
		result = (char *)malloc(sizeof(char) * (i + 1));
	if (!result)
		return (free(result), NULL);
	i = 0;
	while (stash[i] != '\n' && stash[i] != '\0')
	{
		result[i] = stash[i];
		i++;
	}
	if (stash[i] && stash[i] == '\n')
		result[i++] = '\n';
	result[i] = '\0';
	return (result);
}

char	*cut_stash(char *stash, char *line)
{
	char	*result;
	int		i;
	int		len;

	i = 0;
	len = 0;
	result = NULL;
	while (stash[i] == line[i] && line[i] != '\0' && stash[i] != '\0')
	{
		len++;
		i++;
	}
	if (i != 0)
	{
		result = ft_calloc((size_t)(ft_strlen(stash) - (len - 1)), 1);
		if (!result)
			return (free(stash), NULL);
		i = 0;
		while (stash[len] != '\0')
			result[i++] = stash[len++];
		result[i] = '\0';
	}
	else
		result[i] = '\0';
	return (free(stash), result);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = fill_stash(fd, stash);
	if (!stash)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = fill_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = cut_stash(stash, line);
	return (line);
}
