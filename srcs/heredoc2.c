#include "../minishell.h"

static int	sigint_in_heredoc(char *input, char **buffer)
{
	free(input);
	free(*buffer);
	*buffer = NULL;
	signal(SIGINT, handle_sigint);
	return (1);
}

static int	eof_in_heredoc(int count, char *del)
{
	printf_fd(STDIN_FILENO,
		"bash: warning: here-document at line %d ",
		count);
	printf_fd(STDIN_FILENO,
		"delimited by end-of-file (wanted `%s')\n", del);
	return (0);
}

static int	append_line_to_buffer(char **buffer, char *line)
{
	char	*temp;

	temp = ft_charjoin(*buffer, line, '\n');
	free(line);
	free(*buffer);
	if (!temp)
		return (1);
	*buffer = temp;
	return (0);
}

int	process_readline_loop(char *del, t_sh *shell, char **buffer, int *count)
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
			eof_in_heredoc(*count, del);
			break ;
		}
		line = process_heredoc_line(input, del, shell);
		if (!line)
			break ;
		ret = append_line_to_buffer(buffer, line);
		if (ret)
			return (ERROR);
		(*count)++;
	}
	return (SUCCESS);
}

int	read_heredoc_content(char *del, t_sh *shell, char **buffer)
{
	int		count;
	int		ret;

	*buffer = NULL;
	count = 1;
	g_sig = 0;
	signal(SIGINT, handle_here_sig);
	ret = process_readline_loop(del, shell, buffer, &count);
	signal(SIGINT, handle_sigint);
	return (ret);
}
