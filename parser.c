/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:53:25 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/21 16:27:39 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_node_pipe(t_ast **ast)
{
	t_ast	*new_ast;

	new_ast = malloc(sizeof(t_ast));
	if (!new_ast)
		return (ERROR);
	new_ast->type = PIPE;
	new_ast->cmd = NULL;
	new_ast->left = *ast;
	new_ast->right = NULL;
	*ast = new_ast;
	return (SUCCESS);
}

t_command	*create_node_cmd(t_token **exp_lst)
{
	int			i;
	t_token		*tmp;
	t_redirect	*tmp_redir;
	t_command	*cmd;

	i = 0;
	tmp = (*exp_lst);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redirections = malloc(sizeof(t_redirect));
	if (!cmd->redirections)
	{
		free(cmd);
		return (NULL);
	}
	cmd->redirections->next = NULL;
	cmd->redirections->target = NULL;
	tmp_redir = cmd->redirections;
	while ((*exp_lst) && (*exp_lst)->type != PIPE)
	{
		if ((*exp_lst)->type == WORD)
			i++;
		else if ((*exp_lst)->next && (*exp_lst)->next->value)
		{
			if (cmd->redirections->target == NULL)
				cmd->redirections->target = (*exp_lst)->next->value;
			else
			{
				cmd->redirections->next = malloc(sizeof(t_redirect));
				if (!cmd->redirections->next)
				{
					free(cmd);
					return (NULL);
				}
				cmd->redirections = cmd->redirections->next;
				cmd->redirections->target = (*exp_lst)->next->value;
			}
			(*exp_lst) = (*exp_lst)->next;
		}
		(*exp_lst) = (*exp_lst)->next;
	}
	cmd->args = malloc(sizeof(char *) * (i + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	(*exp_lst) = tmp;
	cmd->cmd_name = (*exp_lst)->value;
	cmd->args[i] = (*exp_lst)->value;
	i++;
	(*exp_lst) = (*exp_lst)->next;
	while ((*exp_lst) && (*exp_lst)->type != PIPE)
	{
		if ((*exp_lst)->type == REDIR_APPEND
			|| (*exp_lst)->type == REDIR_HEREDOC || (*exp_lst)->type == REDIR_IN
			|| (*exp_lst)->type == REDIR_OUT)
		{
			if ((*exp_lst)->next->next)
				(*exp_lst) = (*exp_lst)->next->next;
			else
			{
				(*exp_lst) = (*exp_lst)->next->next;
				break ;
			}
		}
		else
		{
			cmd->args[i] = (*exp_lst)->value;
			i++;
			(*exp_lst) = (*exp_lst)->next;
		}
	}
	cmd->args[i] = NULL;
	cmd->redirections = tmp_redir;
	return (cmd);
}

int	parse_ast(t_token *tok_lst, t_ast **ast)
{
	(*ast)->cmd = create_node_cmd(&tok_lst);
	(*ast)->type = COMMAND;
	while (tok_lst)
	{
		printf("TEST 4\n");
		if (tok_lst && tok_lst->type == PIPE)
		{
			printf("TEST PIPE\n");
			create_node_pipe(ast);
			tok_lst = tok_lst->next;
		}
		printf("TEST 5\n");
		if (tok_lst && tok_lst->type == WORD)
		{
			printf("TEST CMD\n");
			(*ast)->right = malloc(sizeof(t_ast));
			(*ast)->right->cmd = create_node_cmd(&tok_lst);
			(*ast)->right->type = COMMAND;
		}
		printf("TEST 8\n");
	}
	printf("PARSING DONE\n");
	return (SUCCESS);
}
void	print_ast(t_ast *ast)
{
	t_redirect	*redir;

	if (!ast)
	{
		printf("NULL node\n");
		return ;
	}
	switch (ast->type)
	{
	case COMMAND:
		printf("COMMAND NODE\n");
		if (ast->cmd)
		{
			printf("  Name: %s\n",
				ast->cmd->cmd_name ? ast->cmd->cmd_name : "NULL");
			printf("  Arguments: ");
			if (ast->cmd->args)
			{
				for (int i = 0; ast->cmd->args[i]; i++)
					printf("%s ", ast->cmd->args[i]);
				printf("\n");
			}
			else
				printf("None\n");
			// Print redirections
			redir = ast->cmd->redirections;
			while (redir)
			{
				if (redir->target)
					printf("  Redirection: %s\n", redir->target);
				redir = redir->next;
			}
		}
		else
			printf("  No command struct\n");
		break ;
	case PIPE:
		printf("PIPE NODE\n");
		break ;
	case REDIR_IN:
		printf("REDIR_IN NODE\n");
		break ;
	case REDIR_OUT:
		printf("REDIR_OUT NODE\n");
		break ;
	case REDIR_APPEND:
		printf("REDIR_APPEND NODE\n");
		break ;
	case REDIR_HEREDOC:
		printf("REDIR_HEREDOC NODE\n");
		break ;
	default:
		printf("Unknown node type: %d\n", ast->type);
	}
	// Print children recursively
	if (ast->left)
	{
		printf("Left child:\n");
		print_ast(ast->left);
	}
	if (ast->right)
	{
		printf("Right child:\n");
		print_ast(ast->right);
	}
}
