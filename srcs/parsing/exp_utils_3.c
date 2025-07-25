/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:21:31 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/25 17:25:04 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_unclosed_pipes(t_lst *tok_lst, t_lst *exp_lst, t_sh *shell)
{
	char	*input;

	(void)exp_lst;
	(void)shell;
	input = NULL;
	input = readline(">");
	create_list_node(&tok_lst, shell);
	tok_lst = tok_lst->next;
	if (tokenize_input(tok_lst, input, shell) == ERROR)
		cleanup_exit(shell, NULL);
	if (input != NULL)
		free(input);
}

void	handle_dollar(t_exp *exp, char *input, t_sh *shell)
{
	if (input[exp->end + 1] && (ft_isalnum(input[exp->end + 1])
			|| input[exp->end + 1] == '_' || input[exp->end + 1] == '?'))
	{
		exp->buffer = ft_substr(input, exp->start, exp->end - exp->start);
		if (!exp->buffer)
			cleanup_exit(shell, NULL);
		exp->tmp = ft_fstrjoin(&exp->result, &exp->buffer, 3);
		if (!exp->tmp)
			cleanup_exit(shell, NULL);
		exp->result = exp->tmp;
		expand_buffer(exp, input, shell);
		exp->end++;
		go_to_end(input, exp);
	}
	else if (input[exp->end + 1] == '\'' || input[exp->end + 1] == '"')
	{
		exp->end++;
		exp->start = exp->end;
	}
	else
		exp->end++;
}

void	handle_single_quote(t_exp *exp, char *input)
{
	exp->end++;
	while (input[exp->end] && input[exp->end] != '\'')
		exp->end++;
	exp->end++;
}

void	handle_double_quote(t_exp *exp, char *input, t_sh *shell)
{
	exp->end++;
	while (input[exp->end] && input[exp->end] != '"')
	{
		if (input[exp->end] == '$' && input[exp->end + 1] != '"')
			handle_dollar(exp, input, shell);
		else
			exp->end++;
	}
	exp->end++;
}
