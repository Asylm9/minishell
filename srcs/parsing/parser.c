/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:53:25 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/16 00:02:54 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redirect	*add_redirection(t_redirect *redir, t_lst *exp_lst)
{
	t_redirect	*new_redir;
	t_redirect	*tmp;

	new_redir = malloc(sizeof(t_redirect));
	if (!new_redir)
		return (NULL);
	new_redir->type = exp_lst->type;
	new_redir->target = ft_strdup(exp_lst->next->value);
	new_redir->fd = exp_lst->hd_fd;
	new_redir->next = NULL;
	if (!redir)
		return (new_redir);
	tmp = redir;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_redir;
	return (redir);
}

int	count_args(t_lst *exp_lst)
{
	int	i;

	i = 0;
	while (exp_lst && exp_lst->type != PIPE)
	{
		if (exp_lst->type >= 3)
			exp_lst = exp_lst->next->next;
		if (exp_lst && exp_lst->type == WORD)
			i++;
		if (exp_lst)
			exp_lst = exp_lst->next;
	}
	return (i);
}

char	**fill_args(t_lst **exp_lst, int argc, char **cmd_name)
{
	char	**args;
	int		i;
	t_lst	*tmp;

	args = malloc(sizeof(char *) * (argc + 1));
	i = 0;
	tmp = *exp_lst;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type >= 3 && tmp->type <= 6)
			tmp = tmp->next->next;
		else
		{
			if (i == 0 && cmd_name)
				*cmd_name = ft_strdup(tmp->value);
			args[i++] = ft_strdup(tmp->value);
			tmp = tmp->next;
		}
	}
	args[i] = NULL;
	return (args);
}

void	process_node(t_lst *exp_lst, t_ast **ast)
{
	while (exp_lst)
	{
		if (exp_lst && exp_lst->type == PIPE)
		{
			create_node_pipe(ast);
			if (exp_lst->next)
			{
				exp_lst = exp_lst->next;
				if (exp_lst->type >= 3 && exp_lst->type <= 6)
				{
					(*ast)->right = malloc(sizeof(t_ast));
					(*ast)->right->cmd = create_node_cmd(&exp_lst);
					(*ast)->right->type = CMD;
				}
			}
		}
		if (exp_lst && exp_lst->type == WORD)
		{
			(*ast)->right = malloc(sizeof(t_ast));
			(*ast)->right->cmd = create_node_cmd(&exp_lst);
			(*ast)->right->type = CMD;
			(*ast)->right->right = NULL;
			(*ast)->right->left = NULL;
		}
	}
}

int	parse_ast(t_lst *exp_lst, t_ast **ast, t_sh *shell)
{
	(void)shell;
	if (exp_lst->type == PIPE)
	{
		shell->exit_status = 2;
		printf_fd(STDERR, " syntax error near unexpected token `|'");
		return (ERROR);
	}
	(*ast)->cmd = create_node_cmd(&exp_lst);
	(*ast)->type = CMD;
	process_node(exp_lst, ast);
	if (!(*ast)->left && !(*ast)->right && !(*ast)->cmd->cmd_name)
		return (ERROR);
	return (SUCCESS);
}

