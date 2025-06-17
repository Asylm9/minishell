#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <stdbool.h>

typedef enum e_token_type
{
	CMD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	//t_token			*next;
}			t_token;

typedef enum e_redir_type
{
	IN,      // <
	OUT,     // >
	APPEND,  // >>
	HEREDOC  // <<
}			t_redir_type;

typedef struct s_redirect
{
	t_redir_type	type;
	char			*target; // fichier ou delimiteur si heredoc
	int 			fd;      // pour heredoc
	//t_redirect		*next;
}			t_redirect;

int	init_heredoc(t_redirect *redir, char *delimiter)
{
	redir->type = HEREDOC;
	redir->target = ft_strdup(delimiter);
	//fd
}
/////////////////////////////// MATT FUNCTIONS //////////////////////////////////////

char	*ft_freejoin(char **s1, char **s2, int flag)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = 0;
	len2 = 0;
	if (s1 && *s1)
		len1 = ft_strlen(*s1);
	if (s2 && *s2)
		len2 = ft_strlen(*s2);
	result = (char *)malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	if (s1 && *s1)
		ft_memcpy(result, *s1, len1);
	if (s2 && *s2)
		ft_memcpy(result + len1, *s2, len2);
	result[len1 + len2] = '\0';
	if (flag == 1 || flag == 3)
		if (s1 && *s1)
			free(*s1);
	if (flag == 2 || flag == 3)
		if (s2 && *s2)
			free(*s2);
	return (result);
}

int	expand_var(char *input, char **result)
{
	int		i;
	char	*var;

	// char	*result;
	i = 1;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	var = ft_substr(input, 0, i);
	(*result) = getenv(var + 1);
	free(var);
	if (result == NULL)
		return (1);
	return (0);
}

char	*expand_token(char *input)
{
	char	*result;
	char	*buffer;
	char	*tmp;
	int		pos;
	int		start;

	result = ft_calloc(1, 1);
	buffer = NULL;
	tmp = NULL;
	pos = 0;
	start = 0;
	while (input[pos] != '\0')
	{
		if (input[pos] == '$')
		{
			buffer = ft_substr(input, start, pos - start);
			tmp = ft_freejoin(&result, &buffer, 0);
			if (result)
			{
				free(result);
				result = NULL;
			}
			if (buffer)
			{
				free(buffer);
				buffer = NULL;
			}
			if (expand_var(input + pos, &buffer))
			{
				result = ft_strdup("");
				free(tmp);
				return (result); // Error handling if variable expansion fails
			}
			if (buffer)
			{
				result = ft_freejoin(&tmp, &buffer, 0);
				free(tmp);
				// Do NOT free buffer if it comes from getenv!
			}
			else
			{
				if (result)
				{
					free(result);
					result = NULL;
				}
				result = tmp;
			}
			pos++;
			while ((ft_isalnum(input[pos]) || input[pos] == '_') && input[pos])
				pos++;
			start = pos;
		}
		else
			pos++;
	}
	buffer = ft_substr(input, start, pos - start);
	tmp = ft_freejoin(&result, &buffer, 0);
	free(result);
	free(buffer);
	result = tmp;
	return (result);
}

/////////////////////////////// MATT FUNCTIONS //////////////////////////////////////

bool	has_quotes(char *delimiter)
{
	int	end;

	end = ft_strlen(delimiter) - 1;
	if (delimiter[0] == '"' && delimiter[end] == '"')
		return (true);
	return (false);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char) s2[i]);
		i++;
	}
	return (0);
}

char	*ft_nwljoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*res;

	if (!s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	res = (void *) malloc((ft_strlen(s1) + ft_strlen(s2) + 2) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i++] = '\n';
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}

int	handle_heredoc(char * delimiter)
{
	char 	*input;
	char	*line;
	char	*buffer;
	char 	*temp;
	int		pfd[2];

	if (pipe(pfd) < 0)
		return (1);
	buffer = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (ft_strcmp(delimiter, input) == 0)
		{
			free(input);
			break ;
		}
		if (!has_quotes(delimiter))
			line = expand_token(input);
		else
			line = ft_strdup(input);
		temp = ft_nwljoin(buffer, line);
		free(input);
		free(line);
		if (!temp)
		{
			close(pfd[0]);
			close(pfd[1]);
			return (free(buffer), 1);
		}
		free(buffer);
		buffer = temp;
	}
	if (buffer)
	{
		write(pfd[1], buffer, ft_strlen(buffer));
		free(buffer);
	
	}
	close(pfd[1]);
	return (pfd[0]);
}

int main(void)
{
	t_redirect redir;
	char buffer[1024];
	int	bytes_read;

	init_heredoc(&redir, "\"FIN\"");
	redir.fd = handle_heredoc(redir.target);
	if (redir.fd > 0)
	{
		printf("**** Contenu du heredoc ****\n");
		while ((bytes_read = read(redir.fd, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_read] = '\0';
			printf("%s", buffer);
		}
		printf("\n**** Fin ****\n");
		close(redir.fd);
	}
	else
		printf("Heredoc error\n");
	free(redir.target);
	return (0);
}