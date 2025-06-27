/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:18:33 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/27 18:18:53 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_input(char *input, t_sh *shell)
{
	int		i;
	int		size;
	bool	valid;
	char	quote;

	// char	delim[2];
	i = 0;
	size = 0;
	valid = 0;
	while (input[i])
	{
		if (ft_isalnum(input[i]) || input[i] == '$')
		{
			valid = 1;
			size++;
		}
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
			{
				if (ft_isalnum(input[i]) || input[i] == '$')
					valid = 1;
				i++;
			}
		}
		if (input[i] == '|')
			if (input[i + 1] == '|')
			{
				shell->exit_status = 2;
				return (ERROR);
			}
		if (input[i] == '<')
			if (input[i + 1] == '|' || input[i + 1] == '>')
			{
				shell->exit_status = 2;
				return (ERROR);
			}
		if (input[i] == '>')
			if (input[i + 1] == '|' || input[i + 1] == '<')
			{
				shell->exit_status = 2;
				return (ERROR);
			}
		i++;
	}
	if (valid == 1)
		return (SUCCESS);
	else
	{
		shell->exit_status = 2;
		return (ERROR);
	}
}