// void	print_ast(t_ast *ast)
// {
// 	printf_fd(STDOUT_FILENO, "Type:\n");
// 	if (!ast)
// 		return ;
// 	if (ast && ast->type == CMD && ast->cmd)
// 	{
// 		if (ast->cmd->cmd_name == NULL)
// 			printf_fd(STDOUT_FILENO, "	Command:\n		Name: NULL\n");
// 		else
// 			printf_fd(STDOUT_FILENO, "	Command:\n		Name: %s\n", ast->cmd->cmd_name);
// 		if (ast->cmd->args)
// 		{
// 			printf_fd(STDOUT_FILENO, "		Arguments:\n");
// 			for (int i = 0; ast->cmd->args[i]; i++)
// 			{
// 				printf_fd(STDOUT_FILENO, "			%s\n", ast->cmd->args[i]);
// 			}
// 			if (ast->cmd->redirections)
// 				printf_fd(STDOUT_FILENO, "Redirection : %s\n", ast->cmd->redirections->target);
// 		}
// 		else
// 			printf_fd(STDOUT_FILENO, "No arguments\n");
// 	}
// 	else if (ast->type == PIPE)
// 		printf_fd(STDOUT_FILENO, "	Pipe\n");
// 	else if (ast->type == REDIR_IN)
// 	{
// 		printf_fd(STDOUT_FILENO, "	Redirect In\n");
// 		if (ast->cmd && ast->cmd->redirections)
// 		{
// 			printf_fd(STDOUT_FILENO, "        Command:\n");
// 			if (ast->cmd->cmd_name == NULL)
// 				printf_fd(STDOUT_FILENO, "		Name: NULL\n");
// 			else
// 				printf_fd(STDOUT_FILENO, "		Name: %s\n", ast->cmd->cmd_name);
// 			if (ast->cmd->args)
// 			{
// 				printf_fd(STDOUT_FILENO, "		Arguments:\n");
// 				for (int i = 0; ast->cmd->args[i]; i++)
// 				{
// 					printf_fd(STDOUT_FILENO, "			%s\n", ast->cmd->args[i]);
// 				}
// 			}
// 			else
// 				printf_fd(STDOUT_FILENO, "No arguments\n");
// 			printf_fd(STDOUT_FILENO, "		Redirection Target: %s\n",
// 						ast->cmd->redirections->target);
// 			printf_fd(STDOUT_FILENO, "		File Descriptor: %d\n", ast->cmd->redirections->fd);
// 		}
// 		else
// 			printf_fd(STDOUT_FILENO, "No redirection target or file descriptor\n");
// 	}
// 	else if (ast->type == REDIR_OUT)
// 	{
// 		printf_fd(STDOUT_FILENO, "	Redirect Out\n");
// 		if (ast->cmd && ast->cmd->redirections)
// 		{
// 			printf_fd(STDOUT_FILENO, "		Redirection Target: %s\n",
// 						ast->cmd->redirections->target);
// 			printf_fd(STDOUT_FILENO, "		File Descriptor: %d\n", ast->cmd->redirections->fd);
// 		}
// 		else
// 			printf_fd(STDOUT_FILENO, "No redirection target or file descriptor\n");
// 	}
// 	else if (ast->type == REDIR_APPEND)
// 	{
// 		printf_fd(STDOUT_FILENO, "	Redirect Append\n");
// 		if (ast->cmd && ast->cmd->redirections)
// 		{
// 			printf_fd(STDOUT_FILENO, "		Redirection Target: %s\n",
// 						ast->cmd->redirections->target);
// 			printf_fd(STDOUT_FILENO, "		File Descriptor: %d\n", ast->cmd->redirections->fd);
// 		}
// 		else
// 			printf_fd(STDOUT_FILENO, "No redirection target or file descriptor\n");
// 	}
// 	else if (ast->type == REDIR_HEREDOC)
// 	{
// 		printf_fd(STDOUT_FILENO, "	Redirect Heredoc\n");
// 		if (ast->cmd && ast->cmd->redirections)
// 		{
// 			printf_fd(STDOUT_FILENO, "		Redirection Target: %s\n",
// 						ast->cmd->redirections->target);
// 			printf_fd(STDOUT_FILENO, "		File Descriptor: %d\n", ast->cmd->redirections->fd);
// 		}
// 		else
// 			printf_fd(STDOUT_FILENO, "No redirection target or file descriptor\n");
// 	}
// 	else
// 		printf_fd(STDOUT_FILENO, "Unknown type\n");
// 	if (ast->left)
// 	{
// 		printf_fd(STDOUT_FILENO, "Left child:\n");
// 		printf_fd(STDOUT_FILENO, "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
// 		print_ast(ast->left);
// 		printf_fd(STDOUT_FILENO, "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
// 	}
// 	else
// 		printf_fd(STDOUT_FILENO, "No left child\n");
// 	if (ast->right)
// 	{
// 		printf_fd(STDOUT_FILENO, "Right child:\n");
// 		printf_fd(STDOUT_FILENO, "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
// 		print_ast(ast->right);
// 		printf_fd(STDOUT_FILENO, "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
// 	}
// 	else
// 		printf_fd(STDOUT_FILENO, "No right child\n");
// }
