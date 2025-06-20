/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/20 13:33:04 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_var(char *input, char **result, t_sh *shell)
{
	int		i;
	char	*var;

	i = 1;
	if (input[i] == '?')
	{
		*result = ft_itoa(shell->exit_status);
		if (!(*result))
			return (ERROR);
		;
	}
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	var = ft_substr(input, 0, i);
	(*result) = getenv(var + 1);
	free(var);
	if ((*result) == NULL)
		return (ERROR);
	return (SUCCESS);
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
			if (expand_var(input + pos, &buffer, shell))
				result = ft_strdup("");
			if (buffer)
			{
				result = ft_fstrjoin(&tmp, &buffer, 1);
				tmp = NULL;
				// Do NOT free buffer if it comes from getenv!
			}
			else
			{
				if (result)
				{
					free(result);
					result = NULL;
				}
				result = tmp;
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
		if (str[i] != '|' && str[i] != '>' && str[i] != '<' || i > 1)
			return (SUCCESS);
		i++;
	}
	return (ERROR);
}

int	expand_list(t_token *tok_lst, char **env, t_token *exp_lst, t_sh *shell)
{
	t_token	*head;
	char	*expanded_value;
	int		i;

	i = 0;
	if (!exp_lst)
		return (1);
	head = exp_lst;
	while (tok_lst)
	{
		exp_lst->expand = NO_EXPAND;
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

// int	expand_size(char *value, char **env)
// {
// 	int		i;
// 	int		len;
// 	int		env_prev_size;
// 	int		env_expd_size;
// 	char	*cur_env;

// 	i = 0;
// 	len = 0;
// 	env_prev_size = 0;
// 	env_expd_size = 0;
// 	while (value[len])
// 	{
// 		if (value[len] == '$')
// 		{
// 			i = 0;
// 			while (value[len + i] && value[len + i] != ' ')
// 				i++;
// 			cur_env = ft_substr(value, len + 1, i - 1);
// 			env_prev_size += i;
// 			if (getenv(cur_env) == NULL)
// 				env_expd_size -= i;
// 			else
// 				env_expd_size += ft_strlen(ft_strdup(getenv(cur_env)));
// 		}
// 		if (i != 0)
// 			len += i;
// 		else
// 			len++;
// 	}
// 	printf("env prev : %d\n", env_prev_size);
// 	printf("env expd : %d\n", env_expd_size);
// 	env_expd_size = len - env_prev_size + env_expd_size + 1;
// 	return (env_expd_size);
// }