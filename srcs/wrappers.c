/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:56:44 by agaland           #+#    #+#             */
/*   Updated: 2025/07/28 15:49:15 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	exit(shell->exit_status);
}

void	*x_malloc(int size, t_sh *shell, t_ast *ast, void *var_to_free)
{
	void	*var;

	var = malloc(size);
	if (!var)
	{
		if (var_to_free)
			free(var_to_free);
		malloc_exit(shell, ast);
	}
	return (var);
}

char	*x_itoa(int n, t_sh *shell, t_ast *ast)
{
	void	*var;

	var = ft_itoa(n);
	if (!var)
		malloc_exit(shell, ast);
	return (var);
}

char	*x_strdup(const char *s1, t_sh *shell, t_ast *ast, void *var_to_free)
{
	void	*var;

	var = ft_strdup(s1);
	if (!var)
	{
		if (var_to_free)
			free(var_to_free);
		malloc_exit(shell, ast);
	}
	return (var);
}

void	malloc_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	printf_fd(STDERR, "Malloc failure: exiting program...");
	exit(EXIT_FAILURE);
}
