/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:42:02 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/01 11:29:53 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tok_lst(t_token **list)
{
	t_token	*temp;

	if (!list)
		return;
	while (*list)
	{
		temp = (*list)->next;
		if ((*list)->value)
			free((*list)->value);
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
			current = redirection;
			redirection = redirection->next;
			if (current->target)
				free(current->target);
			free(current);
			current = NULL;
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
		while (cmd->args[i])
			free(cmd->args[i++]);
		if (cmd->args)
			free(cmd->args);
		if (cmd->redirections)
			free_redir(cmd->redirections);
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
