/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:13:30 by agaland           #+#    #+#             */
/*   Updated: 2025/07/24 00:55:11 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	execute_pipeline(t_ast *ast, t_sh *shell, t_ast *root)
{
	int		pfd[2];
	int		status;
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pfd) < 0)
	{
		perror("pipe");
		cleanup_shell(shell, root);
		exit(1);
	}
	pid_left = process_left_child(ast, shell, pfd, root);
	pid_right = process_right_child(ast, shell, pfd, root);
	close(pfd[0]);
	close(pfd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	shell->exit_status = process_wait_status(status);
	if (shell->exit_status == 130)
		ft_printf("\n");
	else if (shell->exit_status == 131)
		ft_printf("Quit (core dumped)\n");
	return (shell->exit_status);
}

int	execute_command(t_ast *ast, t_sh *shell, t_ast *root)
{
	if (!ast->cmd)
	{
		if (shell->in_pipeline)
			exit(0);
		return (0);
	}
	if (is_builtin(ast->cmd->cmd_name))
		return (handle_builtin(ast, shell, root));
	if (shell->in_pipeline)
		handle_binary_pipeline(ast, shell, root);
	else
		return (fork_single_binary(ast, shell, root));
	return (0);
}

int	execute_ast(t_ast *ast, t_sh *shell, t_ast *root)
{
	signal(SIGINT, handle_sigint_exec);
	if (!ast)
		return (0);
	if (ast->type == CMD)
		return (execute_command(ast, shell, root));
	else if (ast->type == PIPE)
		return (execute_pipeline(ast, shell, root));
	signal(SIGINT, handle_sigint);
	return (0);
}
