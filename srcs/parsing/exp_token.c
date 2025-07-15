/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:21:45 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/15 16:24:58 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	go_to_end(char *input, t_exp *exp)
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

static void	handle_dollar(t_exp *exp, char *input, t_sh *shell)
{
	if (input[exp->end + 1] && (ft_isalnum(input[exp->end + 1])
			|| input[exp->end + 1] == '_' || input[exp->end + 1] == '?'))
	{
		exp->buffer = ft_substr(input, exp->start, exp->end - exp->start);
		exp->tmp = ft_fstrjoin(&exp->result, &exp->buffer, 3);
		exp->result = exp->tmp;
		if (input[exp->end + 1] == '?')
			expand_xcode(&exp->buffer, shell);
		else
			expand_var(input + exp->end, &exp->buffer, shell->envl);
		if (exp->buffer)
		{
			exp->tmp = ft_fstrjoin(&exp->result, &exp->buffer, 3);
			exp->result = exp->tmp;
		}
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

static void	handle_single_quote(t_exp *exp, char *input)
{
	exp->end++;
	while (input[exp->end] && input[exp->end] != '\'')
		exp->end++;
	exp->end++;
}

static void	handle_double_quote(t_exp *exp, char *input, t_sh *shell)
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

char	*expand_token(char *input, t_sh *shell)
{
	t_exp	exp;

	init_exp(&exp);
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
	if (exp.tmp)
		exp.result = exp.tmp;
	exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
	free(exp.result);
	free(exp.buffer);
	return (exp.tmp);
}
