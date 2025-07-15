/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/15 18:10:04 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*trim_quotes(char *input)
{
	t_exp	exp;

	init_exp(&exp);
	while (input[exp.end] && input[exp.end] != '\0')
	{
		match_quotes(input, &exp.end, &exp.quote);
		exp.tmp = ft_substr(input, exp.start, exp.end - exp.start);
		if (exp.result)
		{
			exp.buffer = ft_strdup(exp.result);
			free(exp.result);
			exp.result = ft_fstrjoin(&exp.buffer, &exp.tmp, 3);
		}
		else
			exp.result = exp.tmp;
		if (input[exp.end] != '\0')
			exp.end++;
		exp.start = exp.end;
	}
	return (exp.result);
}

size_t	count_nb_words(char const *s, char c)
{
	size_t	i;
	size_t	count;
	char	quote;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i++];
			while (s[i] != quote)
				i++;
			i++;
		}
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

int	expand_list(t_lst *tok_lst, t_lst *exp_lst, t_sh *shell)
{
	int	advance;

	advance = 0;
	if (!exp_lst)
		return (ERROR);
	if (tok_lst->type == PIPE)
		return (input_error(1, shell, PIPE), ERROR);
	return (process_lst(tok_lst, exp_lst, shell, advance));
}
