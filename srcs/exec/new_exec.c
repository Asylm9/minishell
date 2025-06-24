# include "../../minishell.h"

int	execute_pipeline(t_ast *ast, t_sh *shell)
{
	int		pfd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pfd) < 0)
		return (ERROR);
	pid_left = process_left_child(ast, shell, pfd);
	pid_right = process_right_child(ast, shell, pfd);
	close(pfd[0]);
	close(pfd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	shell->exit_status = process_wait_status(status);
	return (shell->exit_status);
}

int	execute_command(t_command *cmd, t_sh *shell)
{
	if (!cmd || !cmd->cmd_name)
	{
		if (shell->in_pipeline)
			exit(0);
		return (0);
	}
	if (is_builtin(cmd->cmd_name))
		return (handle_builtin(cmd, shell));
	if (shell->in_pipeline)
		handle_binary_pipeline(cmd, shell);
	else
		return (fork_single_binary(cmd, shell));
	return (0);
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
