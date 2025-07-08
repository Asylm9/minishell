/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/07 20:46:08 by magoosse         ###   ########.fr       */
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

int	expand_var(char *input, char **result, t_env *envl)
{
	int		i;
	char	*var;

	i = 1;
	if (input[i] >= '0' && input[i] <= '9')
		var = ft_substr(input, 1, 1);
	else
	{
		while (ft_isalnum(input[i]) || input[i] == '_')
			i++;
		var = ft_substr(input, 1, i - 1);
	}
	(*result) = get_envl_var(var, envl);
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
		if (input[pos] == '\'')
		{
			pos++;
			while (input[pos] && input[pos] != '\'')
				pos++;
			pos++;
		}
		if (input[pos] == '"')
		{
			pos++;
			while (input[pos] && input[pos] != '"')
			{
				if (input[pos] == '$')
				{
					if (input[pos + 1] && (ft_isalnum(input[pos + 1])
							|| input[pos + 1] == '_' || input[pos + 1] == '?'))
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
								result = ft_fstrjoin(&tmp, &buffer, 3);
								tmp = NULL;
							}
						}
						else if (expand_var(input + pos, &buffer, shell->envl))
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
						if (input[pos] >= '0' && input[pos] <= '9')
							pos++;
						else
							while ((ft_isalnum(input[pos]) || input[pos] == '_'
									|| input[pos] == '?') && input[pos])
							{
								pos++;
								if (input[pos - 1] == '?')
									break ;
							}
						start = pos;
					}
					else if (input[pos + 1] && input[pos + 1] != ':'
						&& input[pos + 1] != '=' && input[pos + 1] != '"'
						&& input[pos + 1] != ' ')
					{
						pos++;
						buffer = ft_strdup("");
						tmp = ft_fstrjoin(&result, &buffer, 0);
					}
					else
					{
						pos++;
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
					}
					start = pos;
				}
				else
					pos++;
			}
			pos++;
		}
		else if (input[pos] == '$')
		{
			if (input[pos + 1] && (ft_isalnum(input[pos + 1]) || input[pos
					+ 1] == '_' || input[pos + 1] == '?'))
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
						result = ft_fstrjoin(&tmp, &buffer, 3);
						tmp = result;
					}
				}
				else if (expand_var(input + pos, &buffer, shell->envl))
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
				if (input[pos] >= '0' && input[pos] <= '9')
					pos++;
				else
					while ((ft_isalnum(input[pos]) || input[pos] == '_'
							|| input[pos] == '?') && input[pos])
					{
						pos++;
						if (input[pos - 1] == '?')
							break ;
					}
				start = pos;
			}
			else if (input[pos + 1] && input[pos + 1] != ':' && input[pos
				+ 1] != '=' && input[pos + 1] != ' ')
			{
				pos++;
				buffer = ft_strdup("");
				tmp = ft_fstrjoin(&result, &buffer, 0);
			}
			else
			{
				pos++;
				result = tmp;
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
			}
			start = pos;
		}
		else
			pos++;
	}
	buffer = ft_substr(input, start, pos - start);
	if (tmp)
		result = tmp;
	tmp = ft_fstrjoin(&result, &buffer, 0);
	free(result);
	free(buffer);
	return (tmp);
}

void	match_quotes(char *input, int *end, char *quote)
{
	if (*quote == '\'' || *quote == '"')
	{
		while (input[(*end)] && input[(*end)] != *quote)
			(*end)++;
		*quote = ' ';
	}
	else
	{
		while (input[(*end)] != '\'' && input[(*end)] != '"' && input[(*end)])
			(*end)++;
		*quote = input[(*end)];
	}
}

