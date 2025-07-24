/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:05:28 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/24 19:29:25 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	find_end_of_token(const char *input, int *end)
{
	char	quote;

	while (input[(*end)] && input[(*end)] != ' ' && input[(*end)] != '|'
		&& input[(*end)] != '<' && input[(*end)] != '>')
	{
		if (input[(*end)] == '"' || input[(*end)] == '\'')
		{
			quote = input[(*end)];
			(*end)++;
			while (input[(*end)] != quote)
				(*end)++;
			(*end)++;
		}
		else
			(*end)++;
	}
	return (SUCCESS);
}

void	set_token_type(t_lst *tok_lst, const char *input, int *start, int *end)
{
	if (input[(*start)] == '|')
	{
		tok_lst->type = PIPE;
		(*end)++;
	}
	else if (input[(*start)] == '<')
	{
		tok_lst->type = REDIR_IN;
		(*end)++;
		if (input[(*end)] == '<')
			tok_lst->type = REDIR_HEREDOC;
	}
	else if (input[(*start)] == '>')
	{
		tok_lst->type = REDIR_OUT;
		(*end)++;
		if (input[(*end)] == '>')
			tok_lst->type = REDIR_APPEND;
	}
	else if (!(input[(*end)] == '\'' || !is_env_var(tok_lst->value)))
		tok_lst->expand = EXPAND;
	if (tok_lst->type == 5 || tok_lst->type == 6)
		(*end)++;
}

int	set_value(t_lst *tok_lst, const char *input, int *start, int *end)
{
	tok_lst->value = ft_substr(input, (*start), (*end) - (*start));
	if (!tok_lst->value)
		return (ERROR);
	return (SUCCESS);
}

int	process_token(t_lst *tok_lst, const char *input, int *start, int *end)
{
	skip_spaces(input, start);
	(*end) = *start;
	find_end_of_token(input, end);
	if (set_value(tok_lst, input, start, end) == ERROR)
		return (ERROR);
	set_token_type(tok_lst, input, start, end);
	return (SUCCESS);
}

int	tokenize_input(t_lst *tok_lst, const char *input, t_sh *shell)
{
	int	start;
	int	end;
	int	i;

	start = 0;
	while (input[start])
	{
		if (process_token(tok_lst, input, &start, &end) == ERROR)
			return (ERROR);
		i = end;
		while (input[i] && input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		create_list_node(&tok_lst, shell);
		if (!tok_lst->next)
			return (ERROR);
		if (input[end] == ' ')
			start = end + 1;
		else
			start = end;
		tok_lst = tok_lst->next;
	}
	tok_lst->next = NULL;
	return (SUCCESS);
}
