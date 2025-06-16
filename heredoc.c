#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdlib.h>

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

char	*handle_heredoc(char * delimiter)
{
	char 	*input;
	char	*buffer;
	char 	*temp;

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
		temp = ft_nwljoin(buffer, input);
		free(input);
		if (!temp)
			return (free(buffer), NULL);
		free(buffer);
		buffer = temp;
	}
	return (buffer);
}

int main(void)
{
	t_redirect redir;
	char *buffer;

	init_heredoc(&redir, "FIN");
	buffer = handle_heredoc(redir.target);
	printf("%s\n", buffer);
	free(buffer);
	free(redir.target);
	return (0);
}