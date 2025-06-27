/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:53:25 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/27 13:41:00 by magoosse         ###   ########.fr       */
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

static t_redirect	*add_redirection(t_redirect *redir, t_token *exp_lst)
{
	t_redirect	*new_redir;
	t_redirect	*tmp;

	new_redir = malloc(sizeof(t_redirect));
	if (!new_redir)
		return (NULL);
	new_redir->type = exp_lst->type;
	new_redir->target = exp_lst->next->value;
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

static int	count_args(t_token *exp_lst)
{
	int	i;

	i = 0;
	while (exp_lst && exp_lst->type != PIPE)
	{
		if (exp_lst->type == WORD)
			i++;
		exp_lst = exp_lst->next;
	}
	return (i);
}

static char	**fill_args(t_token **exp_lst, int argc, char **cmd_name)
{
	char	**args;
	int		i;
	t_token	*tmp;

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

t_command	*create_node_cmd(t_token **exp_lst)
{
	t_token		*tmp;
	t_command	*cmd;

	tmp = *exp_lst;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd_name = NULL;
	cmd->args = NULL;
	cmd->redirections = NULL;
	while (*exp_lst && (*exp_lst)->type != PIPE)
	{
		if ((*exp_lst)->type >= 3 && (*exp_lst)->next
			&& (*exp_lst)->next->value)
		{
			cmd->redirections = add_redirection(cmd->redirections, *exp_lst);
			*exp_lst = (*exp_lst)->next;
		}
		*exp_lst = (*exp_lst)->next;
	}
	cmd->args = fill_args(&tmp, count_args(tmp), &cmd->cmd_name);
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
			else
			{
				exp_lst = exp_lst->next;
				printf("Error near exp_lst '|', missing next command.\n");
				// return (ERROR);
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
