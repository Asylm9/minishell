/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:53:25 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/25 17:05:03 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_redirect	*add_redirection(t_redirect *redir, t_lst *exp_lst, t_sh *shell,
		t_ast *ast)
{
	t_redirect	*new_redir;
	t_redirect	*tmp;

	new_redir = x_malloc(sizeof(t_redirect), shell, ast, NULL);
	new_redir->type = exp_lst->type;
	new_redir->target = x_strdup(exp_lst->next->value, shell, ast, NULL);
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
		else if (exp_lst && exp_lst->type == WORD && exp_lst->value)
		{
			i++;
			exp_lst = exp_lst->next;
		}
		else if (exp_lst)
			exp_lst = exp_lst->next;
	}
	return (i);
}

char	**fill_cmd(t_lst **exp_lst, char **cmd_name, t_sh *shell, t_ast *ast)
{
	char	**args;
	int		i;
	t_lst	*tmp;

	i = 0;
	tmp = *exp_lst;
	args = x_malloc(sizeof(char *) * (count_args(tmp) + 1), shell, ast, NULL);
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type >= 3 && tmp->type <= 6)
			tmp = tmp->next->next;
		else if (tmp->value != NULL)
		{
			if (i == 0 && cmd_name)
				*cmd_name = x_strdup(tmp->value, shell, ast, NULL);
			args[i++] = x_strdup(tmp->value, shell, ast, NULL);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

void	process_node(t_lst *exp_lst, t_ast **ast, t_sh *shell)
{
	while (exp_lst)
	{
		if (exp_lst && exp_lst->type == PIPE)
		{
			create_node_pipe(ast, shell);
			if (exp_lst->next)
			{
				exp_lst = exp_lst->next;
				if (exp_lst->type >= 3 && exp_lst->type <= 6)
					create_ast_right_node(&exp_lst, ast, shell);
			}
		}
		if (exp_lst && exp_lst->type == WORD)
			create_ast_right_node(&exp_lst, ast, shell);
	}
}

int	parse_ast(t_lst *exp_lst, t_ast **ast, t_sh *shell)
{
	(void)shell;
	if (exp_lst->type == PIPE)
	{
		shell->exit_status = 2;
		printf_fd(STDERR, " syntax error near unexpected token `|'\n");
		return (ERROR);
	}
	(*ast)->cmd = create_node_cmd(&exp_lst, shell, *ast);
	if ((*ast)->cmd == NULL)
	{
		printf_fd(STDERR, "Malloc error, exiting shell...\n");
		cleanup_exit(shell, (*ast));
	}
	(*ast)->type = CMD;
	process_node(exp_lst, ast, shell);
	if (!(*ast)->left && !(*ast)->right && !(*ast)->cmd->cmd_name)
		return (ERROR);
	return (SUCCESS);
}
