/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:21:45 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/25 17:25:46 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	go_to_end(char *input, t_exp *exp)
{
	if (input[exp->end] >= '0' && input[exp->end] <= '9')
		exp->end++;
	else
	{
		while ((ft_isalnum(input[exp->end]) || input[exp->end] == '_'
				|| input[exp->end] == '?') && input[exp->end])
		{
			if (input[exp->end] == '?')
			{
				exp->end++;
				break ;
			}
			exp->end++;
		}
	}
	exp->start = exp->end;
}

static void	join_and_free(t_exp *exp, t_sh *shell)
{
	exp->tmp = ft_fstrjoin(&exp->result, &exp->buffer, 3);
	if (!exp->tmp)
		cleanup_exit(shell, NULL);
	exp->result = exp->tmp;
}

void	expand_buffer(t_exp *exp, char *input, t_sh *shell)
{
	if (input[exp->end + 1] == '?')
	{
		if (expand_xcode(&exp->buffer, shell))
			cleanup_exit(shell, NULL);
	}
	else
		expand_var(input + exp->end, &exp->buffer, shell);
	if (exp->buffer)
		join_and_free(exp, shell);
}

char	*expand_token(char *input, t_sh *shell)
{
	t_exp	exp;

	if (init_exp(&exp))
		cleanup_exit(shell, NULL);
	while (input[exp.end] != '\0')
	{
		if (input[exp.end] == '\'')
			handle_single_quote(&exp, input);
		else if (input[exp.end] == '"')
			handle_double_quote(&exp, input, shell);
		else if (input[exp.end] == '$')
			handle_dollar(&exp, input, shell);
		else
			exp.end++;
	}
	exp.buffer = ft_substr(input, exp.start, exp.end - exp.start);
	if (!exp.buffer)
		cleanup_exit(shell, NULL);
	if (exp.tmp)
		exp.result = exp.tmp;
	exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
	if (!exp.tmp)
		cleanup_exit(shell, NULL);
	free(exp.result);
	return (free(exp.buffer), exp.tmp);
}
