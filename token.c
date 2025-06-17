/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:05:28 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/17 18:44:41 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_token_list(t_token **tok_lst)
{
	(*tok_lst) = malloc(sizeof(t_token));
	if (!tok_lst)
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

	while (input[(*end)] && input[(*end)] != ' ' && input[(*end)] != '|'
		&& input[(*end)] != '<' && input[(*end)] != '>')
	{
		if (input[(*end)] == '"' || input[(*end)] == '\'')
		{
			quote = input[(*end)];
			(*end)++;
			while (input[(*end)] != quote)
			{
				(*end)++;
				if (input[(*end)] == '\0')
					return (ERROR);
			}
			(*end)++;
		}
		else
			(*end)++;
	}
	if (input[(*start)] == '\'' || input[(*start)] == '"')
		(*start)++;
	if (input[(*end)] == '\'' || input[(*end)] == '"')
		(*end)--;
	return (SUCCESS);
}

void	set_token_type(t_token *tok_lst, const char *input, int *end)
{
	if (input[(*end)] == '|')
		tok_lst->type = PIPE;
	else if (input[(*end)] == '<')
	{
		tok_lst->type = REDIR_IN;
		if (input[(*end) + 1] == '<')
		{
			tok_lst->type = REDIR_HEREDOC;
			(*end)++;
		}
	}
	else if (input[(*end)] == '>')
	{
		tok_lst->type = REDIR_OUT;
		if (input[(*end) + 1] == '>')
		{
			tok_lst->type = REDIR_APPEND;
			(*end)++;
		}
	}
	else
	{
		if (!(input[(*end)] == '\'' || !is_env_var(tok_lst->value)))
			tok_lst->expand = EXPAND;
	}
}

int	set_value(t_token *tok_lst, const char *input, int *start, int *end)
{
	// printf("input[start] = %c, input[end] = %c\n", input[(*start)],
	// input[(*end)
	// - 1]);
	if (input[(*end)] == '\'' || input[(*end)] == '"')
	{
		(*end) -= 2;
		// printf("input[end] = %c\n", input[(*end)]);
		tok_lst->value = ft_substr(input, (*start), (*end) - (*start));
		(*end) += 2;
	}
	else
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
		if (skip_spaces(input, &start) == ERROR)
			return (ERROR);
		end = start;
		if (find_end_of_token(input, &start, &end) == ERROR)
			return (ERROR);
		set_value(tok_lst, input, &start, &end);
		set_token_type(tok_lst, input, &end);
		i = end;
		while (input[i] && input[i] == ' ')
			i++;
		if (input[i] == '\0')
			break ;
		create_token_node(&tok_lst);
		if (!tok_lst->next)
			return (ERROR);
		printf("Token created: %s, type: %d, expand: %d\n", tok_lst->value,
			tok_lst->type, tok_lst->expand);
		tok_lst = tok_lst->next;
		start = end + 1;
	}
	tok_lst->next = NULL;
	return (SUCCESS);
}

// int	tokenize_input(t_token *tok_lst, const char *input)
// {
// 	int		start;
// 	int		end;
// 	int		i;
// 	char	quote;

// 	start = 0;
// 	end = 0;
// 	while (input[start])
// 	{
// 		while (input[start] && input[start] == ' ')
// 			start++;
// 		if (input[start] == '\0')
// 			break ;
// 		end = start;
// 		while (input[end] && input[end] != ' ' && input[end] != '|'
// 			&& input[end] != '<' && input[end] != '>')
// 			end++;
// 		if (input[start] == '"' || input[start] == '\'')
// 		{
// 			quote = input[start++];
// 			while (input[end] && input[end] != quote)
// 				end++;
// 			if (input[end] == '\0')
// 				return (ERROR);
// 		}
// 		tok_lst->value = ft_substr(input, start, end - start);
// 		if (!tok_lst->value)
// 			return (ERROR);
// 		if (input[end] == '|')
// 		{
// 			tok_lst->type = PIPE;
// 		}
// 		else if (input[end] == '<')
// 		{
// 			if (input[end + 1] == '<')
// 			{
// 				tok_lst->type = REDIR_HEREDOC;
// 				end++;
// 			}
// 			else
// 				tok_lst->type = REDIR_IN;
// 		}
// 		else if (input[end] == '>')
// 		{
// 			if (input[end + 1] == '>')
// 			{
// 				tok_lst->type = REDIR_APPEND;
// 				end++;
// 			}
// 			else
// 				tok_lst->type = REDIR_OUT;
// 		}
// 		else
// 		{
// 			tok_lst->type = WORD;
// 			if (input[end] == '\'' || !is_env_var(tok_lst->value))
// 				tok_lst->expand = NO_EXPAND;
// 			else
// 				tok_lst->expand = EXPAND;
// 		}
// 		tok_lst->next = malloc(sizeof(t_token));
// 		if (!tok_lst->next)
// 			return (ERROR);
// 		i = end;
// 		while (input[i] && input[i] == ' ')
// 			i++;
// 		if (input[i] == '\0')
// 			break ;
// 		tok_lst = tok_lst->next;
// 		start = end + 1;
// 	}
// 	tok_lst->next = NULL;
// 	return (SUCCESS);
// }
