# include "../../minishell.h"

pid_t	process_left_child(t_ast *ast, t_sh *shell, int *pfd)
{
	pid_t	pid_left;

	pid_left = fork();
	if (pid_left == 0)
	{
		shell->in_pipeline = true;
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		exit(execute_ast(ast->left, shell));
	}
	return (pid_left);
}

pid_t	process_right_child(t_ast *ast, t_sh *shell, int *pfd)
{
	pid_t	pid_right;

	pid_right = fork();
	if (pid_right == 0)
	{
		shell->in_pipeline = true;
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		exit(execute_ast(ast->right, shell));
	}
	return (pid_right);
}

int	handle_builtin(t_command *cmd, t_sh *shell)
{
	int	ret;

	if (cmd->redirections && !shell->in_pipeline)
		save_or_restore_fds(shell, 's');
	ret = execute_builtin(cmd, shell);
	if (cmd->redirections && !shell->in_pipeline)
		save_or_restore_fds(shell, 'r');
	if (shell->in_pipeline)
		exit(ret);
	return (ret);
}

void	handle_binary_pipeline(t_command *cmd, t_sh *shell)
{
	if (apply_redirections(cmd) == ERROR)
		exit(1);
	exit(execute_binary(cmd, shell->envl));
}

int	fork_single_binary(t_command *cmd, t_sh *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (ERROR);
	else if (pid == 0)
	{
		if (apply_redirections(cmd) == ERROR)
			exit(1);
		exit(execute_binary(cmd, shell->envl));
	}
	waitpid(pid, &status, 0);
	return (process_wait_status(status));
}
