/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:09:47 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/25 17:22:03 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	process_heredoc(t_lst *tok_lst, t_lst *exp_lst, t_sh *shell)
{
	if (tok_lst->next->value)
	{
		exp_lst->hd_fd = handle_heredoc(tok_lst->next->value, shell);
		if (exp_lst->hd_fd == 1)
			cleanup_exit(shell, NULL);
		if (exp_lst->hd_fd == 130)
			return (ERROR);
	}
	return (SUCCESS);
}

static void	process_split_expand(t_lst *exp_lst, char **splitted, int count,
		t_sh *shell)
{
	int	i;

	i = 0;
	while (count != 0)
	{
		exp_lst->value = x_strdup(splitted[i++], shell, NULL, NULL);
		if (!exp_lst->value)
			cleanup_exit(shell, NULL);
		exp_lst->type = WORD;
		create_list_node(&exp_lst, shell);
		exp_lst = exp_lst->next;
		count--;
	}
}

void	process_expand(t_lst *tok_lst, t_lst *exp_lst, char *expanded,
		t_sh *shell)
{
	int		count;
	char	*presplit;
	char	**splitted;

	count = count_nb_words(expanded, ' ');
	if (count > 0)
	{
		presplit = trim_quotes(expanded, shell);
		splitted = ft_split(presplit, ' ');
		if (!splitted)
			cleanup_exit(shell, NULL);
		process_split_expand(exp_lst, splitted, count, shell);
		free_array(splitted, -1);
		free(presplit);
		return ;
	}
	exp_lst->value = trim_quotes(expanded, shell);
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
		exp_lst->value = trim_quotes(tok_lst->value, shell);
		exp_lst->type = tok_lst->type;
	}
	else if ((ft_strlen(expanded)) != 0)
		process_expand(tok_lst, exp_lst, expanded, shell);
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
			if (tok_lst->type == PIPE && !tok_lst->next)
			{
				handle_unclosed_pipes(tok_lst, exp_lst, shell);
				exp_lst->type = PIPE;
			}
			else
			{
				if (process_lst_node(tok_lst, exp_lst, shell) == ERROR)
					return (ERROR);
			}
			tok_lst = tok_lst->next;
			if (tok_lst != NULL)
			{
				create_list_node(&exp_lst, shell);
				exp_lst = exp_lst->next;
			}
		}
		else
			return (ERROR);
	}
	return (SUCCESS);
}
