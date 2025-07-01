
#include "minishell.h"

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
			return ((unsigned char) s1[i] - (unsigned char) s2[j]);
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

static int	read_heredoc_content(char *delimiter, t_sh *shell, char **buffer)
{
	char	*input;
	char	*line;
	char	*temp;

	*buffer = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		line = process_heredoc_line(input, delimiter, shell);
		if (!line)
			break ;
		temp = ft_charjoin(*buffer, line, '\n');
		free(line);
		if (!temp)
			return (free(*buffer), 1);
		free(*buffer);
		*buffer = temp;
	}
	return (0);
}

int	handle_heredoc(char *delimiter, t_sh *shell)
{
	char	*buffer;
	char	*temp;
	int		pfd[2];

	if (pipe(pfd) < 0)
		return (1);
	if (read_heredoc_content(delimiter, shell, &buffer) != SUCCESS)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (1);
	}
	if (buffer)
	{
		temp = ft_strjoin(buffer, "\n");
		if (temp)
		{
			free(buffer);
			buffer = temp;
		}
		write(pfd[1], buffer, ft_strlen(buffer));
		free(buffer);
	}
	close(pfd[1]);
	return (pfd[0]);
}
