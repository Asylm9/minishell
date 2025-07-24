/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:33:10 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/24 16:38:49 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_pipe_redir(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if ((str[i] != '|' && str[i] != '>' && str[i] != '<') || i > 1)
			return (SUCCESS);
		i++;
	}
	return (ERROR);
}

char	*token(t_token_type token)
{
	if (token == 3)
		return ("<");
	if (token == 4)
		return (">");
	if (token == 5)
		return (">>");
	if (token == 6)
		return ("<<");
	return ("newline");
}

void	input_error(int error, t_sh *shell, t_token_type tok)
{
	if (error == 1)
	{
		printf_fd(STDERR, " syntax error near unexpected token `|'\n");
		shell->exit_status = 2;
	}
	if (error == 2)
	{
		printf_fd(STDERR, " syntax error near unexpected token `%s'\n",
			token(tok));
		shell->exit_status = 2;
	}
	if (error == 3)
	{
		printf_fd(STDERR, " syntax error near unexpected token `newline'\n");
		shell->exit_status = 2;
	}
}

int	check_validity(t_lst *tok_lst, t_sh *shell)
{
	t_token_type	first;
	t_token_type	second;

	first = NOT_SET;
	while (tok_lst)
	{
		second = first;
		first = tok_lst->type;
		if (first == PIPE && (second >= 2 || (tok_lst->next
					&& (tok_lst->next->type != 1 && tok_lst->next->type != 4))))
			return (input_error(1, shell, first), ERROR);
		if ((second >= 3 && first != WORD) || (first >= 3 && second >= 3))
			return (input_error(2, shell, first), ERROR);
		if (first >= 3 && !tok_lst->next)
			return (input_error(3, shell, first), ERROR);
		tok_lst = tok_lst->next;
	}
	return (SUCCESS);
}