char	*trim_quotes(char *input)
{
	char	*result;
	char	*tmp;
	char	*buffer;
	int		start;
	int		end;
	char	quote;

	start = 0;
	end = 0;
	result = NULL;
	tmp = NULL;
	buffer = NULL;
	quote = ' ';
	while (input[end] && input[end] != '\0')
	{
		match_quotes(input, &end, &quote);
		tmp = ft_substr(input, start, end - start);
		if (result)
		{
			buffer = ft_strdup(result);
			free(result);
			result = ft_fstrjoin(&buffer, &tmp, 3);
		}
		else
			result = tmp; // first chunk, no join needed
		if (input[end] == '\0')
			break ;
		end++;
		start = end;
	}
	free(input);
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

/*
	0 NOT_SET
	1 WORD,
	2 PIPE,
	3 REDIR_IN,
	4 REDIR_OUT,
	5 REDIR_APPEND,
	6 REDIR_HEREDOC,
	7 CMD,
*/

static int	check_validity(t_token *exp_lst, t_sh *shell)
{
	t_token_type	first;
	t_token_type	second;

	first = NOT_SET;
	second = NOT_SET;
	while (exp_lst)
	{
		second = first;
		first = exp_lst->type;
		if (first == PIPE && (second >= 2 || (exp_lst->next
					&& (exp_lst->next->type != 1 && exp_lst->next->type != 4))
				|| !exp_lst->next))
		{
			printf_fd(STDERR, " syntax error near unexpected token `|'\n");
			shell->exit_status = 2;
			return (ERROR);
		}
		// else
		// 	return (SUCCESS);
		if ((second >= 3 && first != WORD) || (first >= 3 && second >= 3))
			return (ERROR);
		// if (first == 6 && (!exp_lst->next || exp_lst->next->type != WORD))
		// 	return (ERROR);
		exp_lst = exp_lst->next;
	}
	return (SUCCESS);
}

size_t	count_nb_words(char const *s, char c)
{
	size_t	i;
	size_t	count;
	char	quote;

	i = 0;
	count = 0;
	if (!s)
		return (0);
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

int	expand_list(t_token *tok_lst, t_token *exp_lst, t_sh *shell)
{
	int		advance;
	int		count;
	int		i;
	char	*input;
	char	**splitted;
	t_token	*new_line;

	count = 0;
	i = 0;
	new_line = NULL;
	if (!exp_lst)
		return (ERROR);
	if (tok_lst->type == PIPE)
	{
		shell->exit_status = 2;
		printf_fd(STDERR,
			"minishell: syntax error near unexpected token `|'\n");
		return (ERROR);
	}
	while (tok_lst)
	{
		if (check_validity(tok_lst, shell) == SUCCESS)
		{
			advance = 1;
			exp_lst->expand = NO_EXPAND;
			if (tok_lst->type == REDIR_HEREDOC)
				if (tok_lst->next->value)
				{
					exp_lst->hd_fd = handle_heredoc(tok_lst->next->value,
							shell);
					if (exp_lst->hd_fd == 130)
						return (ERROR);
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
				exp_lst->type = tok_lst->type;
			}
			else if ((ft_strlen(expand_token(tok_lst->value, shell))) != 0)
			{
				count = count_nb_words(expand_token(tok_lst->value, shell),
						' ');
				if (count > 1)
				{
					splitted = ft_split(trim_quotes(expand_token(tok_lst->value,
									shell)), ' ');
					while (count != 0)
					{
						exp_lst->value = splitted[i];
						exp_lst->type = WORD;
						create_token_node(&exp_lst);
						exp_lst = exp_lst->next;
						i++;
						count--;
					}
				}
				else
				{
					exp_lst->value = trim_quotes(expand_token(tok_lst->value,
								shell));
					exp_lst->type = tok_lst->type;
				}
			}
			else
				advance = 0;
			tok_lst = tok_lst->next;
			if (tok_lst != NULL && advance)
			{
				create_token_node(&exp_lst);
				exp_lst = exp_lst->next;
			}
			else if (advance)
			{
				exp_lst->next = NULL;
				break ;
			}
		}
		else if (tok_lst->type == PIPE)
		{
			if (!tok_lst->next)
			{
				exp_lst->type = PIPE;
				create_token_node(&exp_lst);
				exp_lst = exp_lst->next;
				input = readline(">");
				create_token_node(&new_line);
				tokenize_input(new_line, input);
				expand_list(new_line, exp_lst, shell);
				tok_lst = tok_lst->next;
			}
		}
		else
			return (ERROR);
	}
	return (SUCCESS);
}
