#include "minishell.h"

int	execute_pipeline(t_ast *ast, t_sh *shell)
{
	int		pfd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pfd) < 0)
		return (ERROR);
	pid_left = fork();
	if (pid_left == 0)
	{
		shell->in_pipeline = true;
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		exit(execute_ast(ast->left, shell));
	}
	pid_right = fork();
	if (pid_right == 0)
	{
		shell->in_pipeline = true;
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		exit(execute_ast(ast->right, shell));
	}
	close(pfd[0]);
	close(pfd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	return (process_wait_status(status));
}

int	save_or_restore_fds(t_sh *shell, char flag)
{
	if (flag == 's')
	{
		shell->saved_stdin = dup(STDIN_FILENO);
		if (shell->saved_stdin < 0)
			return (perror("dup"), ERROR);
		shell->saved_stdout = dup(STDOUT_FILENO);
		if (shell->saved_stdout < 0)
			return (perror("dup"), ERROR);
	}
	else if (flag == 'r')
	{
		if (shell->saved_stdin < 0)
			return (ERROR);
		if (dup2(shell->saved_stdin, STDIN_FILENO) < 0)
			return (perror("dup2"), ERROR);
		if (shell->saved_stdout < 0)
			return (ERROR);
		if (dup2(shell->saved_stdout, STDOUT_FILENO) < 0)
			return (perror("dup2"), ERROR);
	}
	else
		return (ERROR);
	return (SUCCESS);
}

int	execute_command(t_command *cmd, t_sh *shell)
{
	pid_t	pid;
	int		status;
	int		ret;

	if (!cmd || !cmd->cmd_name)
	{
		if (shell->in_pipeline)
			exit(0);
		return (0);
	}
	if (is_builtin(cmd->cmd_name))
	{
		if (cmd->redirections && !shell->in_pipeline)
			save_or_restore_fd(shell, 's');
		ret = execute_builtin(cmd, shell);
		if (cmd->redirections && !shell->in_pipeline)
			save_or_restore_fd(shell, 'r');
		if (shell->in_pipeline)
			exit(ret);
		return (ret);
	}
	if (shell->in_pipeline)
	{
		if (apply_redirections(cmd) == ERROR)
			exit(1);
		exit(execute_binary(cmd, shell->env));
	}
	else
	{
		pid = fork();
		if (pid < 0)
			return (ERROR);
		else if (pid == 0)
		{
			if (apply_redirections(cmd) == ERROR)
				exit(1);
			exit(execute_binary(cmd, shell->env));
		}
		waitpid(pid, &status, 0);
		return (process_wait_status(status));
	}
}

int	execute_ast(t_ast *ast, t_sh *shell)
{
	if (!ast)
		return (0);
	if (ast->type == CMD)
		return (execute_command(ast->cmd, shell));
	else if (ast->type == PIPE)
		return (execute_pipeline(ast, shell));
	return (0);
}
