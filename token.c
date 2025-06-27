/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:05:28 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/27 03:28:59 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_token_list(t_token **tok_lst)
{
	(*tok_lst) = malloc(sizeof(t_token));
	if (!(*tok_lst))
	{
		perror("Token list malloc failed.\n");
		return (ERROR);
	}
	(*tok_lst)->value = NULL;
	(*tok_lst)->expand = NO_EXPAND;
	(*tok_lst)->type = WORD;
	(*tok_lst)->next = NULL;
	return (SUCCESS);
}

int	create_token_node(t_token **tok_lst)
{
	t_token	*new_token;
	t_token	*temp;

	temp = *tok_lst;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("Token list malloc failed.\n");
		return (ERROR);
	}
	new_token->value = NULL;
	new_token->expand = NO_EXPAND;
	new_token->type = WORD;
	new_token->next = NULL;
	new_token->hd_fd = -1;
	if (*tok_lst == NULL)
		*tok_lst = new_token;
	else
	{
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
	return (SUCCESS);
}

int	skip_spaces(const char *input, int *pos)
{
	while (input[*pos] && input[*pos] == ' ')
		(*pos)++;
	if (input[*pos] == '\0')
		return (ERROR);
	return (SUCCESS);
}

int	find_end_of_token(const char *input, int *start, int *end)
{
	char	quote;

	(void)start;
	while (input[(*end)] && input[(*end)] != ' ' && input[(*end)] != '|'
		&& input[(*end)] != '<' && input[(*end)] != '>')
	{
		if (input[(*end)] == '"' || input[(*end)] == '\'')
		{
			quote = input[(*end)];
			(*end)++;
			while (input[(*end)] != quote)
			{
				if (input[(*end)] == '\0')
					return (ERROR);
				(*end)++;
			}
			(*end)++;
		}
		else
			(*end)++;
	}
	return (SUCCESS);
}

void	set_token_type(t_token *tok_lst, const char *input, int *start,
		int *end)
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
		{
			tok_lst->type = REDIR_HEREDOC;
			(*end)++;
		}
	}
	else if (input[(*start)] == '>')
	{
		tok_lst->type = REDIR_OUT;
		(*end)++;
		if (input[(*end)] == '>')
		{
			tok_lst->type = REDIR_APPEND;
			(*end)++;
		}
	}
	else
		if (!(input[(*end)] == '\'' || !is_env_var(tok_lst->value)))
			tok_lst->expand = EXPAND;
}

int	set_value(t_token *tok_lst, const char *input, int *start, int *end)
{
	tok_lst->value = ft_substr(input, (*start), (*end) - (*start));
	if (!tok_lst->value)
	{
		perror("ft_substr failed.\n");
		return (ERROR);
	}
	return (SUCCESS);
}

int	tokenize_input(t_token *tok_lst, const char *input)
{
	int	start;
	int	end;
	int	i;

	i = 0;
	start = 0;
	while (input[start])
	{
		skip_spaces(input, &start);
		end = start;
		if (find_end_of_token(input, &start, &end) == ERROR)
			return (ERROR);
		set_value(tok_lst, input, &start, &end);
		set_token_type(tok_lst, input, &start, &end);
		i = end;
		while (input[i] && input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		create_token_node(&tok_lst);
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
