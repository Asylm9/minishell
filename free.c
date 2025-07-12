/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:42:02 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/12 21:22:38 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tok_lst(t_lst **list)
{
	t_lst	*temp;

	if (!list)
		return ;
	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value != NULL)
			free((*list)->value);
		(*list)->value = NULL;
		free(*list);
		*list = temp;
	}
	*list = NULL;
}

void	free_redir(t_redirect *redirection)
{
	t_redirect	*current;

	if (redirection)
	{
		while (redirection)
		{
			current = redirection->next;
			// redirection = redirection->next;
			if (redirection->target)
				free(redirection->target);
			free(redirection);
			redirection = current;
		}
	}
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (cmd)
	{
		i = 0;
		if (cmd->cmd_name)
		{
			free(cmd->cmd_name);
			cmd->cmd_name = NULL;
		}
		if (cmd->args)
		{
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->redirections)
		{
			if (cmd->redirections->fd != -1)
				close(cmd->redirections->fd);
			free_redir(cmd->redirections);
		}
		free(cmd);
		cmd = NULL;
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
		ast = NULL;
	}
}
