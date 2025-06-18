/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:53:25 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/18 15:56:10 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(t_token *tok_lst)
{
	int	count;

	count = 0;
	while (tok_lst)
	{
		if (tok_lst->type == PIPE)
			count++;
		tok_lst = tok_lst->next;
	}
	return (count);
}

int	create_node_pipe(t_ast **ast, t_token *tok_lst)
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

int	create_node_redir(t_ast **ast, t_token *tok_lst)
{
	t_ast	*new_ast;

	new_ast = malloc(sizeof(t_ast));
	if (!new_ast)
		return (ERROR);
	new_ast->type = tok_lst->type;
	new_ast->cmd = NULL;
	new_ast->left = *ast;
	new_ast->right = NULL;
	*ast = new_ast;
	return (SUCCESS);
}

int	create_node_command(t_ast **ast, t_token *tok_lst)
{
	t_ast	*new_ast;

	new_ast = malloc(sizeof(t_ast));
	if (!new_ast)
		return (1);
	new_ast->type = WORD;
	new_ast->cmd = malloc(sizeof(t_command));
	if (!new_ast->cmd)
	{
		free(new_ast);
		return (1);
	}
	new_ast->cmd->cmd_name = strdup(tok_lst->value);
	if (!new_ast->cmd->cmd_name)
	{
		free(new_ast->cmd);
		free(new_ast);
		return (1);
	}
	new_ast->cmd->args = NULL;
	if (ast)
		new_ast->left = *ast;
	else
		new_ast->left = NULL;
	new_ast->right = NULL;
	*ast = new_ast;
	return (0);
}

int	parse_ast(t_token *tok_lst, t_ast **ast)
{
	t_ast	*new_ast;
	int		arg_count;
	int		i;
	char	**new_args;

	printf("TEST CA PTN\n");
	printf("tok lst value : %s\n", tok_lst->value);
	while (tok_lst)
	{
		if (tok_lst->type == WORD)
			create_node_command(ast, tok_lst);
		else if (tok_lst->type == PIPE)
			create_node_pipe(ast, tok_lst);
		else if (tok_lst->type == REDIR_IN || tok_lst->type == REDIR_OUT
			|| tok_lst->type == REDIR_APPEND || tok_lst->type == REDIR_HEREDOC)
			create_node_redir(ast, tok_lst);
		tok_lst = tok_lst->next;
		// ast = ast->right;
		if ((*ast)->cmd && (*ast)->cmd->cmd_name)
			printf("Node created, type : %d  value : %s\n", (*ast)->type, (*ast)->cmd->cmd_name);
		else
			printf("Node created, type : %d  value : (no cmd)\n", (*ast)->type);
		if ((*ast)->left && (*ast)->left->cmd)
		{
			printf("Left child command name: %s\n", (*ast)->left->cmd->cmd_name);
		}
		else
		{
			printf("No left child or no command in left child.\n");
		}
	}
	printf("AST created successfully.\n");
	return (0);
}

void	print_ast(t_ast *ast)
{
	printf("Type:\n");
	if (!ast)
		return ;
	if (ast->type == WORD && ast->cmd)
	{
		if (ast->cmd->cmd_name == NULL)
			printf("	Command:\n		Name: NULL\n");
		else
			printf("	Command:\n		Name: %s\n", ast->cmd->cmd_name);
		if (ast->cmd->args)
		{
			printf("		Arguments count: %d\n", ast->cmd->argc);
			printf("		Arguments:\n");
			for (int i = 0; ast->cmd->args[i]; i++)
			{
				printf("			%s\n", ast->cmd->args[i]);
			}
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
				printf("		Arguments count: %d\n", ast->cmd->argc);
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
	// else
	// 	printf("No left child\n");
	if (ast->right)
	{
		printf("Right child:\n");
		printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
		print_ast(ast->right);
		printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
	}
	// else
	// 	printf("No right child\n");
}

// void	print_ast(t_ast *ast)
// {
// 	printf("Type: %d\n", ast->type);
// 	if (!ast)
// 		return ;
// 	if (ast->type == WORD && ast->cmd)
// 	{
// 		if (ast->cmd->cmd_name == NULL)
// 			printf("Command name: NULL\n");
// 		else
// 			printf("Command name: %s\n", ast->cmd->cmd_name);
// 		if (ast->cmd->args)
// 		{
// 			printf("Arguments:\n");
// 			for (int i = 0; ast->cmd->args[i]; i++)
// 				printf("  Arg %d: %s\n", i + 1, ast->cmd->args[i]);
// 		}
// 		else
// 			printf("No arguments\n");
// 	}
// 	else if (ast->type == PIPE)
// 		printf("Pipe\n");
// 	else if (ast->type == REDIR_IN)
// 		printf("Redirect In\n");
// 	else if (ast->type == REDIR_OUT)
// 		printf("Redirect Out\n");
// 	else if (ast->type == REDIR_APPEND)
// 		printf("Redirect Append\n");
// 	else if (ast->type == REDIR_HEREDOC)
// 		printf("Redirect Heredoc\n");
// 	if (ast->left)
// 		print_ast(ast->left);
// 	else
// 		printf("No left child\n");
// 	printf("Right child:\n");
// 	printf("ast right value: %p\n", (void *)ast->right);
// 	if (ast->right->cmd)
// 		print_ast(ast->right);
// 	else
// 		printf("No right child\n");
// }
