#include "../../minishell.h"

int	builtin_echo(char **args)
{
	int		i;
	int		j;
	bool	newline;

	if (!args || !args[0])
		return (BUILTIN_ERR);
	newline = true;
	i = 1;
	while (args[i] && (ft_strncmp(args[i], "-n", 2)) == 0)
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		newline = false;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (SUCCESS);
}
