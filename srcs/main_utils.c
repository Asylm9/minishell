/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:03:39 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/25 15:04:50 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_ast(t_ast **ast, t_sh *shell)
{
	*ast = x_malloc(sizeof(t_ast), shell, *ast, NULL);
	(*ast)->cmd = NULL;
	(*ast)->left = NULL;
	(*ast)->right = NULL;
	return (SUCCESS);
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
