/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 19:51:03 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/01 00:37:07 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_fstrjoin(char **s1, char **s2, int flag)
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

/* static bool delimiter_matches(char *delimiter, char *input)
{
    int del_i = 0;
    int input_i = 0;
    
    if (!delimiter || !input)
        return (false);
    
    while (delimiter[del_i])
    {
        // Skipper les quotes dans le délimiteur
        if (delimiter[del_i] == '"' || delimiter[del_i] == '\'')
        {
            del_i++;
            continue;
        }
        
        // Si input est fini mais pas délimiteur (sans quotes)
        if (!input[input_i])
            return (false);
            
        // Comparer les caractères
        if (delimiter[del_i] != input[input_i])
            return (false);
            
        del_i++;
        input_i++;
    }
    
    // input doit être fini aussi
    return (input[input_i] == '\0');
} */

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

/* int main(void)
{
	t_redirect redir;
	t_sh	shell;
	char buffer[1024];
	int	bytes_read;

	shell.exit_status = 0;
	init_heredoc(&redir, "EOF");
	printf("del: %s\n", redir.target);
	redir.fd = handle_heredoc(redir.target, &shell);
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
} */