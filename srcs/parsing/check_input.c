/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:18:33 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/16 10:34:21 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_empty(char *input)
{
	int	i;

	i = 0;
	if (input)
	{
		while (input[i])
		{
			if (input[i] != ' ')
				return (ERROR);
			i++;
		}
	}
	return (SUCCESS);
}

char	next_input(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] == ' ')
		i++;
	return (input[i]);
}

int	check_input(char *input, t_sh *shell)
{
	int		i;
	int		size;
	char	delim[2];

	i = -1;
	size = 0;
	if (is_empty(input) == SUCCESS || ft_strlen(input) == 0)
		return (ERROR);
	while (input[++i])
	{
		size++;
		if (input[i] == '\'' || input[i] == '"')
			if (check_quotes(input, &i, &size, shell) == ERROR)
				return (ERROR);
		if (input[i] == '&')
			if (check_ampersand(input, i, shell) == ERROR)
				return (ERROR);
		if (input[i] == '!' && ft_strlen(input) == 1)
			return (shell->exit_status = 1, ERROR);
		if (input[i] != ' ')
			delim[0] = input[i];
	}
	if (check_special_delim(delim[0], size, shell) == ERROR)
		return (ERROR);
	return (SUCCESS);
}
