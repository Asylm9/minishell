/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:33:10 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/23 16:22:54 by agaland          ###   ########.fr       */
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

int	check_validity(t_lst *exp_lst, t_sh *shell)
{
	t_token_type	first;
	t_token_type	second;

	first = NOT_SET;
	while (exp_lst)
	{
		second = first;
		first = exp_lst->type;
		if (first == PIPE && (second >= 2 || (exp_lst->next
					&& (exp_lst->next->type != 1 && exp_lst->next->type != 4))
				|| !exp_lst->next))
			return (input_error(1, shell, first), ERROR);
		if ((second >= 3 && first != WORD) || (first >= 3 && second >= 3))
			return (input_error(2, shell, first), ERROR);
		if (first >= 3 && !exp_lst->next)
			return (input_error(3, shell, first), ERROR);
		exp_lst = exp_lst->next;
	}
	return (SUCCESS);
}
