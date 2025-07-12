/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/12 22:54:21 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void	init_exp(t_exp *exp)
{
	exp->result = ft_calloc(1, 1);
	exp->buffer = NULL;
	exp->tmp = NULL;
	exp->end = 0;
	exp->start = 0;
	exp->quote = ' ';
}

static void	handle_single_quote(t_exp *exp, char *input)
{
	exp->end++;
	while (input[exp->end] && input[exp->end] != '\'')
		exp->end++;
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
		if (input[exp.end] == '"')
		{
			exp.end++;
			while (input[exp.end] && input[exp.end] != '"')
			{
				if (input[exp.end] == '$')
				{
					if (input[exp.end + 1] && (ft_isalnum(input[exp.end + 1])
							|| input[exp.end + 1] == '_' || input[exp.end
							+ 1] == '?'))
					{
						exp.buffer = ft_substr(input, exp.start, exp.end
								- exp.start);
						exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
						if (exp.result)
						{
							free(exp.result);
							exp.result = NULL;
						}
						if (exp.buffer)
						{
							free(exp.buffer);
							exp.buffer = NULL;
						}
						if (input[exp.end + 1] == '?')
						{
							if (expand_xcode(&exp.buffer, shell))
								exp.result = exp.tmp;
							if (exp.buffer)
							{
								exp.result = ft_fstrjoin(&exp.tmp, &exp.buffer,
										3);
								exp.tmp = NULL;
							}
						}
						else if (expand_var(input + exp.end, &exp.buffer,
								shell->envl))
							exp.result = exp.tmp;
						else
						{
							if (exp.buffer)
							{
								exp.result = ft_fstrjoin(&exp.tmp, &exp.buffer,
										1);
								exp.tmp = NULL;
							}
							else
							{
								if (exp.result)
									free(exp.result);
								exp.result = exp.tmp;
							}
						}
						exp.end++;
						if (input[exp.end] >= '0' && input[exp.end] <= '9')
							exp.end++;
						else
							while ((ft_isalnum(input[exp.end])
									|| input[exp.end] == '_'
									|| input[exp.end] == '?') && input[exp.end])
							{
								exp.end++;
								if (input[exp.end - 1] == '?')
									break ;
							}
						exp.start = exp.end;
					}
					else if (input[exp.end + 1] && input[exp.end + 1] != ':'
						&& input[exp.end + 1] != '=' && input[exp.end
						+ 1] != '"' && input[exp.end + 1] != ' ')
					{
						exp.end++;
						exp.buffer = ft_strdup("");
						exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
					}
					else
					{
						exp.end++;
						exp.buffer = ft_substr(input, exp.start, exp.end
								- exp.start);
						exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
						if (exp.result)
						{
							free(exp.result);
							exp.result = NULL;
						}
						if (exp.buffer)
						{
							free(exp.buffer);
							exp.buffer = NULL;
						}
					}
					exp.start = exp.end;
				}
				else
					exp.end++;
			}
			exp.end++;
		}
		else if (input[exp.end] == '$')
		{
			if (input[exp.end + 1] && (ft_isalnum(input[exp.end + 1])
					|| input[exp.end + 1] == '_' || input[exp.end + 1] == '?'))
			{
				exp.buffer = ft_substr(input, exp.start, exp.end - exp.start);
				exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
				if (exp.result)
				{
					free(exp.result);
					exp.result = NULL;
				}
				if (exp.buffer)
				{
					free(exp.buffer);
					exp.buffer = NULL;
				}
				if (input[exp.end + 1] == '?')
				{
					if (expand_xcode(&exp.buffer, shell))
						exp.result = exp.tmp;
					if (exp.buffer)
					{
						exp.result = ft_fstrjoin(&exp.tmp, &exp.buffer, 3);
						exp.tmp = exp.result;
					}
				}
				else if (expand_var(input + exp.end, &exp.buffer, shell->envl))
					exp.result = exp.tmp;
				else
				{
					if (exp.buffer)
					{
						exp.result = ft_fstrjoin(&exp.tmp, &exp.buffer, 1);
						exp.tmp = NULL;
					}
					else
					{
						if (exp.result)
							free(exp.result);
						exp.result = exp.tmp;
					}
				}
				exp.end++;
				if (input[exp.end] >= '0' && input[exp.end] <= '9')
					exp.end++;
				else
					while ((ft_isalnum(input[exp.end]) || input[exp.end] == '_'
							|| input[exp.end] == '?') && input[exp.end])
					{
						exp.end++;
						if (input[exp.end - 1] == '?')
							break ;
					}
				exp.start = exp.end;
			}
			else if (input[exp.end + 1] && input[exp.end + 1] != ':'
				&& input[exp.end + 1] != '=' && input[exp.end + 1] != ' ')
			{
				exp.end++;
				exp.buffer = ft_strdup("");
				exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
			}
			else
			{
				exp.end++;
				exp.result = exp.tmp;
				exp.buffer = ft_substr(input, exp.start, exp.end - exp.start);
				exp.tmp = ft_fstrjoin(&exp.result, &exp.buffer, 0);
				if (exp.result)
				{
					free(exp.result);
					exp.result = NULL;
				}
				if (exp.buffer)
				{
					free(exp.buffer);
					exp.buffer = NULL;
				}
			}
			exp.start = exp.end;
		}
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
			exp.result = exp.tmp; // first chunk, no join needed
		if (input[exp.end] != '\0')
			exp.end++;
		exp.start = exp.end;
	}
	free(input);
	return (exp.result);
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

char	*token(t_token_type token)
{
	if (token == 3)
		return ("<");
	if (token == 4)
		return (">");
	if (token == 5)
		return (">>");
	if (token == 6)
		return ("<<");
	return ("newline");
}

static int	check_validity(t_lst *exp_lst, t_sh *shell)
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
		if ((second >= 3 && first != WORD) || (first >= 3 && second >= 3))
		{
			printf_fd(STDERR, " syntax error near unexpected token `%s'\n",
				token(first));
			shell->exit_status = 2;
			return (ERROR);
		}
		if (first >= 3 && !exp_lst->next)
		{
			printf_fd(STDERR,
				" syntax error near unexpected token `newline'\n");
			shell->exit_status = 2;
			return (ERROR);
		}
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
	int		advance;
	int		count;
	int		i;
	char	*input;
	char	**splitted;
	t_lst	*new_line;

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
			{
				if (tok_lst->next->value)
				{
					exp_lst->hd_fd = handle_heredoc(tok_lst->next->value,
							shell);
					if (exp_lst->hd_fd == 130)
						return (ERROR);
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
						create_list_node(&exp_lst);
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
				create_list_node(&exp_lst);
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
				create_list_node(&exp_lst);
				exp_lst = exp_lst->next;
				input = readline(">");
				create_list_node(&new_line);
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
