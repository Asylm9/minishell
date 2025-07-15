/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:09:51 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/15 16:56:07 by magoosse         ###   ########.fr       */
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
	int		advance;
	int		count;
	int		i;
	char	*input;
	char	*expanded;
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
			expanded = expand_token(tok_lst->value, shell);
			if (tok_lst->expand == NO_EXPAND)
			{
				if (!is_pipe_redir(tok_lst->value))
				{
					exp_lst->value = trim_quotes(expanded);
					if (!exp_lst->value)
					{
						free(exp_lst);
						return (ERROR);
					}
				}
				exp_lst->type = tok_lst->type;
			}
			else if ((ft_strlen(expanded)) != 0)
			{
				count = count_nb_words(expanded, ' ');
				if (count > 1)
				{
					splitted = ft_split(trim_quotes(expanded), ' ');
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
					exp_lst->value = trim_quotes(expanded);
					exp_lst->type = tok_lst->type;
				}
			}
			else
				advance = 0;
			free(expanded);
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
