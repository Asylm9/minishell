/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:18:33 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/07 20:25:31 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty(char *input)
{
	int	i;

	i = 0;
	if (input)
		while (input[i])
		{
			if (input[i] != ' ')
				return (ERROR);
			i++;
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
	bool	valid;
	char	quote;
	char	delim[2];

	i = 0;
	size = 0;
	valid = 0;
	if (is_empty(input) == SUCCESS || ft_strlen(input) == 0)
		return (ERROR);
	while (input[i])
	{
		if (ft_isalnum(input[i]) || input[i] == '$' || input[i] == '.'
			|| input[i] == '-' || input[i] == '~' || input[i] == '/'
			|| input[i] == '&' || input[i] == ':' || input[i] == '|')
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
				{
					valid = 1;
					size++;
				}
				i++;
			}
			if (input[i] == '\0')
			{
				printf_fd(STDERR,
					"minishell: unexpected EOF while looking for matching `%c'\n",
					quote);
				shell->exit_status = 2;
				return (ERROR);
			}
		}
		if (input[i] == '&')
		{
			if (next_input(&input[i + 1]) == '&')
			{
				printf_fd(STDERR, " syntax error near unexpected token `&&'\n");
				shell->exit_status = 2;
				return (ERROR);
			}
			printf_fd(STDERR, " invalid caracter : '&'\n");
			shell->exit_status = 2;
			return (ERROR);
		}
		/* if (input[i] == '|')
		{
			if (next_input(&input[i + 1]) == '|' && input[i + 1] != ' ')
			{
				printf_fd(STDERR,
					"minishell: syntax error near unexpected token `||'\n");
				shell->exit_status = 2;
				return (ERROR);
			}
			else if (next_input(&input[i + 1]) == '\0' || (input[i + 1] == ' '
					&& next_input(&input[i + 1]) == '|'))
			{
				printf_fd(STDERR, " syntax error near unexpected token `|'\n");
				shell->exit_status = 2;
				return (ERROR);
			}
		}
		if (input[i] == '<')
		{
			if (next_input(&input[i + 1]) == '|' || next_input(&input[i
					+ 1]) == '>')
			{
				printf_fd(STDERR,
					"minishell: syntax error near unexpected token `<'\n");
				shell->exit_status = 2;
				return (ERROR);
			}
			if (next_input(&input[i] + 1) == '\0')
			{
				printf_fd(STDERR,
					" syntax error near unexpected token `newline'\n");
				shell->exit_status = 2;
				return (ERROR);
			}
		}
		if (input[i] == '>')
		{
			if (next_input(&input[i + 1]) == '|' || next_input(&input[i
					+ 1]) == '<')
			{
				printf_fd(STDERR,
					"minishell: syntax error near unexpected token `>'\n");
				shell->exit_status = 2;
				return (ERROR);
			}
			if (next_input(&input[i] + 1) == '\0')
			{
				printf_fd(STDERR,
					" syntax error near unexpected token `newline'\n");
				shell->exit_status = 2;
				return (ERROR);
			}
		} */
		if (input[i] == '!' && ft_strlen(input) == 1)
		{
			shell->exit_status = 1;
			return (ERROR);
		}
		if (input[i] != ' ')
			delim[0] = input[i];
		i++;
	}
	if (delim[0] == ':' && size == 1)
	{
		shell->exit_status = 0;
		return (ERROR);
	}
	if (delim[0] == '.' && size == 1)
	{
		shell->exit_status = 2;
		printf_fd(STDERR, "minishell: filename argument required\n");
		printf_fd(STDERR, ".: usage: . filename [arguments]\n");
		return (ERROR);
	}
	// if (!ft_isalnum(delim[0]) && valid == 1 && size <= 2)
	// {
	// 	printf_fd(STDERR, " syntax error near unexpected token `newline'\n");
	// 	return (ERROR);
	// }
	if (valid == 1)
		return (SUCCESS);
	else
	{
		shell->exit_status = 2;
		return (ERROR);
	}
}
