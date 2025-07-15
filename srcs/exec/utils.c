#include "../../minishell.h"

int	process_wait_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (ERROR);
}

bool	is_numeric(char *arg)
{
	int	i;

	i = 0;
	while (ft_isspace(arg[i]))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!arg[i])
		return (false);
	while (arg[i])
	{
		if (!ft_isdigit((arg[i])))
			return (false);
		i++;
	}
	return (true);
}

char	*ft_charjoin(char const *s1, char const *s2, char c)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	if (!s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = (void *) malloc((len1 + len2 + 2) * sizeof(char));
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	result[len1] = c;
	ft_memcpy(result + len1 + 1, s2, len2);
	result[len1 + len2 + 1] = '\0';
	return (result);
}
