#include "../../minishell.h"

int	builtin_pwd(char **args)
{
	char	buffer[PATH_MAX];

	if (args[1] && args[1][0] == '-' && args[1][1])
	{
		if (!(args[1][1] == '-' && args[1][2] == '\0'))
		{
			printf_fd(STDERR,"minishell: pwd: %s: invalid option\n", args[1]);
			return (BUILTIN_ERR);
		}
	}
	if (!getcwd(buffer, sizeof(buffer)))
	{
		perror("getcwd");
		return (ERROR);
	}
	ft_putendl_fd(buffer, 1);
	return (SUCCESS);
}
