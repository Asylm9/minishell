/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/17 15:30:19 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_var(char *input, char **result)
{
	int		i;
	char	*var;

	// char	*result;
	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	var = ft_substr(input, 0, i);
	(*result) = getenv(var + 1);
	free(var);
	if (result == NULL)
		return (1);
	return (0);
}

char	*expand_token(char *input)
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
			tmp = ft_strjoin(&result, &buffer, 0);
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
			if (expand_var(input + pos, &buffer))
			{
				result = ft_strdup("");
				free(tmp);
				return (result); // Error handling if variable expansion fails
			}
			if (buffer)
			{
				result = ft_strjoin(&tmp, &buffer, 0);
				free(tmp);
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
			while ((ft_isalnum(input[pos]) || input[pos] == '_') && input[pos])
				pos++;
			start = pos;
		}
		else
			pos++;
	}
	buffer = ft_substr(input, start, pos - start);
	tmp = ft_strjoin(&result, &buffer, 0);
	free(result);
	free(buffer);
	result = tmp;
	return (result);
}

// char	*trim_quotes(char *input)
// {
// 	char	*result;
// 	char	*tmp;
// 	char	*buffer;
// 	int		start;
// 	int		end;

// 	start = 0;
// 	end = 0;
// 	tmp = NULL;
// 	result = NULL;
// 	buffer = NULL;
// 	while (input[end] && input[end] != '\0')
// 	{
// 		while (input[end] != '\'' && input[end] != '"' && input[end])
// 			end++;
// 		tmp = ft_substr(input, start, end - start);
// 		if (result)
// 		{
// 			buffer = ft_strdup(result);
// 			free(result);
// 			result = NULL;
// 		}
// 		result = ft_strjoin(&buffer, &tmp, 3);
// 		start = end + 1;
// 		end++;
// 	}
// 	return (result);
// }

int	expand_list(t_token *tok_lst, char **env, t_token *exp_lst)
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
		printf("Expanding token: %s\n", tok_lst->value);
		exp_lst->expand = NO_EXPAND;
		if (tok_lst->expand == NO_EXPAND)
		{
			exp_lst->value = ft_strdup(tok_lst->value);
			if (!exp_lst->value)
			{
				free(exp_lst);
				return (1);
			}
		}
		else
			exp_lst->value = trim_quotes(expand_token(tok_lst->value));
		printf("test\n");
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
	return (0);
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