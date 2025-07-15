/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:34:17 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/15 19:08:49 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	has_quotes(char *delimiter)
{
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	dquote = 0;
	squote = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '"')
			dquote++;
		if (delimiter[i] == '\'')
			squote++;
		i++;
	}
	if (dquote >= 2 || squote >= 2)
		return (true);
	else
		return (false);
}

int	del_compare(const char *s1, const char *s2)
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


static char	*process_heredoc_line(char *input, char *delimiter, t_sh *shell)
{
	char	*line;

	if (del_compare(delimiter, input) == 0)
	{
		free(input);
		return (NULL);
	}
	if (has_quotes(delimiter) == false)
		line = expand_token(input, shell);
	else
		line = ft_strdup(input);
	free(input);
	return (line);
}

static int	sigint_in_heredoc(char *input, char **buffer)
{
    free(input);
    free(*buffer);
    *buffer = NULL;
    signal(SIGINT, handle_sigint);
    return (1);
}

static int	eof_in_heredoc(int count, char *delimiter)
{
    printf_fd(STDIN_FILENO,
        "bash: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n",
        count,
        delimiter);
    return (0);
}

static int	append_line_to_buffer(char **buffer, char *line)
{
    char	*temp;

    temp = ft_charjoin(*buffer, line, '\n');
    free(line);
    if (!temp)
    {
        free(*buffer);
        return (1);
    }
    free(*buffer);
    *buffer = temp;
    return (0);
}
static int	read_heredoc_loop(char *delimiter, t_sh *shell, char **buffer, int *count)
{
    char	*input;
    char	*line;
    int		ret;

    input = NULL;
    line = NULL;
    while (1)
    {
        input = readline("> ");
        if (g_sig == SIGINT)
            return (sigint_in_heredoc(input, buffer));
        if (!input)
        {
            eof_in_heredoc(*count, delimiter);
            break ;
        }
        line = process_heredoc_line(input, delimiter, shell);
        if (!line)
            break ;
        ret = append_line_to_buffer(buffer, line);
        if (ret != 0)
            return (1);
        (*count)++;
    }
    return (0);
}

static int	read_heredoc_content(char *delimiter, t_sh *shell, char **buffer)
{
    int	count;
    int	ret;

    count = 1;
    *buffer = NULL;
    g_sig = 0;
    signal(SIGINT, handle_here_sig);
    ret = read_heredoc_loop(delimiter, shell, buffer, &count);
    signal(SIGINT, handle_sigint);
    return (ret);
}

static void	write_buffer_to_pipe(char *buffer, int fd)
{
    char	*temp;

    temp = NULL;
    if (buffer)
    {
        temp = ft_strjoin(buffer, "\n");
        if (temp)
        {
            free(buffer);
            buffer = temp;
        }
        write(fd, buffer, ft_strlen(buffer));
        free(buffer);
    }
}

static int	handle_heredoc_child(char *delimiter, t_sh *shell, int *pfd)
{
    char	*buffer;

    buffer = NULL;
    if (read_heredoc_content(delimiter, shell, &buffer) != SUCCESS)
    {
		close(pfd[0]);
		close(pfd[1]);
		cleanup_shell(shell, NULL);
        exit(130);
    }
    write_buffer_to_pipe(buffer, pfd[1]);
    close_all_fds(3);
	close(pfd[0]);
	close(pfd[1]);
	cleanup_shell(shell, NULL);
    exit(0);
    return (0);
}

int	handle_heredoc(char *delimiter, t_sh *shell)
{
    int		pfd[2];
    int		pid;
    int		status;

    if (pipe(pfd) < 0)
        return (1);
    signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid == 0)
		handle_heredoc_child(delimiter, shell, pfd);
	close(pfd[1]);
    waitpid(pid, &status, 0);
    signal(SIGINT, handle_sigint);
    shell->exit_status = process_wait_status(status);
    if (shell->exit_status == 130)
        return (close(pfd[0]), shell->exit_status);
    return (pfd[0]);
}
