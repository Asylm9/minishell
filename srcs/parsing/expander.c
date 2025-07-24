/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/24 17:57:43 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*trim_quotes(char *input, t_sh *shell)
{
	t_exp	exp;

	if (init_exp(&exp))
		cleanup_exit(shell, NULL);
	while (input[exp.end] && input[exp.end] != '\0')
	{
		match_quotes(input, &exp.end, &exp.quote);
		exp.tmp = ft_substr(input, exp.start, exp.end - exp.start);
		if (!exp.tmp)
			cleanup_exit(shell, NULL);
		exp.buffer = ft_strdup(exp.result);
		if (!exp.buffer)
			cleanup_exit(shell, NULL);
		free(exp.result);
		exp.result = ft_fstrjoin(&exp.buffer, &exp.tmp, 3);
		if (!exp.result)
			cleanup_exit(shell, NULL);
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
	if (tok_lst->type == PIPE)
		return (input_error(1, shell, PIPE), ERROR);
	return (process_lst(tok_lst, exp_lst, shell));
}
