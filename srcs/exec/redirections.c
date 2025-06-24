# include "../../minishell.h"

/* int	setup_pipes_redirections(int **pipes, int nb_pipes, int i)
{
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0) //le premier pipe devient le stdin de la 2ere CMD et ainsi de suite (read end)
			return (1);
	}
	if (i < nb_pipes)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) < 0) //idem avec stdin si pas derniere CMD (write end)
			return (1);
	}
	return (0);
} */

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

int	redirect_in(t_redirect *redir)
{
	int fd;

	if (redir->type != REDIR_HEREDOC)
		fd = open(redir->target, O_RDONLY);
	else
		fd = redir->fd;
	if (fd < 0)
		return (perror("open"), ERROR);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close (fd);
		return (perror("dup2"), ERROR);
	}
	close(fd);
	return (SUCCESS);
}

int	redirect_out(t_redirect *redir)
{
	int fd;

	if (redir->type == REDIR_OUT)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		printf_fd(STDERR, "minishell: %s: Permission denied\n", redir->target);
		return (/* perror("open"),  */ERROR);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close (fd);
		return (perror("dup2"), ERROR);
	}
	close(fd);
	return (SUCCESS);
}

int	apply_redirections(t_command *cmd)
{
	t_redirect *redir;

	if (!cmd->redirections)
		return (0); //pas d'erreur si pas de redirections
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			if (redirect_in(redir) != SUCCESS)
				return (ERROR);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (redirect_out(redir) != SUCCESS)
				return (ERROR);
		}
		redir = redir->next;
	}
	return (SUCCESS);
}
