#include "minishell.h"

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

int	redirect_in(t_redirect *redir)
{
	int fd;

	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
		return (perror("open"), 1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close (fd);
		return (perror("dup2"), 1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_command *cmd)
{
	t_redirect *redir;
	int	fd;

	if (!cmd->redirections)
		return (0); //pas d'erreur si pas de redirections
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == IN)
		{
			if (redirect_in(redir) == 1) //a voir
				return (1);
		}
		else if (redir->type == OUT)
		{
			fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == APPEND)
		{
			fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == HEREDOC)
		{
			//voir comment heredoc fonctionne
		}
		redir = redir->next;
	}
	return (0);
}
