/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:56:44 by agaland           #+#    #+#             */
/*   Updated: 2025/07/25 14:57:45 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	exit(shell->exit_status);
}

void	*x_malloc(int size, t_sh *shell, t_ast *ast)
{
	void	*var;

	var = malloc(size);
	if (!var)
		malloc_exit(shell, ast);
	return (var);
}

char	*x_strjoin(const char *s1, const char *s2, t_sh *shell, t_ast *ast)
{
	void	*var;

	var = ft_strjoin(s1, s2);
	if (!var)
		malloc_exit(shell, ast);
	return (var);
}

char	*x_strdup(const char *s1, t_sh *shell, t_ast *ast)
{
	void	*var;

	var = ft_strdup(s1);
	if (!var)
		malloc_exit(shell, ast);
	return (var);
}

void	malloc_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	printf_fd(STDERR, "Malloc failure: exiting program...");
	exit(EXIT_FAILURE);
}
