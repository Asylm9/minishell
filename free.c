/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:42:02 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/26 21:36:42 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tok_lst(t_token *list)
{
	t_token	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

void	free_redir(t_redirect *redirection)
{
	t_redirect	*current;

	if (redirection)
	{
		while (redirection)
		{
			current = redirection;
			redirection = redirection->next;
			if (current->target)
				free(current->target);
			free(current);
		}
		free(redirection);
	}
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (cmd)
	{
		i = 1;
		if (cmd->cmd_name)
			free(cmd->cmd_name);
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		if (cmd->args)
			free(cmd->args);
		if (cmd->redirections)
			free_redir(cmd->redirections);
		free(cmd);
	}
}

void	free_ast(t_ast *ast)
{
	if (ast)
	{
		if (ast->cmd)
			free_cmd(ast->cmd);
		if (ast->right)
			free_ast(ast->right);
		if (ast->left)
			free_ast(ast->left);
		free(ast);
	}
}
