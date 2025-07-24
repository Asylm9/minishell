/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:09:47 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/24 14:13:04 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_unclosed_pipes(t_lst *tok_lst, t_lst *exp_lst, t_sh *shell)
{
	t_lst	*new_line;
	char	*input;

	new_line = NULL;
	input = NULL;
	if (!tok_lst->next)
	{
		exp_lst->type = PIPE;
		create_list_node(&exp_lst);
		exp_lst = exp_lst->next;
		input = readline(">");
		create_list_node(&new_line);
		tokenize_input(new_line, input);
		if (input != NULL)
			free(input);
		expand_list(new_line, exp_lst, shell);
		tok_lst = tok_lst->next;
	}
}

int	process_heredoc(t_lst *tok_lst, t_lst *exp_lst, t_sh *shell)
{
	if (tok_lst->next->value)
	{
		exp_lst->hd_fd = handle_heredoc(tok_lst->next->value, shell);
		if (exp_lst->hd_fd == 130)
			return (ERROR);
	}
	return (SUCCESS);
}

void	process_expand(t_lst *tok_lst, t_lst *exp_lst, char *expanded)
{
	int		count;
	char	*presplit;
	char	**splitted;
	int		i;

	i = 0;
	count = count_nb_words(expanded, ' ');
	if (i < count)
	{
		presplit = trim_quotes(expanded);
		splitted = ft_split(presplit, ' ');
		while (count != 0)
		{
			exp_lst->value = ft_strdup(splitted[i++]);
			exp_lst->type = WORD;
			create_list_node(&exp_lst);
			exp_lst = exp_lst->next;
			count--;
		}
		free_array(splitted, -1);
		free(presplit);
		return ;
	}
	exp_lst->value = trim_quotes(expanded);
	exp_lst->type = tok_lst->type;
}

int	process_lst_node(t_lst *tok_lst, t_lst *exp_lst, t_sh *shell)
{
	char	*expanded;
	
	exp_lst->expand = NO_EXPAND;
	if (tok_lst->type == REDIR_HEREDOC)
		if (process_heredoc(tok_lst, exp_lst, shell) == ERROR)
			return (ERROR);
	expanded = expand_token(tok_lst->value, shell);
	if (tok_lst->expand == NO_EXPAND)
	{
		exp_lst->value = trim_quotes(tok_lst->value);
		exp_lst->type = tok_lst->type;
	}
	else if ((ft_strlen(expanded)) != 0)
		process_expand(tok_lst, exp_lst, expanded);
	else
	{
		exp_lst->value = NULL;
		exp_lst->type = tok_lst->type;
	}
	free(expanded);
	return (SUCCESS);
}

int	process_lst(t_lst *tok_lst, t_lst *exp_lst, t_sh *shell)
{
	while (tok_lst)
	{
		if (check_validity(tok_lst, shell) == SUCCESS)
		{
			if (process_lst_node(tok_lst, exp_lst, shell) == ERROR)
				return (ERROR);
			tok_lst = tok_lst->next;
			if (tok_lst != NULL)
			{
				create_list_node(&exp_lst);
				exp_lst = exp_lst->next;
			}
		}
		else if (tok_lst->type == PIPE)
			handle_unclosed_pipes(tok_lst, exp_lst, shell);
		else
			return (ERROR);
	}
	return (SUCCESS);
}
