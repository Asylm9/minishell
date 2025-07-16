/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:03:39 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/16 15:04:45 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	exit(shell->exit_status);
}

int	init_ast(t_ast **ast, t_sh *shell)
{
	*ast = malloc(sizeof(t_ast));
	if (!ast)
	{
		free_tok_lst(&shell->tok_lst);
		free_tok_lst(&shell->exp_lst);
		shell->tok_lst = NULL;
		shell->exp_lst = NULL;
		return (ERROR);
	}
	else
	{
		(*ast)->cmd = NULL;
		(*ast)->left = NULL;
		(*ast)->right = NULL;
		return (SUCCESS);
	}
}

int	tokenize(char *input, t_sh *shell)
{
	if (create_list_node(&shell->tok_lst) == ERROR)
	{
		free(input);
		return (ERROR);
	}
	else if (tokenize_input(shell->tok_lst, input) == ERROR)
	{
		free_tok_lst(&shell->tok_lst);
		free(input);
		return (ERROR);
	}
	return (SUCCESS);
}