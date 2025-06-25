/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:53:25 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/25 16:07:43 by agaland          ###   ########.fr       */
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

void	free_redir(t_redirect *redirection)
{
	t_redirect	*current;

	current = redirection;
	while (redirection->next)
	{
		redirection = redirection->next;
		free(current);
	}
}

t_command	*create_node_cmd(t_token **exp_lst)
{
	int			i;
	t_token		*tmp;
	t_command	*cmd;
	t_redirect	*current;

	i = 0;
	tmp = (*exp_lst);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redirections = NULL;
	while ((*exp_lst) && (*exp_lst)->type != PIPE)
	{
		if ((*exp_lst)->type == WORD)
			i++;
		else if ((*exp_lst)->next && (*exp_lst)->next->value)
		{
			if (cmd->redirections == NULL)
			{
				cmd->redirections = malloc(sizeof(t_redirect));
				if (cmd->redirections == NULL)
					return (NULL);
				cmd->redirections->next = NULL;
				cmd->redirections->type = (*exp_lst)->type;
				cmd->redirections->target = (*exp_lst)->next->value;
				cmd->redirections->fd = (*exp_lst)->hd_fd;
			}
			else
			{
				current = cmd->redirections;
				while (current->next != NULL)
					current = current->next;
				current->next = malloc(sizeof(t_redirect));
				if (current->next == NULL)
					return (NULL);
				current->next->type = (*exp_lst)->type;
				current->next->target = (*exp_lst)->next->value;
				cmd->redirections->fd = (*exp_lst)->hd_fd;
				current->next->next = NULL;
			}
			(*exp_lst) = (*exp_lst)->next;
		}
		(*exp_lst) = (*exp_lst)->next;
	}
	cmd->args = malloc(sizeof(char *) * (i + 1));
	if (!cmd->args)
	{
		// Free redirections if needed
		free(cmd);
		return (NULL);
	}
	i = 0;
	(*exp_lst) = tmp;
	while ((*exp_lst) && (*exp_lst)->type != PIPE)
	{
		if ((*exp_lst)->type == REDIR_APPEND
			|| (*exp_lst)->type == REDIR_HEREDOC || (*exp_lst)->type == REDIR_IN
			|| (*exp_lst)->type == REDIR_OUT)
			(*exp_lst) = (*exp_lst)->next->next;
		else
		{
			if (i == 0)
				cmd->cmd_name = (*exp_lst)->value;
			cmd->args[i] = (*exp_lst)->value;
			i++;
			(*exp_lst) = (*exp_lst)->next;
		}
	}
	cmd->args[i] = NULL;
	return (cmd);
}

int	parse_ast(t_token *exp_lst, t_ast **ast)
{
	(*ast)->cmd = create_node_cmd(&exp_lst);
	(*ast)->type = CMD;
	while (exp_lst)
	{
		if (exp_lst && exp_lst->type == PIPE)
		{
			create_node_pipe(ast);
			exp_lst = exp_lst->next;
			if (exp_lst->type == REDIR_OUT || exp_lst->type == REDIR_IN || exp_lst->type == REDIR_APPEND || exp_lst->type == REDIR_HEREDOC)
			{
				(*ast)->right = malloc(sizeof(t_ast));
				(*ast)->right->cmd = create_node_cmd(&exp_lst);
				(*ast)->right->type = CMD;
			}
		}
		if (exp_lst && exp_lst->type == WORD)
		{
			(*ast)->right = malloc(sizeof(t_ast));
			(*ast)->right->cmd = create_node_cmd(&exp_lst);
			(*ast)->right->type = CMD;
		}
	}
	return (SUCCESS);
}
void	print_ast(t_ast *ast)
{
	printf("Type:\n");
	if (!ast)
		return ;
	if (ast && ast->type == CMD && ast->cmd)
	{
		if (ast->cmd->cmd_name == NULL)
			printf("	Command:\n		Name: NULL\n");
		else
			printf("	Command:\n		Name: %s\n", ast->cmd->cmd_name);
		if (ast->cmd->args)
		{
			printf("		Arguments:\n");
			for (int i = 0; ast->cmd->args[i]; i++)
			{
				printf("			%s\n", ast->cmd->args[i]);
			}
			if (ast->cmd->redirections)
				printf("Redirection : %s\n", ast->cmd->redirections->target);
		}
		else
			printf("No arguments\n");
	}
	else if (ast->type == PIPE)
		printf("	Pipe\n");
	else if (ast->type == REDIR_IN)
	{
		printf("	Redirect In\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("        Command:\n");
			if (ast->cmd->cmd_name == NULL)
				printf("		Name: NULL\n");
			else
				printf("		Name: %s\n", ast->cmd->cmd_name);
			if (ast->cmd->args)
			{
				printf("		Arguments:\n");
				for (int i = 0; ast->cmd->args[i]; i++)
				{
					printf("			%s\n", ast->cmd->args[i]);
				}
			}
			else
				printf("No arguments\n");
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else if (ast->type == REDIR_OUT)
	{
		printf("	Redirect Out\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else if (ast->type == REDIR_APPEND)
	{
		printf("	Redirect Append\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else if (ast->type == REDIR_HEREDOC)
	{
		printf("	Redirect Heredoc\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else
		printf("Unknown type\n");
	if (ast->left)
	{
		printf("Left child:\n");
		printf("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
		print_ast(ast->left);
		printf("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
	}
	else
		printf("No left child\n");
	if (ast->right)
	{
		printf("Right child:\n");
		printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
		print_ast(ast->right);
		printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
	}
	else
		printf("No right child\n");
}
