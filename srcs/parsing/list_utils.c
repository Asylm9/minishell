/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:11:07 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/25 14:53:15 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	create_list_node(t_lst **tok_lst, t_sh *shell)
{
	t_lst	*new_token;
	t_lst	*temp;

	temp = *tok_lst;
	new_token = x_malloc(sizeof(t_lst), shell, NULL, NULL);
	new_token->value = NULL;
	new_token->expand = NO_EXPAND;
	new_token->type = WORD;
	new_token->next = NULL;
	new_token->hd_fd = -1;
	if (*tok_lst == NULL)
		*tok_lst = new_token;
	else
	{
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
	return (SUCCESS);
}

int	create_node_pipe(t_ast **ast, t_sh *shell)
{
	t_ast	*new_ast;

	new_ast = x_malloc(sizeof(t_ast), shell, *ast, NULL);
	new_ast->type = PIPE;
	new_ast->cmd = NULL;
	new_ast->left = *ast;
	new_ast->right = NULL;
	*ast = new_ast;
	return (SUCCESS);
}

void	create_ast_right_node(t_lst **exp_lst, t_ast **ast, t_sh *shell)
{
	(*ast)->right = x_malloc(sizeof(t_ast), shell, *ast, NULL);
	(*ast)->right->cmd = create_node_cmd(exp_lst, shell, *ast);
	(*ast)->right->type = CMD;
	(*ast)->right->right = NULL;
	(*ast)->right->left = NULL;
}

t_command	*create_node_cmd(t_lst **exp_lst, t_sh *shell, t_ast *ast)
{
	t_lst		*tmp;
	t_command	*cmd;

	tmp = *exp_lst;
	cmd = x_malloc(sizeof(t_command), shell, ast, NULL);
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
	cmd->args = fill_cmd(&tmp, count_args(tmp), &cmd->cmd_name);
	return (cmd);
}

int	skip_spaces(const char *input, int *pos)
{
	while (input[*pos] && input[*pos] == ' ')
		(*pos)++;
	if (input[*pos] == '\0')
		return (ERROR);
	return (SUCCESS);
}
