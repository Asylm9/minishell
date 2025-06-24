/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/24 20:05:15 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_var(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	expand_var(char *input, char **result)
{
	int		i;
	char	*var;

	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	var = ft_substr(input, 0, i);
	(*result) = getenv(var + 1);
	free(var);
	if ((*result) == NULL)
		return (1);
	return (0);
}

int	expand_xcode(char **result, t_sh *shell)
{
	*result = ft_itoa(shell->exit_status);
	if (!(*result))
		return (1);
	return (0);
}

char	*expand_token(char *input, t_sh *shell)
{
	char	*result;
	char	*buffer;
	char	*tmp;
	int		pos;
	int		start;

	result = ft_calloc(1, 1);
	buffer = NULL;
	tmp = NULL;
	pos = 0;
	start = 0;
	while (input[pos] != '\0')
	{
		if (input[pos] == '$')
		{
			buffer = ft_substr(input, start, pos - start);
			tmp = ft_fstrjoin(&result, &buffer, 0);
			if (result)
			{
				free(result);
				result = NULL;
			}
			if (buffer)
			{
				free(buffer);
				buffer = NULL;
			}
			if (input[pos + 1] == '?')
			{
				if (expand_xcode(&buffer, shell))
					result = tmp;
				if (buffer)
				{
					result = ft_fstrjoin(&tmp, &buffer, 1);
					free(buffer);
					tmp = NULL;
				}
			}
			else if (expand_var(input + pos, &buffer))
				result = tmp;
			else
			{
				if (buffer)
				{
					result = ft_fstrjoin(&tmp, &buffer, 1);
					tmp = NULL;
				}
				else
				{
					if (result)
						free(result);
					result = tmp;
				}
			}
			pos++;
			while ((ft_isalnum(input[pos]) || input[pos] == '_'
					|| input[pos] == '?') && input[pos])
			{
				pos++;
				if (input[pos - 1] == '?')
					break ;
			}
			start = pos;
		}
		else
			pos++;
	}
	buffer = ft_substr(input, start, pos - start);
	tmp = ft_fstrjoin(&result, &buffer, 0);
	free(result);
	free(buffer);
	return (tmp);
}

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
