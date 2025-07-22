/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:13:18 by agaland           #+#    #+#             */
/*   Updated: 2025/07/22 17:38:26 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

pid_t	process_left_child(t_ast *ast, t_sh *shell, int *pfd, t_ast *root)
{
	pid_t	pid_left;

	pid_left = fork();
	if (pid_left == 0)
	{
		shell->in_pipeline = true;
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		if (!ast->left)
		{
			cleanup_shell(shell, root);
			exit(0);
		}
		if (ast->left->type == PIPE)
			execute_pipeline(ast->left, shell, root);
		else
			exit(execute_ast(ast->left, shell, root));
		cleanup_shell(shell, root);
		exit(0);
	}
	return (pid_left);
}

pid_t	process_right_child(t_ast *ast, t_sh *shell, int *pfd, t_ast *root)
{
	pid_t	pid_right;

	pid_right = fork();
	if (pid_right == 0)
	{
		shell->in_pipeline = true;
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		if (!ast->right)
		{
			cleanup_shell(shell, root);
			exit(0);
		}
		if (ast->right->type == PIPE)
			execute_pipeline(ast->right, shell, root);
		else
			exit(execute_ast(ast->right, shell, root));
		cleanup_shell(shell, root);
		exit(0);
	}
	return (pid_right);
}

int	handle_builtin(t_ast *ast, t_sh *shell, t_ast *root)
{
	int	ret;

	if (ast->cmd->redirections && !shell->in_pipeline)
		save_or_restore_fds(shell, 's');
	if (apply_redirections(ast->cmd) == ERROR)
		exit(1);
	ret = execute_builtin(ast, shell);
	if (ast->cmd->redirections && !shell->in_pipeline)
		save_or_restore_fds(shell, 'r');
	if (shell->in_pipeline)
	{
		cleanup_shell(shell, root);
		exit(ret);
	}
	return (ret);
}

void	handle_binary_pipeline(t_ast *ast, t_sh *shell, t_ast *root)
{
	if (apply_redirections(ast->cmd) == ERROR)
	{
		cleanup_shell(shell, ast);
		exit(1);
	}
	close_all_fds(3);
	exit(execute_binary(ast, shell, root));
}

int	fork_single_binary(t_ast *ast, t_sh *shell, t_ast *root)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (ERROR);
	else if (pid == 0)
	{
		if (apply_redirections(ast->cmd) == ERROR)
		{
			cleanup_shell(shell, ast);
			exit(1);
		}
		exit(execute_binary(ast, shell, root));
	}
	waitpid(pid, &status, 0);
	shell->exit_status = process_wait_status(status);
	if (shell->exit_status == 130)
		printf("\n");
	if (shell->exit_status == 131)
		printf("Quit (core dumped)\n");
	return (0);
}
