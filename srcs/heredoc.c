/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:12:00 by agaland           #+#    #+#             */
/*   Updated: 2025/07/25 15:44:31 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	has_quotes(char *del)
{
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	dquote = 0;
	squote = 0;
	while (del[i])
	{
		if (del[i] == '"')
			dquote++;
		if (del[i] == '\'')
			squote++;
		i++;
	}
	if (dquote >= 2 || squote >= 2)
		return (true);
	else
		return (false);
}

static int	del_compare(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i] || s2[j])
	{
		while (s1[i] == '"' || s1[i] == '\'')
			i++;
		if (!s1[i] && !s2[j])
			return (0);
		if ((!s1[i] || !s2[j]) || s1[i] != s2[j])
			return ((unsigned char)s1[i] - (unsigned char)s2[j]);
		i++;
		j++;
	}
	return (0);
}

char	*process_heredoc_line(char *input, char *del, t_sh *shell)
{
	char	*line;

	if (del_compare(del, input) == 0)
	{
		free(input);
		return (NULL);
	}
	if (has_quotes(del) == false)
		line = expand_token(input, shell);
	else
		line = x_strdup(input, shell, NULL, NULL);
	free(input);
	return (line);
}

void	heredoc_child(char *del, t_sh *shell, int *pfd)
{
	char	*buffer;
	char	*temp;
	int		ret;

	buffer = NULL;
	ret = read_heredoc_content(del, shell, &buffer);
	if (ret > 0)
	{
		if (ret == SIGINT)
			fd_clean_exit(shell, pfd, 130);
		fd_clean_exit(shell, pfd, ERROR);
	}
	if (buffer)
	{
		temp = ft_strjoin(buffer, "\n");
		free(buffer);
		if (!temp)
			fd_clean_exit(shell, pfd, ERROR);
		buffer = temp;
		write(pfd[1], buffer, ft_strlen(buffer));
		free(buffer);
	}
	close_all_fds(3);
	fd_clean_exit(shell, pfd, SUCCESS);
}

int	handle_heredoc(char *del, t_sh *shell)
{
	int	pfd[2];
	int	pid;
	int	status;

	if (pipe(pfd) < 0)
		return (ERROR);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		heredoc_child(del, shell, pfd);
	close(pfd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	shell->exit_status = process_wait_status(status);
	if (shell->exit_status == 130)
		return (close(pfd[0]), shell->exit_status);
	return (pfd[0]);
}
