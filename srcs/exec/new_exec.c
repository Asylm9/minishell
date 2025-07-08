#include "../../minishell.h"

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

int	execute_command(t_ast *ast, t_sh *shell)
{
	if (!ast->cmd)
	{
		if (shell->in_pipeline)
			exit(0);
		return (0);
	}
	signal(SIGINT, SIG_IGN);
	if (is_builtin(ast->cmd->cmd_name))
		return (handle_builtin(ast, shell));
	if (shell->in_pipeline)
		handle_binary_pipeline(ast->cmd, shell);
	else
		return (fork_single_binary(ast->cmd, shell));
	signal(SIGINT, handle_sigint);
	return (0);
}

int	execute_ast(t_ast *ast, t_sh *shell)
{
	if (!ast)
		return (0);
	if (ast->type == CMD)
		return (execute_command(ast, shell));
	else if (ast->type == PIPE)
		return (execute_pipeline(ast, shell));
	return (0);
}
