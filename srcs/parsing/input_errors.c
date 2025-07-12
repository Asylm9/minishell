/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:35:42 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/12 21:56:22 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_quotes(const char *input, int *i, int *size, t_sh *shell)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		if (ft_isalnum(input[*i]) || input[*i] == '$')
			(*size)++;
		(*i)++;
	}
	if (input[*i] == '\0')
	{
		printf_fd(STDERR,
			"minishell: unexpected EOF while looking for matching `%c'\n",
			quote);
		shell->exit_status = 2;
		return (ERROR);
	}
	return (SUCCESS);
}

int	check_ampersand(const char *input, int i, t_sh *shell)
{
	if (next_input((char *)&input[i + 1]) == '&')
	{
		printf_fd(STDERR, " syntax error near unexpected token `&&'\n");
		shell->exit_status = 2;
		return (ERROR);
	}
	printf_fd(STDERR, " invalid caracter : '&'\n");
	shell->exit_status = 2;
	return (ERROR);
}

int	check_special_delim(char delim, int size, t_sh *shell)
{
	if (delim == ':' && size == 1)
	{
		shell->exit_status = 0;
		return (ERROR);
	}
	if (delim == '.' && size == 1)
	{
		shell->exit_status = 2;
		printf_fd(STDERR, "minishell: filename argument required\n");
		printf_fd(STDERR, ".: usage: . filename [arguments]\n");
		return (ERROR);
	}
	return (SUCCESS);
}