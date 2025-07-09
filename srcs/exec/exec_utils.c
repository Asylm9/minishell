#include "../../minishell.h"

pid_t	process_left_child(t_ast *ast, t_sh *shell, int *pfd, t_ast *root)
{
	pid_t	pid_left;

	pid_left = fork();
	if (pid_left == 0)
	{
		set_subprocess_signals();
		shell->in_pipeline = true;
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		exit(execute_ast(ast->left, shell, root));
	}
	return (pid_left);
}

pid_t	process_right_child(t_ast *ast, t_sh *shell, int *pfd, t_ast *root)
{
	pid_t	pid_right;

	pid_right = fork();
	if (pid_right == 0)
	{
		set_subprocess_signals();
		shell->in_pipeline = true;
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		exit(execute_ast(ast->right, shell, root));
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
	shell->exit_status = execute_binary(ast, shell, root);
	exit(shell->exit_status);
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
		set_subprocess_signals();
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
	return (shell->exit_status);
}
