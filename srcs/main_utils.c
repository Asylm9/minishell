/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:03:39 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/24 19:42:19 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	exit(shell->exit_status);
}

void	malloc_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	printf_fd(STDERR, "Malloc failure: exiting program...");
	exit(EXIT_FAILURE);
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
	if (create_list_node(&shell->tok_lst, shell) == ERROR)
	{
		free(input);
		cleanup_exit(shell, NULL);
	}
	else if (tokenize_input(shell->tok_lst, input, shell) == ERROR)
	{
		free(input);
		cleanup_exit(shell, NULL);
	}
	return (SUCCESS);
}
