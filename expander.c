/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/24 19:13:18 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_quotes(char *input)
{
	char	*result;
	char	*tmp;
	char	*buffer;
	int		start;
	int		end;

	start = 0;
	end = 0;
	result = NULL;
	tmp = NULL;
	buffer = NULL;
	while (input[end] && input[end] != '\0')
	{
		while (input[end] != '\'' && input[end] != '"' && input[end])
			end++;
		tmp = ft_substr(input, start, end - start);
		if (result)
		{
			buffer = result;
			result = ft_fstrjoin(&buffer, &tmp, 3);
		}
		else
			result = tmp; // first chunk, no join needed
		if (input[end] == '\0')
			break ;
		start = end + 1;
		end++;
	}
	return (result);
}

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

int	expand_list(t_token *tok_lst, t_token *exp_lst, t_sh *shell)
{
	if (!exp_lst)
		return (ERROR);
	while (tok_lst)
	{
		exp_lst->expand = NO_EXPAND;
		if (tok_lst->type == REDIR_HEREDOC)
		{
			if (tok_lst->next->value)
			{
				exp_lst->hd_fd = handle_heredoc(tok_lst->next->value, shell);
			}
		}
		if (tok_lst->expand == NO_EXPAND)
		{
			if (!is_pipe_redir(tok_lst->value))
			{
				exp_lst->value = trim_quotes(ft_strdup(tok_lst->value));
				if (!exp_lst->value)
				{
					free(exp_lst);
					return (ERROR);
				}
			}
		}
		else
			exp_lst->value = trim_quotes(expand_token(tok_lst->value, shell));
		exp_lst->type = tok_lst->type;
		tok_lst = tok_lst->next;
		if (tok_lst != NULL)
		{
			create_token_node(&exp_lst);
			exp_lst = exp_lst->next;
		}
		else
		{
			exp_lst->next = NULL;
			break ;
		}
	}
	return (SUCCESS);
}
